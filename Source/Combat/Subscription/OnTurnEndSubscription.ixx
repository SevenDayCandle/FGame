export module fab.OnTurnEndSubscription;

import fab.CombatInstance;
import fab.CombatTurn;
import fab.FUtil;
import std;

namespace fab {
	export class OnTurnEndSubscription : public CombatInstance::SubscriberD<OnTurnEndSubscription> {
	public:
		OnTurnEndSubscription() {}
		virtual ~OnTurnEndSubscription() = default;

		virtual void onTurnEnd(CombatTurn& turn) = 0;
	};
}