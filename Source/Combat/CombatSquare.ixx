export module fab.CombatSquare;

import fab.AttributeObject;
import fab.FieldObject;
import fab.FUtil;
import fab.IDrawable;
import fab.TurnObject;
import std;

namespace fab {
	export class CombatSquare : public FieldObject {
	public:
		class OccupantObject : public FieldObject, public TurnObject {
		public:
			OccupantObject() {}
			OccupantObject(int faction) : TurnObject(faction) {}
			OccupantObject(OccupantObject&& other) = default;
			virtual ~OccupantObject() override = default;

			CombatSquare* currentSquare;

			virtual inline bool canMoveTo(CombatSquare* other, bool isDestination, bool isManual) { return !isDestination || !other || other->getOccupant() == nullptr; }
			virtual inline int getMovement() { return 0; }
			virtual inline void postInitialize() {}
			virtual inline void onMoved() {}

			virtual IDrawable& getImageField() const = 0;
		};

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

	export using OccupantObject = CombatSquare::OccupantObject;
}