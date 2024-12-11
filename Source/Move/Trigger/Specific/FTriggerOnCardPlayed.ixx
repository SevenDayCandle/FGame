export module fab.FTriggerOnCardPlayed;

import fab.Card;
import fab.CombatInstance;
import fab.CombatSquare;
import fab.FMove;
import fab.FTrigger;
import fab.FUtil;
import fab.OnCardPlayedSubscription;
import fab.PileGroup;
import std;

namespace fab {
	export class FTriggerOnCardPlayed : public FTrigger, public OnCardPlayedSubscription {
	public:
		inline static auto DATA = DataD<FTriggerOnCardPlayed>();

		FTriggerOnCardPlayed(FMove& source): FTrigger(DATA, source) {}
		virtual ~FTriggerOnCardPlayed() = default;

		void onCardPlayed(Card& card, CombatSquare& target, OccupantObject* user, PileGroup* sourceGroup) final;
	};

	void FTriggerOnCardPlayed::onCardPlayed(Card& card, CombatSquare& target, OccupantObject* user, PileGroup* sourceGroup) {
		any payload = &card;
		source.use(instance, user, &target, &payload);
	}
}