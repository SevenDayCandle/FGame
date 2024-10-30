export module fab.CreatureBehaviorAction;

import fab.CombatInstance;
import fab.Creature;
import fab.FUtil;
import std;

namespace fab {
	export class CreatureBehaviorAction : public CombatInstance::Action {
	public:
		CreatureBehaviorAction(CombatInstance& instance, Creature& creature): CombatInstance::Action(instance), creature(creature) {}
		virtual ~CreatureBehaviorAction() = default;

		Creature& creature;
	};
}