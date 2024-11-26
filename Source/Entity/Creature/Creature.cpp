module;

import fab.ArbitraryAction;
import fab.Card;
import fab.CardData;
import fab.CardMoveAction;
import fab.CombatInstance;
import fab.CombatSquare;
import fab.CombatTurn;
import fab.DrawCardAction;
import fab.GameRun;
import fab.Passive;
import fab.PassiveData;
import fab.SequentialAction;

module fab.Creature;

namespace fab {

	bool Creature::onTurnBegin(CombatTurn& turn)
	{
		// Refresh energy, movement, etc. on this creature
		energy = std::min(energy + getEnergyGain(), energyMax);
		movement = movementMax;

		// Discard current hand
		SequentialAction<>& discardAction = turn.instance.queueNew<SequentialAction<>>();
		for (auto it = pile.hand.end(); it != pile.hand.begin();) {
			--it;
			discardAction.addNew<CardMoveAction>(pile.hand, pile.discardPile, it)
				.setManual(false);
		}

		// Draw start of turn cards
		SequentialAction<>& drawAction = turn.instance.queueNew<SequentialAction<>>();
		for (int i = 0; i < getCardDraw(); i++) {
			drawAction.addNew<DrawCardAction>(pile)
				.setManual(false);
		}


		// TODO start turn hooks
		// TODO status updates

		return behavior == nullptr;
	}

	// If a behavior is defined, the turn consists of its execution
	// Otherwise, the turn is dependent on user input
	bool Creature::onTurnRun(CombatTurn& turn) {
		return behavior && behavior->act(*this);
	}

	// Reinsert a turn into queue based on current speed
	void Creature::onTurnEnd(CombatTurn& turn)
	{
		// TODO end turn hooks
		// TODO status updates
	}

	// Attempt to move to the designated square. Return true if successful
	bool Creature::canMoveTo(CombatSquare* other, bool isDestination, bool isManual)
	{
		if (!OccupantObject::canMoveTo(other, isDestination, isManual)) {
			return false;
		}

		if (isManual) {
			// TODO hooks for movement amount
			int cost = 1;
			if (movement < cost) {
				return false;
			}
			movement -= cost;
		}

		return true;
	}

	// For field images, use the field override if it exists, otherwise fall back to the data
	IDrawable& Creature::getImageField() const
	{
		if (imageFieldOverride) {
			return *imageFieldOverride;
		}
		return data.getImageField();
	}

	// For portrait images, use theportrait override if it exists, otherwise fall back to the data
	IDrawable& Creature::getImagePortrait() const
	{
		if (imagePortraitOverride) {
			return *imagePortraitOverride;
		}
		return data.getImagePortrait();
	}

	// A value that determines how "quickly" this creature will onTurnBegin. The higher the value, the higher in the action queue this creature's turns will be placed
	int Creature::getActionSpeed()
	{
		int base = data.getResultActSpeed(upgrades);
		// TODO subscribers
		// TODO statuses
		return base;
	}

	// How many cards this creature draws at the start of its turn
	int Creature::getCardDraw()
	{
		int base = data.getResultHandDraw(upgrades);
		// TODO subscribers
		// TODO statuses
		return base;
	}

	// How much energy this creature gains at the start of its turn
	int Creature::getEnergyGain()
	{
		int base = data.getResultEnergyGain(upgrades);
		// TODO subscribers
		// TODO statuses
		return base;
	}

	// How far this creature can move this turn
	int Creature::getMovement() {
		// TODO subscribers
		// TODO statuses
		return movement;
	}

	// Set up cards and passives for this creature
	void Creature::initialize(vec<ItemListing>& setupCards, vec<ItemListing>& setupPassives)
	{
		for (ItemListing& listing : setupCards) {
			CardData* data = CardData::get(listing.id);
			if (data) {
				for (int i = 0; i < listing.count; ++i) {
					pile.drawPile.push_back(make_unique<Card>(*data, listing.upgrades));
				}
			}
		}
		GameRun::current->rngCard.shuffle(pile.drawPile);

		// TODO passives
		for (ItemListing& listing : setupPassives) {
			PassiveData* data = PassiveData::get(listing.id);
			if (data) {
				uptr<Passive> pa = make_unique<Passive>(*data, listing.upgrades);
				passives.push_back(move(pa));
			}
		}
	}
}