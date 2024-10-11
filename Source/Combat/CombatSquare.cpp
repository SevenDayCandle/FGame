module;

import fab.AttributeObject;
import fab.FieldObject;
import fab.FUtil;
import fab.UIBase;
import std;

module fab.CombatSquare;

namespace fab {

	// Whether the square can be moved through or not
	// TODO implement
	bool CombatSquare::passable()
	{
		return true;
	}

	// Assign the given occupant to this square
	CombatSquare& CombatSquare::setOccupant(OccupantObject* occupant)
	{
		this->occupant = occupant;
		if (occupant) {
			occupant->currentSquare = this;
			occupant->onMoved();
		}
		// TODO hooks
		// TODO power triggers

		return *this;
	}
}