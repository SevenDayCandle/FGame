export module fab.CombatInstance;

import fab.Action;
import fab.CallbackVFX;
import fab.CombatSquare;
import fab.CombatTurn;
import fab.CoreContent;
import fab.FUtil;
import fab.IDrawable;
import fab.OccupantObject;
import fab.RunEncounter;
import fab.RunZone;
import fab.SavedCreatureEntry;
import fab.TurnObject;
import std;

namespace fab {
	export constexpr int DEFAULT_ROUND_LENGTH = 100;
	export constexpr arr<int, 4> DIR_X = { -1, 1, 0, 0 };
	export constexpr arr<int, 4> DIR_Y = { 0, 0, -1, 1 };

	export class CombatInstance {
	public:
		class Subscriber {
		public:
			virtual ~Subscriber() = default;

			virtual void subscribe(CombatInstance& instance) = 0;
			virtual void unsubscribe() = 0;
		protected:
			Subscriber() {}
		};

		template <typename T> class SubscriberD : public virtual Subscriber {
		public:
			virtual ~SubscriberD() {
				if (instance) {
					instance->unsubscribe<T>(static_cast<T*>(this));
				}
			}

			inline void subscribe(CombatInstance& instance) final { 
				instance.subscribe<T>(static_cast<T*>(this)); 
				this->instance = &instance;
			}
			inline void unsubscribe() final {
				if (instance) {
					instance->unsubscribe<T>(static_cast<T*>(this));
					instance = nullptr;
				}
			}
		protected:
			SubscriberD() {}
			CombatInstance* instance;
		};

		struct IViewListener {
			virtual ~IViewListener() = default;

			virtual void onActionBegin(const Action* action) {}
			virtual void onActionEnd(const Action* action, bool isLast) {}
			virtual void onPlayerTurnBegin(const CombatTurn* turn) {}
			virtual void onPlayerTurnEnd(const CombatTurn* turn) {}
			virtual void onRoundEnd() {}
			virtual void onTurnAdded(const CombatTurn& turn) {}
			virtual void onTurnChanged(ref_view<const mset<CombatTurn>> turns) {}
			virtual void onTurnRemoved(const CombatTurn* turn) {}
		};

		CombatInstance() {}

		IViewListener* viewSubscriber;

		inline auto getOccupants() const { return std::views::transform(occupants, [](const uptr<OccupantObject>& item) {return item.get(); }); }
		inline bool hasAction() const { return currentActionLow != nullptr || currentAction != nullptr; }
		inline bool isCompleted() const { return completed; }
		inline CombatSquare* getDistanceSource() const { return distanceOrigin; }
		inline CombatTurn* getCurrentTurn() const { return currentTurn; }
		inline Action* getCurrentAction() const { return currentAction ? currentAction : currentActionLow; }
		inline int getCurrentRound() const { return totalActionTime / roundTime; }
		inline int getFieldColumns() const { return fieldColumns; }
		inline int getFieldRows() const { return fieldRows; }
		inline int getSquareIndex(int col, int row) const { return col + fieldColumns * row; }
		inline int getTotalActionTime() const { return totalActionTime; }
		inline ref_view<const mset<CombatTurn>> getTurns() const { return std::views::all(turns); }
		inline ref_view<const vec<CombatSquare>> getSquares() const { return std::views::all(squares); }
		inline RunZone* getZone() const { return zone; }
		template <typename T> inline auto getSubscribers() { return std::views::transform(subscribers[typeid(T)], [](void* s) {return static_cast<T*>(s); }); }
		template <c_ext<Action> T> inline T& queueAction(uptr<T>&& action) {
			T& ref = *action;
			queueActionImpl(move(action));
			return ref;
		}
		template <c_ext<Action> T> inline T& queueActionFront(uptr<T>&& action) {
			T& ref = *action;
			queueActionImplFront(move(action));
			return ref;
		}
		template <c_ext<Action> T, typename... Args> requires std::constructible_from<T, Args&&...> inline T& queueNew(Args&&... args) { return queueAction(make_unique<T>(forward<Args>(args)...)); };
		template <c_ext<Action> T, typename... Args> requires std::constructible_from<T, Args&&...> inline T& queueNewFront(Args&&... args) { return queueActionFront(make_unique<T>(forward<Args>(args)...)); };
		template <c_ext<Action> T, typename... Args> requires std::constructible_from<T, CombatInstance&, Args&&...> inline T& queueNew(Args&&... args) { return queueAction(make_unique<T>(*this, forward<Args>(args)...)); };
		template <c_ext<Action> T, typename... Args> requires std::constructible_from<T, CombatInstance&, Args&&...> inline T& queueNewFront(Args&&... args) { return queueActionFront(make_unique<T>(*this, forward<Args>(args)...)); };
		template <c_ext<Subscriber> T> inline T* subscribe(T* subscriber) {
			subscribers[typeid(T)].push_back(subscriber);
			return subscriber;
		};
		template <typename T> inline void unsubscribe(T* subscriber) {std::erase(subscribers[typeid(T)], subscriber);};

		bool modifyTurnOrder(const TurnObject& target, int diff);
		bool nextTurn();
		bool update();
		CombatSquare* getSquare(int col, int row);
		int getDistanceTo(CombatSquare* square);
		OccupantObject* getCurrentActor() const;
		vec<CombatSquare*> findShortestPath(const CombatSquare* targ, int limit = futil::INT_MAX);
		void endCombat();
		void endCurrentTurn();
		void fillDistances(CombatSquare* origin);
		void initialize(RunEncounter& encounter, vec<SavedCreatureEntry>& runCreatures, int playerFaction);
		void queueActionImpl(uptr<Action>&& action);
		void queueActionImplFront(uptr<Action>&& action);
		void queueCompleteTurn();
		void queueTurn(TurnObject& source, int actionValue);
		void requestQueueTurn(TurnObject& object);
	private:
		bool completed;
		CombatSquare* distanceOrigin;
		CombatTurn* currentTurn;
		deque<uptr<Action>> actionQueue;
		deque<uptr<Action>> actionQueueLow;
		Action* currentAction;
		Action* currentActionLow;
		mset<CombatTurn> turns;
		int fieldColumns = 1;
		int fieldRows = 1;
		int roundTime = DEFAULT_ROUND_LENGTH;
		int totalActionTime = 0;
		RunZone* zone;
		umap<type_index, vec<void*>> subscribers;
		vec<CombatSquare> squares;
		vec<uptr<OccupantObject>> occupants;

		int getSquareIndexAllowRandom(int col, int row);
	};
}