module;

import fab.CombatSquare;
import fab.GameRun;
import fab.PileType;

module fab.Card;

namespace fab {

	// Whether this card can be played
	bool Card::isPlayable()
	{
		// TODO
		return true;
	}

	// Set the upgrade level of the card and refresh values
	Card& Card::setUpgrade(int upgrades)
	{
		this->upgrades = upgrades;
		// TODO refresh values
		return *this;
	}

	// Get the pile that the card should be moved to once used
	const PileType& Card::getPileAfterUse()
	{
		// TODO expend check
		// TODO card modifier checks
		return piletype::DISCARD;
	}
}