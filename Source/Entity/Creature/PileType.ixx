export module fab.PileType;

import fab.FUtil;
import fab.KeyedItem;
import std;

namespace fab {
	export class PileType : public KeyedItem<PileType> {
	public:
		PileType(strv key) : KeyedItem<PileType>(key) {}
	};

	export namespace piletype {

		export const PileType DISCARD("DISCARD");
		export const PileType DRAW("DRAW");
		export const PileType EXPEND("EXPEND");
		export const PileType HAND("HAND");
	}
}