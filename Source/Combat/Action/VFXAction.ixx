export module fab.VFXAction;

import fab.CombatInstance;
import fab.CallbackAction;
import fab.CallbackVFX;
import fab.FUtil;
import fab.FWindow;
import fab.UIScreen;
import std;

namespace fab {
	export template <typename T = CombatInstance::Action > class VFXAction : public CallbackAction<T> {
	public:
		VFXAction(CombatInstance& instance) : CallbackAction<T>(instance) {}

		bool isDone = false;

		inline virtual bool run() override { return isDone; }

		virtual void start() override;
	protected:
		virtual uptr<CallbackVFX> getVfx() = 0;
	};

	template <typename T> void VFXAction<T>::start()
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