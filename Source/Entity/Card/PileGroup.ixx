export module fab.PileGroup;

import fab.CardPile;
import fab.FieldObject;
import fab.FUtil;
import fab.PileType;
import std;

namespace fab {
	export class PileGroup {
	public:
		PileGroup(FieldObject* source, int handSize = futil::INT_MAX): source(source),
			discardPile(CardPile(piletype::DISCARD, source)),
			drawPile(CardPile(piletype::DRAW, source)),
			expendPile(CardPile(piletype::EXPEND, source)),
			hand(CardPile(piletype::HAND, source, handSize))
		{}
		virtual ~PileGroup() = default;

		CardPile discardPile;
		CardPile drawPile;
		CardPile expendPile;
		CardPile hand;
		FieldObject* source;

		CardPile& getPile(const PileType& type);
		PileGroup& setSource(FieldObject* source);	
	};

	// Get the actual card list associated with this pile type
	CardPile& PileGroup::getPile(const PileType& type) {
		if (type == piletype::DISCARD) {
			return discardPile;
		}
		if (type == piletype::DRAW) {
			return drawPile;
		}
		if (type == piletype::EXPEND) {
			return expendPile;
		}
		return hand;
		// TODO support custom pile types
	}

	PileGroup& PileGroup::setSource(FieldObject* source) {
		this->source = source;
		discardPile.source = source;
		drawPile.source = source;
		expendPile.source = source;
		hand.source = source;
		return *this;
	}
}