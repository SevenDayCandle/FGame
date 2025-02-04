export module fab.FTriggerOnCardPlayed;

import fab.Card;
import fab.CombatInstance;
import fab.CombatSquare;
import fab.FEffect;
import fab.FTrigger;
import fab.FUtil;
import fab.OccupantObject;
import fab.OnCardPlayedSubscription;
import fab.PileGroup;
import std;

namespace fab {
	export class FTriggerOnCardPlayed : public FTrigger, public OnCardPlayedSubscription {
	public:
		inline static auto DATA = DataD<FTriggerOnCardPlayed>();

		FTriggerOnCardPlayed(FEffect& source): FTrigger(DATA, source) {}
		virtual ~FTriggerOnCardPlayed() = default;

		void onCardPlayed(Card& card, CombatSquare& target, OccupantObject* user, PileGroup* sourceGroup) final;
	};

	void FTriggerOnCardPlayed::onCardPlayed(Card& card, CombatSquare& target, OccupantObject* user, PileGroup* sourceGroup) {
		any payload = &card;
		source.use(instance, user, &target, &payload);
	}
}