module;

import fab.ArbitraryAction;
import fab.Card;
import fab.CardData;
import fab.CardMoveAction;
import fab.CombatInstance;
import fab.CombatSquare;
import fab.DrawCardAction;
import fab.GameRun;
import fab.Passive;
import fab.PassiveData;
import fab.SequentialAction;

module fab.Creature;

namespace fab {
	// If a behavior is defined, the turn consists of its execution
	// Otherwise, the turn is dependent on user input
	bool Creature::onTurnBegin()
	{
		CombatInstance& instance = *GameRun::current->getCombatInstance();

		// Refresh energy, movement, etc. on this creature
		energy = std::min(energy + getEnergyGain(), energyMax);
		movement = movementMax;

		// Discard current hand
		SequentialAction& discardAction = instance.queueNew<SequentialAction>();
		for (auto it = pile.hand.end(); it != pile.hand.begin();) {
			--it;
			discardAction.add(make_unique<CardMoveAction>(instance, pile.hand, pile.discardPile, it))
				.setManual(false);
		}

		// Draw start of turn cards
		SequentialAction& drawAction = instance.queueNew<SequentialAction>();
		for (int i = 0; i < getCardDraw(); i++) {
			drawAction.add(make_unique<DrawCardAction>(instance, pile))
				.setManual(false);
		}


		// TODO start turn hooks
		// TODO status updates
		if (behavior) {
			behavior->act(*this);
			return true;
		}
		// TODO show stats and cards on screen
		return false;
	}

	// Reinsert a turn into queue based on current speed
	void Creature::onTurnEnd()
	{
		GameRun::current->getCombatInstance()->queueNew<ArbitraryAction>([this]() {queueTurn(); });
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
				pile.drawPile.push_back(make_unique<Card>(*data, listing.upgrades));
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

	void Creature::queueTurn()
	{
		int actionValue = DEFAULT_ROUND_LENGTH * 100 / (1 + getActionSpeed());
		GameRun::current->getCombatInstance()->queueTurn(*this, actionValue);
	}
}