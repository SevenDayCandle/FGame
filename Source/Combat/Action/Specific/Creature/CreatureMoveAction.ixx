export module fab.CreatureMoveAction;

import fab.CallbackAction;
import fab.CallbackVFX;
import fab.CombatInstance;
import fab.CombatSquare;
import fab.FUtil;
import fab.VFXAction;
import std;

namespace fab {
	export class CreatureMoveAction : public VFXAction {
	public:
		struct Listener {
			virtual ~Listener() = default;

			virtual uptr<CallbackVFX> creatureMoveVFX(const OccupantObject* occupant, const CombatSquare* target) = 0;
		};

		CreatureMoveAction(CombatInstance& instance, OccupantObject* occupant, CombatSquare* target, bool isManual, bool isDestination = true):
			VFXAction(instance), target(target), isManual(isManual), isDestination(isDestination), occupant(occupant) {}
		virtual ~CreatureMoveAction() = default;

		inline CreatureMoveAction& setIsDestination(bool val) { return isDestination = val, *this; }
		inline CreatureMoveAction& setIsManual(bool val) { return isManual = val, *this; }

		bool isDestination = true;
		bool isManual = false;
		CombatSquare* target;
		OccupantObject* occupant;

		virtual bool isSuccess() override;
		virtual void start() override;
	protected:
		uptr<CallbackVFX> getVfx() override;
	};

	bool CreatureMoveAction::isSuccess() {
		return occupant ? occupant->currentSquare == target : target ? target->getOccupant() == occupant : false;
	}

	void CreatureMoveAction::start() {
		if (!occupant->canMoveTo(target, isDestination, isManual)) {
			isDone = true;
		}
		else {
			VFXAction::start();
			if (occupant) {
				if (target) {
					target->setOccupant(occupant);
				}
				else {
					occupant->currentSquare = nullptr;
				}
				occupant->onMoved();
			}
			else if (!occupant && target) {
				target->setOccupant(occupant);
			}
		}
	}

	uptr<CallbackVFX> CreatureMoveAction::getVfx() {
		Listener* listener = dynamic_cast<Listener*>(instance.viewSubscriber);
		return listener ? listener->creatureMoveVFX(occupant, target) : uptr<CallbackVFX>();
	}
}