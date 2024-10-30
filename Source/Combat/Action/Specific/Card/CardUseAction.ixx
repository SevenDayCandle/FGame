export module fab.CardUseAction;

import fab.Card;
import fab.CardPile;
import fab.CallbackVFX;
import fab.CardMoveAction;
import fab.CombatInstance;
import fab.CombatSquare;
import fab.FieldObject;
import fab.FUtil;
import fab.PileGroup;
import fab.PileType;
import fab.UIScreen;
import fab.VFXAction;
import std;

namespace fab {
	export class CardUseAction : public VFXAction<CardUseAction> {
	public:
		CardUseAction(CombatInstance& instance, Card& card, const CombatSquare& target, FieldObject* source = nullptr, PileGroup* sourceGroup = nullptr) :
			VFXAction(instance), card(card), target(target), source(source), sourceGroup(sourceGroup) {}
		CardUseAction(CombatInstance& instance, Card& card, const CombatSquare& target, PileGroup* sourceGroup): CardUseAction(instance, card, target, sourceGroup->source, sourceGroup) {}
		virtual ~CardUseAction() = default;

		Card& card;
		const CombatSquare& target;
		FieldObject* source;
		PileGroup* sourceGroup;

		inline bool isLowPriority() final { return true; }
		inline bool isSuccess() override { return canUse; }

		virtual void complete() override;
		virtual void start() override;
	protected:
		uptr<CallbackVFX> getVfx() override;
	private:
		bool canUse = true;
	};

	void CardUseAction::complete() {
		VFXAction::complete();

		// If this card was played from the hand or draw pile, move it to discard pile or expend pile (or wherever else it is supposed to go)
		if (canUse && sourceGroup) {
			CardPile& pile = sourceGroup->getPile(card.getPileAfterUse());
			auto it = std::ranges::find_if(sourceGroup->hand, [this](const uptr<Card>& ptr) {
				return ptr.get() == &card;
			});
			if (it != sourceGroup->hand.end()) {
				instance.queueNewFront<CardMoveAction>(sourceGroup->hand, pile, it);
			}
			else {
				it = std::ranges::find_if(sourceGroup->drawPile, [this](const uptr<Card>& ptr) {
					return ptr.get() == &card;
				});
				if (it != sourceGroup->drawPile.end()) {
					instance.queueNewFront<CardMoveAction>(sourceGroup->drawPile, pile, it);
				}
			}
		}
	}

	void CardUseAction::start() {
		VFXAction::start();

		canUse = card.isPlayable();

		if (canUse) {
			int minCol = target.col - card.targetSizeX() / 2;
			int minRow = target.row - card.targetSizeY() / 2;
			int maxCol = minCol + card.targetSizeX();
			int maxRow = minRow + card.targetSizeY();
			for (int i = minCol; i < maxCol; i++) {
				for (int j = minRow; j < maxRow; j++) {
					CombatSquare* square = instance.getSquare(i, j);
					if (square) {
						// TODO activate effects
						// TODO hooks
					}
				}
			}
		}
	}

	uptr<CallbackVFX> CardUseAction::getVfx() {
		return instance.viewSubscriber ? instance.viewSubscriber->cardUseVFX(card, target) : uptr<CallbackVFX>();
	}
}