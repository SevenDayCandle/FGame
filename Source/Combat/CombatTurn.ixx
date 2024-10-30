export module fab.CombatTurn;

import fab.FUtil;
import fab.IDrawable;
import fab.TurnObject;
import std;

namespace fab {
	export class CombatTurn {
	public:
		CombatTurn(TurnObject& source, int actionValue): source(source), actionValue(actionValue) {}

		bool isDone = false;
		int actionValue;
		TurnObject& source;

		bool operator<(const CombatTurn& other) const { return actionValue < other.actionValue; }
		bool operator>(const CombatTurn& other) const { return actionValue > other.actionValue; }

		inline IDrawable& getPortrait() { return source.getImagePortrait(); }

		void end();
		bool run();
		bool start();
	};

	bool CombatTurn::start()
	{
		// TODO hooks
		return source.onTurnBegin();
	}

	void CombatTurn::end()
	{
		source.onTurnEnd();
		// TODO hooks
	}

	bool CombatTurn::run() {
		return source.onTurnRun() || isDone;
	}
}