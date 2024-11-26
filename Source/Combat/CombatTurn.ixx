export module fab.CombatTurn;

import fab.FUtil;
import fab.IDrawable;
import fab.TurnObject;
import std;

namespace fab {
	export class CombatInstance;

	export class CombatTurn {
	public:
		CombatTurn(CombatInstance& instance, TurnObject& source, int actionValue) : instance(instance), source(source), actionValue(actionValue) {}

		bool isDone = false;
		CombatInstance& instance;
		int actionValue;
		TurnObject& source;

		bool operator<(const CombatTurn& other) const { return actionValue < other.actionValue; }
		bool operator>(const CombatTurn& other) const { return actionValue > other.actionValue; }

		inline IDrawable& getPortrait() { return source.getImagePortrait(); }
	};
}