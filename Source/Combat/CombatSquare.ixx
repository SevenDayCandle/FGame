export module fab.CombatSquare;

import fab.AttributeObject;
import fab.FieldObject;
import fab.FUtil;
import fab.IDrawable;
import fab.TurnObject;
import std;

namespace fab {
	export class OccupantObject;

	export class CombatSquare : public FieldObject {
	public:
		CombatSquare(int col, int row): col(col), row(row) {}

		int col;
		int row;
		int sDist;

		inline int getLineDistance(const CombatSquare& other) const { return getLineDistance(other.col, other.row); }
		inline int getLineDistance(int tCol, int tRow) const { return std::max(std::abs(tCol - col), std::abs(tRow - row)); }
		inline OccupantObject* getOccupant() const { return occupant; }

		bool passable(OccupantObject* source);
		CombatSquare& setOccupant(OccupantObject* occupant);
		CombatSquare& setOccupantForce(OccupantObject* occupant);
	private:
		OccupantObject* occupant = nullptr;
	};
}