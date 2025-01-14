export module fab.OccupantObject;

import fab.CombatSquare;
import fab.FieldObject;
import fab.FUtil;
import fab.IDrawable;
import fab.TurnObject;
import std;

namespace fab {
	export class OccupantObject : public FieldObject, public TurnObject {
	public:
		OccupantObject() {}
		OccupantObject(int faction) : TurnObject(faction) {}
		OccupantObject(OccupantObject&& other) = default;
		virtual ~OccupantObject() override = default;

		CombatSquare* currentSquare;

		virtual inline bool canDamage() { return getHealth() > 0; }
		virtual inline bool canMoveTo(CombatSquare* other, bool isDestination, bool isManual) { return !isDestination || !other || other->getOccupant() == nullptr; }
		virtual inline int addHealth(int amount) { return -1; }
		virtual inline int getHealth() { return -1; }
		virtual inline int getMovement() { return 0; }
		virtual inline void postInitialize() {}
		virtual inline void onMoved() {}

		virtual IDrawable& getImageField() const = 0;
	};
}