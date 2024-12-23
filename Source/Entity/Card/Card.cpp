module;

import fab.CombatSquare;
import fab.FieldObject;
import fab.GameRun;
import fab.PileType;

module fab.Card;

namespace fab {
	// Whether this card can affect the given square
	bool Card::canAffect(OccupantObject* source, CombatSquare& target) {
		if (data.data.target.targetSquare) {
			return true;
		}
		OccupantObject* occupant = target.getOccupant();
		if (occupant) {
			if (!source) {
				return true;
			}
			return occupant->faction == source->faction ? data.data.target.targetAlly : data.data.target.targetEnemy;
		}
		return false;
	}

	// TODO Whether this card can affect the square that the source is on
	bool Card::canAffectSquare(OccupantObject* source) {
		return false;
	}

	// Whether the target is in the range of this card
	bool Card::isInRange(OccupantObject* source, CombatSquare& target) {
		if (!source) {
			return true;
		}
		if (!source->currentSquare) {
			return false;
		}

		int dist = target.getLineDistance(*source->currentSquare);
		int halfSize = targetSizeX() / 2;
		return dist < targetRangeEnd() + halfSize && dist > targetRangeBegin() - halfSize;
	}

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