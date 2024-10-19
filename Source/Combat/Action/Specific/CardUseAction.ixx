export module fab.CardUseAction;

import fab.Card;
import fab.CallbackAction;
import fab.CallbackVFX;
import fab.CombatSquare;
import fab.FUtil;
import fab.GameObject;
import fab.VFXAction;
import std;

namespace fab {
	export class CardUseAction : public VFXAction<CardUseAction> {
	public:
		CardUseAction(Card& card, vec<CombatSquare*>&& targets, const func<uptr<CallbackVFX>()>& vfxFunc) : VFXAction(vfxFunc), card(card), targets(move(targets)) {}
		virtual ~CardUseAction() = default;

		Card& card;
		vec<CombatSquare*> targets;

		virtual bool isSuccess() override;
		virtual void start() override;
	private:
		bool canUse = true;
	};

	bool CardUseAction::isSuccess() {
		return canUse;
	}

	void CardUseAction::start() {
		VFXAction::start();

		canUse = card.isPlayable();

		if (canUse) {
			for (CombatSquare* square : targets) {
				if (square) {
					// TODO activate effects
					// TODO hooks
				}
			}
		}
	}
}