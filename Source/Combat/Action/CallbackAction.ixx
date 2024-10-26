export module fab.CallbackAction;

import fab.CombatInstance;
import fab.FUtil;
import std;

namespace fab {
	export template <typename T> class CallbackAction : public CombatInstance::Action {
	public:
		CallbackAction(CombatInstance& instance): CombatInstance::Action(instance) {}
		CallbackAction(CombatInstance& instance, const func<void(T&)>& onComplete) : CombatInstance::Action(instance), onComplete(onComplete) {}
		virtual ~CallbackAction() = default;

		CallbackAction& addOnComplete(const func<void(T&)>& onComplete);

		virtual inline void complete() override;
	protected:
		func<void(T&)> onComplete;
	};

	template<typename T> CallbackAction<T>& CallbackAction<T>::addOnComplete(const func<void(T&)>& onComplete) {
		auto prev = this->onComplete;
		if (prev) {
			this->onComplete = [prev, onComplete](auto& val) {
				prev(val);
				onComplete(val);
			};
		}
		else {
			this->onComplete = onComplete;
		}
		return *this;
	}

	template<typename T> inline void CallbackAction<T>::complete() {
		if (onComplete) {
			onComplete(static_cast<T&>(*this));
		}
	}
}