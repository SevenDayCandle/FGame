export module fab.CardDataRenderable;

import fab.Card;
import fab.CardData;
import fab.CardRenderable;
import fab.FUtil;
import fab.FWindow;
import fab.Hitbox;
import fab.RelativeHitbox;
import std;

namespace fab {
	export class CardDataRenderable : public CardRenderable {
	public:
		CardDataRenderable(FWindow& window, uptr<Hitbox> hb, CardData& data) : cardInstance(data), CardRenderable(window, move(hb), cardInstance) {}
		CardDataRenderable(FWindow& window, Hitbox& source, CardData& data, float offX = 0, float offY = 0) : cardInstance(data), CardRenderable(window, make_unique<RelativeHitbox>(window, source, offX, offY, CARD_W, CARD_H), data) {}
		virtual ~CardDataRenderable() = default;
	private:
		Card cardInstance;
	};
}