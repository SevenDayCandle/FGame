export module fab.CreatureInfoDisplay;

import fab.CoreContent;
import fab.FUtil;
import fab.FWindow;
import fab.Hitbox;
import fab.IDrawable;
import fab.OccupantObject;
import fab.TextDrawable;
import fab.UIInteractable;
import sdl.SDLBase; 
import sdl.SDLBatchRenderPass;
import sdl.SDLRunner;
import std;

namespace fab {
	export class CreatureInfoDisplay : public UIInteractable {
	public:
		CreatureInfoDisplay(FWindow& window, uptr<Hitbox> hb, const OccupantObject& creature) : UIInteractable(window, move(hb), window.props.defaultBackground()),
			creature(creature), portrait(creature.getImagePortrait()), text(window.props.fontRegular(), creature.name()) {}

		const OccupantObject& creature;
		
		virtual void renderImpl(sdl::SDLBatchRenderPass& rp) override;
	private:
		IDrawable& portrait;
		TextDrawable text;
	};

	void CreatureInfoDisplay::renderImpl(sdl::SDLBatchRenderPass& rp) {
		UIInteractable::renderImpl(rp);
		portrait.draw(rp, hb->x + hb->w / 8, hb->y + hb->h * 0.15f, hb->w / 2, hb->h * 0.7f, win.getW(), win.getH(), scaleX, scaleY, rotation, &color);
	}
}