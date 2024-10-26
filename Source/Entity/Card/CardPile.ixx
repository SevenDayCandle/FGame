export module fab.CardPile;

import fab.Card;
import fab.FieldObject;
import fab.FUtil;
import fab.PileType;
import std;

namespace fab {
	export class CardPile : public vec<uptr<Card>> {
	public:
		CardPile(const PileType& type, FieldObject* source = nullptr, int maxSize = futil::INT_MAX) : vec<uptr<Card>>(), type(type), source(source), maxSize(maxSize) {}

		const PileType& type;
		FieldObject* source;
		int maxSize;

		CardPile::iterator find(Card* card);
		int findPos(const Card* card);
	};

	CardPile::iterator CardPile::find(Card* card) {
		return std::ranges::find_if(*this, [&card](const uptr<Card>& ptr) {return ptr.get() == card; });
	}

	int CardPile::findPos(const Card* card) {
		int i = 0;
		for (uptr<Card>& entry : *this) {
			if (entry.get() == card) {
				return i;
			}
			++i;
		}
		return -1;
	}
}