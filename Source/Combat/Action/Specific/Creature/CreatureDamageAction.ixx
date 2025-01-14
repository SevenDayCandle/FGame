export module fab.CreatureDamageAction;

import fab.CallbackAction;
import fab.CallbackVFX;
import fab.CombatInstance;
import fab.CombatSquare;
import fab.FUtil;
import fab.OccupantObject;
import fab.TurnObject;
import fab.UIScreen;
import fab.VFXAction;
import std;

namespace fab {
	export class CreatureDamageAction : public VFXAction {
	public:
		struct Listener {
			virtual ~Listener() = default;

			virtual uptr<CallbackVFX> damageVFX(const OccupantObject* target, int newHealth) = 0;
		};

		CreatureDamageAction(CombatInstance& instance, TurnObject* source, OccupantObject* target, int amount): VFXAction(instance), source(source), target(target), amount(amount) {}
		virtual ~CreatureDamageAction() = default;

		int amount;
		OccupantObject* target;
		TurnObject* source;

		// TODO damage vs direct hp loss flag

		virtual bool isSuccess() override;
		virtual void start() override;
	protected:
		int resultHealth = -1;

		uptr<CallbackVFX> getVfx() override;
	};

	// TODO proper condition
	bool CreatureDamageAction::isSuccess() {
		return target && resultHealth >= 0;
	}

	void CreatureDamageAction::start() {
		if (target && target->canDamage()) {
			resultHealth = target->addHealth(-amount);
			VFXAction::start();
			// TODO calculations if required
			// TODO hooks
			// TODO death checks
		}
	}

	uptr<CallbackVFX> CreatureDamageAction::getVfx() {
		Listener* listener = dynamic_cast<Listener*>(instance.viewSubscriber);
		return listener ? listener->damageVFX(target, resultHealth) : uptr<CallbackVFX>();
	}
}