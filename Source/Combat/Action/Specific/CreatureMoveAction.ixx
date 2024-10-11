export module fab.CreatureMoveAction;

import fab.CallbackAction;
import fab.CallbackVFX;
import fab.CombatSquare;
import fab.FUtil;
import fab.VFXAction;
import std;

namespace fab {
	export class CreatureMoveAction : public VFXAction<CreatureMoveAction> {
	public:
		CreatureMoveAction(OccupantObject* occupant, CombatSquare* target, const func<uptr<CallbackVFX>()>& vfxFunc): VFXAction(vfxFunc), target(target), occupant(occupant) {}
		virtual ~CreatureMoveAction() = default;

		CombatSquare* target;
		OccupantObject* occupant;

		virtual void start() override;
	};

	void CreatureMoveAction::start() {
		if (target) {
			target->setOccupant(occupant);
		}
		else if (occupant) {
			occupant->currentSquare = nullptr;
			occupant->onMoved();
		}
	}
}