export module fab.TurnObject;

import fab.GameObject;
import fab.IDrawable;

/*
 * Objects that can be queued up for turns in combat
 */
namespace fab {
	export class CombatTurn;

	export class TurnObject : public virtual GameObject {
	public:
		TurnObject() {}
		TurnObject(int faction) : faction(faction) {}
		TurnObject(TurnObject&& other) = default;
		virtual ~TurnObject() = default;

		int faction;

		virtual inline int getActionSpeed() { return 0; }

		virtual IDrawable& getImagePortrait() const = 0;
		virtual bool onTurnBegin(CombatTurn& turn) = 0;
		virtual bool onTurnRun(CombatTurn& turn) = 0;
		virtual void onTurnEnd(CombatTurn& turn) = 0;
	};
}