export module fab.Action;

import fab.FUtil;
import fab.TurnObject;
import std;

namespace fab {
	export class CombatInstance;

	export class Action {
	public:
		Action(CombatInstance& instance) : instance(instance) {}
		Action(CombatInstance& instance, TurnObject* source) : instance(instance), source(source) {}
		virtual ~Action() = default;

		CombatInstance& instance;
		TurnObject* source;

		inline Action& setSource(TurnObject* source) { return this->source = source, *this; }
		virtual inline bool isLowPriority() { return false; }
		virtual inline bool isSuccess() { return true; }
		virtual inline bool run() { return true; }
		virtual inline void complete() {}

		virtual void start() = 0;
	};
}