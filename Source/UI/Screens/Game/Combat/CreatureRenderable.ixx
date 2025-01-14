export module fab.CreatureRenderable;

import fab.CombatSquare;
import fab.CoreContent;
import fab.FUtil;
import fab.FWindow;
import fab.Hitbox;
import fab.OccupantObject;
import fab.UIInteractable;
import sdl.SDLBase; 
import sdl.SDLBatchRenderPass;
import sdl.SDLRunner;
import std;

namespace fab {
	export class CreatureRenderable : public UIInteractable {
	public:
		CreatureRenderable(FWindow& window, uptr<Hitbox> hb, const OccupantObject& creature): UIInteractable(window, move(hb), creature.getImageField()), creature(creature) {}

		const OccupantObject& creature;

		virtual void renderImpl(sdl::SDLBatchRenderPass& rp) override;
	};

	void CreatureRenderable::renderImpl(sdl::SDLBatchRenderPass& rp)
	{
		UIInteractable::renderImpl(rp);
	}
}