export module fab.ArbitraryAction;

import fab.Action;
import fab.CombatInstance;
import fab.FUtil;
import fab.TurnObject;
import std;

namespace fab {
	export class ArbitraryAction : public Action {
	public:
		ArbitraryAction(CombatInstance& instance, const func<void()>& onStart): Action(instance), onStart(onStart) {}
		ArbitraryAction(CombatInstance& instance, TurnObject* source, const func<void()>& onStart) : Action(instance, source), onStart(onStart) {}
		ArbitraryAction(CombatInstance& instance, func<void()>&& onStart) : Action(instance), onStart(move(onStart)) {}
		ArbitraryAction(CombatInstance& instance, TurnObject* source, func<void()>&& onStart) : Action(instance, source), onStart(move(onStart)) {}
		virtual ~ArbitraryAction() = default;

		bool lowPriority;

		ArbitraryAction& setIsLowPriority(bool val) { return lowPriority = val, *this; }

		inline virtual bool isLowPriority() override { return lowPriority; }
		inline virtual void start() override { onStart(); }
	protected:
		func<void()> onStart;
	};
}