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
	bool CombatSquare::passable(OccupantObject* source) {
		return true;
	}

	// Assign the given occupant to this square
	CombatSquare& CombatSquare::setOccupant(OccupantObject* occupant)
	{
		// Ensure that existing occupants of squares being passed through don't get overwritten
		if (this->occupant == nullptr || occupant == nullptr) {
			this->occupant = occupant;
		}
		if (occupant) {
			// Ensure that existing occupants of squares being passed through don't get written out
			if (occupant->currentSquare && occupant->currentSquare->getOccupant() == occupant) {
				occupant->currentSquare->setOccupant(nullptr);
			}
			occupant->currentSquare = this;
			occupant->onMoved();
		}
		// TODO hooks
		// TODO power triggers

		return *this;
	}
}