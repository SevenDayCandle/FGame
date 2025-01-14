export module fab.SequentialAction;

import fab.Action;
import fab.CombatInstance;
import fab.CallbackAction;
import fab.FUtil;
import fab.TurnObject;
import std;

namespace fab {
	export template <c_ext<Action> T = Action> class SequentialAction : public CallbackAction {
	public:
		SequentialAction(CombatInstance& instance): CallbackAction(instance) {}
		SequentialAction(CombatInstance& instance, TurnObject* source) : CallbackAction(instance, source) {}
		template <c_varg<uptr<T>>... Args> SequentialAction(CombatInstance& instance, Args&&... items) : CallbackAction(instance) {
			actions.reserve(sizeof...(items));
			(actions.push_back(move(items)), ...);
		}
		template <c_varg<uptr<T>>... Args> SequentialAction(CombatInstance& instance, TurnObject* source, Args&&... items) : CallbackAction(instance, source) {
			actions.reserve(sizeof...(items));
			(actions.push_back(move(items)), ...);
		}
		virtual ~SequentialAction() = default;

		inline auto getActions() const { return std::views::transform(actions, [](const uptr<T>& x) {return x.get(); }); }
		inline bool isSuccess() override { return executeIndex >= actions.size(); }
		inline int executedCount() const { return executeIndex; }
		inline int totalCount() const { return actions.size(); }
		inline T* at(int index) { return index < actions.size() ? actions[index].get() : nullptr; }
		inline T* lastExecuted() { return at(executeIndex); }
		template <c_ext<T> U, typename... Args> requires std::constructible_from<U, CombatInstance&, Args&&...> inline U& addNew(Args&&... args) { return add(make_unique<U>(this->instance, forward<Args>(args)...)); }
		template <c_varg<uptr<T>&&>... Args> inline SequentialAction<T>& addAll(Args&&... items) { return (actions.push_back(move(items)), ...), * this; }

		template <c_ext<T> U> U& add(uptr<U>&& act);
		virtual bool run() override;
		virtual void start() override;
	protected:
		vec<uptr<T>> actions;
	private:
		int executeIndex = 0;
	};

	template <c_ext<Action> T> bool SequentialAction<T>::run() {
		if (executeIndex >= actions.size()) {
			return true;
		}
		if (actions[executeIndex]->run()) {
			if (!actions[executeIndex]->isSuccess()) {
				return true;
			}
			++executeIndex;
			if (executeIndex >= actions.size()) {
				return true;
			}
			actions[executeIndex]->start();
		}
		return false;
	}

	template <c_ext<Action> T> void SequentialAction<T>::start() {
		if (executeIndex < actions.size()) {
			actions[executeIndex]->start();
		}
	}

	template <c_ext<Action> T> template <c_ext<T> U> U& SequentialAction<T>::add(uptr<U>&& act) {
		U& ref = *act;
		actions.push_back(move(act));
		return ref;
	}
}