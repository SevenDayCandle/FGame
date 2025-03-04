export module fab.Creature;

import fab.AttributeObject;
import fab.Card;
import fab.CombatSquare;
import fab.CombatTurn;
import fab.CreatureData;
import fab.FUtil;
import fab.GameObject;
import fab.KeyedItem;
import fab.IDrawable;
import fab.ItemListing;
import fab.OccupantObject;
import fab.PileGroup;
import fab.PileType;
import sdl.SDLBase; 
import sdl.SDLBatchRenderPass;
import sdl.SDLRunner;
import std;

namespace fab {
	export class Creature : public GameObjectD<CreatureData>, public OccupantObject {
	public:
		class Behavior : public KeyedItem<Behavior> {
		public:
			Behavior(strv key) : KeyedItem<Behavior>(key) {}

			virtual bool act(Creature& source) = 0;
		};

		Creature(CreatureData& data, Behavior* behavior, int faction, int upgrades, int startingHealth) :
			GameObjectD(data), OccupantObject(faction, startingHealth, data.getResultHealth(upgrades)), behavior(behavior), upgrades(upgrades), energyMax(data.getResultEnergyMax(upgrades)), pile(this, data.getResultHandSize(upgrades)), movementMax(data.getResultMovement(upgrades)) {
		}
		Creature(CreatureData& data, Behavior* behavior, int faction, int upgrades) : Creature(data, behavior, faction, upgrades, data.getResultHealth(upgrades)) {}

		Behavior* behavior;
		IDrawable* imageFieldOverride;
		IDrawable* imagePortraitOverride;
		int energy;
		int energyMax;
		int movement;
		int movementMax;
		int upgrades;
		PileGroup pile;
		vec<uptr<AttributeObject>> passives;

		virtual bool onTurnBegin(CombatTurn& turn) override;
		virtual bool onTurnRun(CombatTurn& turn) override;
		virtual void onTurnEnd(CombatTurn& turn) override;
		bool canMoveTo(CombatSquare* other, bool isDestination, bool isManual) override;
		IDrawable& getImageField() const final override;
		IDrawable& getImagePortrait() const final override;
		int getActionSpeed() override;
		int getCardDraw();
		int getEnergyGain();
		int getMovement() override;
		void initialize(vec<ItemListing>& cards, vec<ItemListing>& passives);
	};
}