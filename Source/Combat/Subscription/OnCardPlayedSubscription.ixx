export module fab.OnCardPlayedSubscription;

import fab.Card;
import fab.CombatInstance;
import fab.CombatSquare;
import fab.FUtil;
import fab.OccupantObject;
import fab.PileGroup;
import std;

namespace fab {
	export class OnCardPlayedSubscription : public CombatInstance::SubscriberD<OnCardPlayedSubscription> {
	public:
		OnCardPlayedSubscription() {}
		virtual ~OnCardPlayedSubscription() = default;

		virtual void onCardPlayed(Card& card, CombatSquare& target, OccupantObject* source, PileGroup* sourceGroup) = 0;
	};
}