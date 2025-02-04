export module fab.CardType;

import fab.FUtil;
import fab.KeyedItem;

namespace fab {
	export class CardType : public KeyedItem<CardType> {
	public:
		CardType(strv key) : KeyedItem<CardType>(key) {}
	};

	export namespace cardtype {
		export const CardType ATTACK("ATTACK");
		export const CardType DEFENSE("DEFENSE");
		export const CardType ENHANCE("ENHANCE");
		export const CardType IMPAIR("IMPAIR");
		export const CardType MOVE("MOVE");
		export const CardType NEGATIVE("NEGATIVE");
	}
}