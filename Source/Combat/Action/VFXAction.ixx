export module fab.VFXAction;

import fab.Action;
import fab.CallbackAction;
import fab.CallbackVFX;
import fab.FUtil;
import fab.FWindow;
import fab.UIScreen;
import std;

namespace fab {
	export template <typename T = Action> class VFXAction : public CallbackAction<T> {
	public:
		VFXAction(const func<uptr<CallbackVFX>()>& vfxFunc) : CallbackAction<T>(), vfxFunc(vfxFunc) {}
		VFXAction(func<uptr<CallbackVFX>()>&& vfxFunc) : CallbackAction<T>(), vfxFunc(move(vfxFunc)) {}
		VFXAction(uptr<CallbackVFX>&& vfx) : CallbackAction<T>(), vfxFunc([v = move(vfx)]() mutable {
			return move(v);
		}) {}

		bool isDone = false;

		inline virtual bool run() override { return isDone; }

		virtual void start() override;
	protected:
		func<uptr<CallbackVFX>()> vfxFunc;
	};

	template <typename T> void VFXAction<T>::start()
	{
		uptr<CallbackVFX> vfx = vfxFunc();
		if (vfx) {
			UIScreen* screen = dynamic_cast<UIScreen*>(vfx->win.currentScreen());
			if (screen) {
				vfx->setOnComplete([this](CallbackVFX& v) {isDone = true; });
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