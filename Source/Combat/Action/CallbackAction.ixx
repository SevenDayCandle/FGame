export module fab.CallbackAction;

import fab.Action;
import fab.CombatInstance;
import fab.FUtil;
import std;

namespace fab {
	export class CallbackAction : public Action {
	public:
		CallbackAction(CombatInstance& instance): Action(instance) {}
		CallbackAction(CombatInstance& instance, const func<void(CallbackAction&)>& onComplete) : Action(instance), onComplete(onComplete) {}
		virtual ~CallbackAction() = default;

		template <c_ext<CallbackAction> T> inline CallbackAction& addOnComplete(const func<void(T&)>& newOnComplete) { return CallbackAction::addOnComplete([newOnComplete](CallbackAction& cb) {newOnComplete(static_cast<T&>(cb)); }); }

		CallbackAction& addOnComplete(const func<void(CallbackAction&)>& newOnComplete);

		virtual inline void complete() override;
	protected:
		func<void(CallbackAction&)> onComplete;
	};


	CallbackAction& CallbackAction::addOnComplete(const func<void(CallbackAction&)>& newOnComplete) {
		auto prev = this->onComplete;
		if (prev) {
			this->onComplete = [prev, newOnComplete](auto& val) {
				prev(val);
				newOnComplete(val);
			};
		}
		else {
			this->onComplete = newOnComplete;
		}
		return *this;
	}

	void CallbackAction::complete() {
		if (onComplete) {
			onComplete(*this);
		}
	}
}