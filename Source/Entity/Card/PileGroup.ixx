export module fab.PileGroup;

import fab.CardPile;
import fab.CombatSquare;
import fab.FieldObject;
import fab.FUtil;
import fab.OccupantObject;
import fab.PileType;
import std;

namespace fab {
	export class PileGroup {
	public:
		PileGroup(OccupantObject* source, int handSize = futil::INT_MAX): source(source),
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
		OccupantObject* source;

		CardPile& getPile(const PileType& type);
		PileGroup& setSource(OccupantObject* source);
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

	PileGroup& PileGroup::setSource(OccupantObject* source) {
		this->source = source;
		discardPile.source = source;
		drawPile.source = source;
		expendPile.source = source;
		hand.source = source;
		return *this;
	}
}