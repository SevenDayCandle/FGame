export module fab.SequentialAction;

import fab.CombatInstance;
import fab.CallbackAction;
import fab.FUtil;
import std;

namespace fab {
	export class SequentialAction : public CallbackAction<SequentialAction> {
	public:
		SequentialAction(CombatInstance& instance): CallbackAction<SequentialAction>(instance) {}
		template <c_varg<uptr<Action>>... Args> SequentialAction(CombatInstance& instance, Args&&... items) : CallbackAction<SequentialAction>(instance) {
			actions.reserve(sizeof...(items));
			(actions.push_back(move(items)), ...);
		}
		virtual ~SequentialAction() = default;

		inline bool isSuccess() override { return executeIndex >= actions.size(); }
		inline CombatInstance::Action* at(int index) { return index < actions.size() ? actions[index].get() : nullptr; }
		inline int executedCount() const { return executeIndex; }
		inline int totalCount() const { return actions.size(); }
		template <c_varg<uptr<Action>&&>... Args> inline SequentialAction& addAll(Args&&... items) { return (actions.push_back(move(items)), ...), * this; }

		template <c_ext<CombatInstance::Action> T> T& add(uptr<T>&& act);
		virtual bool run() override;
		virtual void start() override;
	protected:
		vec<uptr<CombatInstance::Action>> actions;
	private:
		int executeIndex = 0;
	};

	inline bool SequentialAction::run() {
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

	void SequentialAction::start() {
		if (executeIndex < actions.size()) {
			actions[executeIndex]->start();
		}
	}

	template <c_ext<CombatInstance::Action> T> T& SequentialAction::add(uptr<T>&& act) {
		T& ref = *act;
		actions.push_back(move(act));
		return ref;
	}
}