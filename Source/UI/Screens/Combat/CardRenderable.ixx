export module fab.CardRenderable;

import fab.Card;
import fab.CoreContent;
import fab.FUtil;
import fab.FWindow;
import fab.Hitbox;
import fab.RelativeHitbox;
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
		CardRenderable(FWindow& window, uptr<Hitbox> hb, const Card& card) : UICallbackInteractable<CardRenderable>(window, move(hb), window.props.defaultPanel()), card(card) {}
		CardRenderable(FWindow& window, Hitbox& source, const Card& card, float offX = 0, float offY = 0) : UICallbackInteractable<CardRenderable>(window, make_unique<RelativeHitbox>(window, source, offX, offY, CARD_W, CARD_H), window.props.defaultPanel()), card(card) {}

		const Card& card;
	};
}