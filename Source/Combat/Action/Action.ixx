export module fab.Action;

import fab.FUtil;
import std;

namespace fab {
	export class CombatInstance;

	export class Action {
	public:
		Action(CombatInstance& instance) : instance(instance) {}
		virtual ~Action() = default;

		CombatInstance& instance;

		virtual inline bool isLowPriority() { return false; }
		virtual inline bool isSuccess() { return true; }
		virtual inline bool run() { return true; }
		virtual inline void complete() {}

		virtual void start() = 0;
	};
}