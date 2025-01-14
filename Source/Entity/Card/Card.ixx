export module fab.Card;

import fab.CardData;
import fab.CombatSquare;
import fab.FieldObject;
import fab.FPrimary;
import fab.FUtil;
import fab.GameObject;
import fab.ObjectRarity;
import fab.OccupantObject;
import fab.PileType;
import sdl.SDLBase; 
import sdl.SDLRunner;
import std;

namespace fab {
	export class Card : public GameObjectD<CardData> {
	public:
		Card(CardData& data, int upgrades = 0) : GameObjectD<CardData>(data), upgrades(upgrades) {}
		Card(const Card& other) : GameObjectD<CardData>(data), upgrades(other.upgrades),
			effectOverride(other.effectOverride ? futil::transform(other.effectOverride.value(), [](const uptr<FPrimary>& eff) {return make_unique<FPrimary>(*eff); }) : opt<vec<uptr<FPrimary>>>()) {}

		int upgrades;
		opt<vec<uptr<FPrimary>>> effectOverride;

		inline int cost() const { return data.getCost(upgrades); };
		inline int targetRangeBegin() const { return data.getTargetRangeBegin(upgrades); };
		inline int targetRangeEnd() const { return data.getTargetRangeEnd(upgrades); };
		inline int targetSizeX() const { return data.getTargetSizeX(upgrades); };
		inline int targetSizeY() const { return data.getTargetSizeY(upgrades); };
		inline ObjectRarity* rarity() const { return data.data.rarity; }
		inline vec<uptr<FPrimary>>& getEffects() { return effectOverride ? effectOverride.value() : data.data.effects; }

		bool canAffect(OccupantObject* source, CombatSquare& target);
		bool canAffectSquare(OccupantObject* source);
		bool isInRange(OccupantObject* source, CombatSquare& target);
		bool isPlayable();
		Card& setUpgrade(int upgrades);
		const PileType& getPileAfterUse();
	};
}