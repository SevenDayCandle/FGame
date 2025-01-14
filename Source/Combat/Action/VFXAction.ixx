export module fab.VFXAction;

import fab.Action;
import fab.CombatInstance;
import fab.CallbackAction;
import fab.CallbackVFX;
import fab.FUtil;
import fab.FWindow;
import fab.TurnObject;
import fab.UIScreen;
import std;

namespace fab {
	export class VFXAction : public CallbackAction {
	public:
		VFXAction(CombatInstance& instance) : CallbackAction(instance) {}
		VFXAction(CombatInstance& instance, TurnObject* source) : CallbackAction(instance, source) {}
		virtual ~VFXAction() = default;

		bool isDone = false;

		inline virtual bool run() override { return isDone; }

		void forceVfx();
		virtual void start() override;
	protected:
		virtual uptr<CallbackVFX> getVfx() = 0;
	};

	void VFXAction::forceVfx() {
		uptr<CallbackVFX> vfx = getVfx();
		if (vfx) {
			UIScreen* screen = dynamic_cast<UIScreen*>(vfx->win.currentScreen());
			if (screen) {
				screen->addVfx(move(vfx));
			}
		}
	}

	void VFXAction::start()
	{
		uptr<CallbackVFX> vfx = getVfx();
		if (vfx) {
			UIScreen* screen = dynamic_cast<UIScreen*>(vfx->win.currentScreen());
			if (screen) {
				vfx->addOnComplete([this](CallbackVFX& v) {isDone = true; });
				screen->addVfx(move(vfx));
			}
			else {
				isDone = true;
			}
		}
		else {
			isDone = true;
		}
	}
}