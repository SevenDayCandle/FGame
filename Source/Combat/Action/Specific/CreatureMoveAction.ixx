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

		inline CreatureMoveAction& setIsDestination(bool val) { return isDestination = val, *this; }

		bool isDestination = false;
		CombatSquare* target;
		OccupantObject* occupant;

		virtual bool isSuccess() override;
		virtual void start() override;
	};

	bool CreatureMoveAction::isSuccess() {
		return occupant ? occupant->currentSquare == target : target ? target->getOccupant() == occupant : false;
	}

	void CreatureMoveAction::start() {
		if (occupant) {
			if (target && occupant->canMoveTo(target, isDestination)) {
				target->setOccupant(occupant);
			}
			else {
				occupant->currentSquare = nullptr;
				occupant->onMoved();
			}
		}
		else if (!occupant && target) {
			target->setOccupant(occupant);
		}
	}
}