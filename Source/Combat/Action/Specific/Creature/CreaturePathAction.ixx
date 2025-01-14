export module fab.CreaturePathAction;

import fab.CallbackAction;
import fab.CallbackVFX;
import fab.CombatInstance;
import fab.CombatSquare;
import fab.CreatureMoveAction;
import fab.FUtil;
import fab.OccupantObject;
import fab.SequentialAction;
import fab.VFXAction;
import std;

namespace fab {
	export class CreaturePathAction : public SequentialAction<CreatureMoveAction> {
	public:
		CreaturePathAction(CombatInstance& instance, OccupantObject* occupant, vec<CombatSquare*> path, bool isManual): SequentialAction(instance), occupant(occupant) {
			for (int i = 0; i < path.size(); ++i) {
				addNew<CreatureMoveAction>(occupant, path[i], isManual, i >= path.size() - 1);
			}
		}
		virtual ~CreaturePathAction() = default;

		OccupantObject* occupant;

		virtual bool run() override;
	};

	bool CreaturePathAction::run() {
		bool result = SequentialAction::run();
		// On completion, ensure the target is not on a space that is already occupied
		if (result) {
			CombatSquare* square = occupant->currentSquare;
			if (square && square->getOccupant() != occupant) {
				int lastPlace = executedCount() - 1;
				while (lastPlace >= 0) {
					CreatureMoveAction* prevAction = at(executedCount() - 1);
					if (prevAction && !prevAction->target->getOccupant()) {
						prevAction->target->setOccupantForce(occupant);
						prevAction->forceVfx();
						break;
					}
					else {
						--lastPlace;
					}
				}
			}
		}
		return result;
	}
}