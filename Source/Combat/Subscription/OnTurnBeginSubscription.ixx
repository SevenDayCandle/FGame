export module fab.OnTurnBeginSubscription;

import fab.CombatInstance;
import fab.CombatTurn;
import fab.FUtil;
import std;

namespace fab {
	export class OnTurnBeginSubscription : public CombatInstance::SubscriberD<OnTurnBeginSubscription> {
	public:
		OnTurnBeginSubscription() {}
		virtual ~OnTurnBeginSubscription() = default;

		virtual void onTurnBegin(CombatTurn& turn) = 0;
	};
}