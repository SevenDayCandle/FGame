export module fab.FActDrawCard;

import fab.CallbackAction;
import fab.Card;
import fab.CombatInstance;
import fab.Creature;
import fab.DrawCardAction;
import fab.FAct;
import fab.FEffect;
import fab.FieldObject;
import fab.FUtil;
import fab.GameObject;
import fab.SequentialAction;
import std;

namespace fab {
	export class FActDrawCard : public FAct {
	public:
		inline static auto DATA = FEffect::DataD<FActDrawCard>();

		FActDrawCard(): FAct(DATA) {}
		FActDrawCard(const FEffect::Save& save) : FAct(DATA, save) {}
		FActDrawCard(const FEffect& other) : FAct(other) {}
		FActDrawCard(FActDrawCard&& other) noexcept = default;
		virtual ~FActDrawCard() = default;

		any getPayload(CallbackAction& act, CombatInstance* instance, GameObject* source, FieldObject* target) final;
		CallbackAction& runAction(CombatInstance& instance, GameObject* source, FieldObject* target, any* payload) final;
	};

	any FActDrawCard::getPayload(CallbackAction& act, CombatInstance* instance, GameObject* source, FieldObject* target) {
		SequentialAction<DrawCardAction>& action = dynamic_cast<SequentialAction<DrawCardAction>&>(act);
		vec<Card*> cards;
		for (DrawCardAction* draw : action.getActions()) {
			Card* res = draw->getResult();
			if (res) {
				cards.push_back(res);
			}
		}
		return cards;
	}

	CallbackAction& FActDrawCard::runAction(CombatInstance& instance, GameObject* source, FieldObject* target, any* payload) {
		SequentialAction<DrawCardAction>& action = instance.queueNew<SequentialAction<DrawCardAction>>();
		Creature* cr = dynamic_cast<Creature*>(source);
		if (cr) {
			for (int i = 0; i < invokeVars(&instance, source, target, payload); ++i) {
				action.addNew<DrawCardAction>(cr->pile);
			}
		}

		return action;
	}
}