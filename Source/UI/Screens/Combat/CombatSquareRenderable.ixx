export module fab.CombatSquareRenderable;

import fab.CombatSquare;
import fab.CoreContent;
import fab.FUtil;
import fab.FWindow;
import fab.Hitbox;
import fab.IDrawable;
import fab.UICallbackInteractable;
import sdl.SDLBase; 
import sdl.SDLBatchRenderPass;
import sdl.SDLRunner;
import std;

namespace fab {
	export class CombatSquareRenderable : public UICallbackInteractable<CombatSquareRenderable> {
	public:
		CombatSquareRenderable(FWindow& window, uptr<Hitbox>&& hb, const CombatSquare& square): UICallbackInteractable<CombatSquareRenderable>(window, move(hb), window.props.defaultPanel()), square(square) {}

		bool valid;
		const CombatSquare& square;
		const sdl::Color* originalColor = &sdl::COLOR_STANDARD;
		IDrawable* arrow;
		sdl::Color pathColor = sdl::COLOR_STANDARD;

		virtual void renderImpl(sdl::SDLBatchRenderPass& rp) override;
	};

	void CombatSquareRenderable::renderImpl(sdl::SDLBatchRenderPass& rp)
	{
		UIImage::renderImpl(rp);
	}
}