export module fab.CardRenderable;

import fab.Card;
import fab.CoreContent;
import fab.FUtil;
import fab.FWindow;
import fab.Hitbox;
import fab.RelativeHitbox;
import fab.TextDrawable;
import fab.UICallbackInteractable;
import sdl.SDLBase; 
import sdl.SDLBatchRenderPass;
import sdl.SDLRunner;
import std;

namespace fab {
	export constexpr float CARD_H = 600;
	export constexpr float CARD_W = 400;

	export class CardRenderable : public UICallbackInteractable<CardRenderable> {
	public:
		CardRenderable(FWindow& window, uptr<Hitbox> hb, const Card& card) : UICallbackInteractable<CardRenderable>(window, move(hb), window.props.defaultPanel()),
			card(card), nameText(window.props.fontRegular(), card.name()) {}
		CardRenderable(FWindow& window, Hitbox& source, const Card& card, float offX = 0, float offY = 0) : CardRenderable(window, make_unique<RelativeHitbox>(window, source, offX, offY, CARD_W, CARD_H), card) {}

		const Card& card;

		virtual void renderImpl(sdl::SDLBatchRenderPass& rp) override;
	private:
		TextDrawable nameText;
	};

	void CardRenderable::renderImpl(sdl::SDLBatchRenderPass& rp) {
		UIInteractable::renderImpl(rp);
		// TODO make this look better
		nameText.drawCentered(rp, hb->cX(), hb->y + hb->h * 0.1, win.getW(), win.getH());
	}
}