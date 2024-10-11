export module fab.CombatTurnRenderable;

import fab.CombatTurn;
import fab.CoreContent;
import fab.FUtil;
import fab.FWindow;
import fab.Hitbox;
import fab.IDrawable;
import fab.UIInteractable;
import sdl.SDLBase; 
import sdl.SDLBatchRenderPass;
import sdl.SDLRunner;
import std;

namespace fab {
	export class CombatTurnRenderable : public UIInteractable {
	public:
		CombatTurnRenderable(FWindow& window, uptr<Hitbox> hb, const CombatTurn& turn) : UIInteractable(window, move(hb), window.props.defaultBackground()), portrait(turn.source.getImagePortrait()) {}

		IDrawable& portrait;

		virtual void renderImpl(sdl::SDLBatchRenderPass& rp) override;

		// TODO highlight turn owner when hovered
	};

	void CombatTurnRenderable::renderImpl(sdl::SDLBatchRenderPass& rp)
	{
		UIInteractable::renderImpl(rp);
		portrait.draw(rp, hb->x + hb->w / 8, hb->y + hb->h * 0.15f, hb->w / 2, hb->h * 0.7f, win.getW(), win.getH(), scaleX, scaleY, rotation, &color);
	}
}