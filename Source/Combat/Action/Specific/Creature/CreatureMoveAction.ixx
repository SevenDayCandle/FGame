export module fab.CreatureMoveAction;

import fab.CallbackAction;
import fab.CallbackVFX;
import fab.CombatInstance;
import fab.CombatSquare;
import fab.FUtil;
import fab.SequentialAction;
import fab.VFXAction;
import std;

namespace fab {
	export class CreatureMoveAction : public VFXAction<CreatureMoveAction> {
	public:
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

		static uptr<SequentialAction> pathMove(CombatInstance& instance, OccupantObject* occupant, vec<CombatSquare*> path, bool isManual);
	protected:
		uptr<CallbackVFX> getVfx() override;
	};

	bool CreatureMoveAction::isSuccess() {
		return occupant ? occupant->currentSquare == target : target ? target->getOccupant() == occupant : false;
	}

	void CreatureMoveAction::start() {
		VFXAction::start();
		if (occupant) {
			if (target && occupant->canMoveTo(target, isDestination, isManual)) {
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

	uptr<SequentialAction> CreatureMoveAction::pathMove(CombatInstance& instance, OccupantObject* occupant, vec<CombatSquare*> path, bool isManual) {
		uptr<SequentialAction> action = make_unique<SequentialAction>(instance);
		for (int i = 0; i < path.size(); ++i) {
			action->add(make_unique<CreatureMoveAction>(instance, occupant, path[i], isManual, i >= path.size() - 1));
		}
		return action;
	}

	uptr<CallbackVFX> CreatureMoveAction::getVfx() {
		return instance.viewSubscriber ? instance.viewSubscriber->creatureMoveVFX(occupant, target) : uptr<CallbackVFX>();
	}
}