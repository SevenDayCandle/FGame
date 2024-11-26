export module fab.CreatureBehaviorAction;

import fab.Action;
import fab.CombatInstance;
import fab.Creature;
import fab.FUtil;
import std;

namespace fab {
	export class CreatureBehaviorAction : public Action {
	public:
		CreatureBehaviorAction(CombatInstance& instance, Creature& creature): Action(instance), creature(creature) {}
		virtual ~CreatureBehaviorAction() = default;

		Creature& creature;
	};
}