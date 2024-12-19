export module fab.DrawCardAction;

import fab.Card;
import fab.CardMoveAction;
import fab.CardPile;
import fab.CallbackVFX;
import fab.CombatInstance;
import fab.FUtil;
import fab.GameRun;
import fab.PileGroup;
import fab.PileReshuffleAction;
import fab.PileType;
import fab.UIScreen;
import std;

// TODO add result handle to get detected
namespace fab {
	export class DrawCardAction : public PileReshuffleAction {
	public:
		DrawCardAction(CombatInstance& instance, PileGroup& group, bool manual = true): PileReshuffleAction(instance, group), group(group), manual(manual) {}
		virtual ~DrawCardAction() = default;

		bool manual;

		inline bool isSuccess() override { return detected; }
		inline Card* getResult() const { return detected; }
		inline DrawCardAction& setManual(bool val) { return manual = val, *this; }

		virtual void start() override;
	protected:
		PileGroup& group;

		uptr<CallbackVFX> getVfx() override;
	private:
		Card* detected;
	};

	void DrawCardAction::start() {
		if (group.drawPile.empty() && !group.discardPile.empty()) {
			PileReshuffleAction::start();
		}
		auto it = group.drawPile.begin();
		if (it != group.drawPile.end()) {
			Card& card = *it->get();
			if (group.hand.add(it)) {
				group.drawPile.erase(it);
				detected = &card;
			}
			// Only manual draws can cause drawn cards to go to the discard pile
			else if (manual && group.discardPile.add(it)) {
				group.drawPile.erase(it);
				// TODO catch if the draw went to discard pile
			}

			// TODO hooks
		}
		VFXAction::start();
	}

	uptr<CallbackVFX> DrawCardAction::getVfx() {
		CardMoveAction::Listener* listener = dynamic_cast<CardMoveAction::Listener*>(instance.viewSubscriber);
		return listener && detected ? listener->cardMoveVFX(*detected, group.hand, manual) : uptr<CallbackVFX>();
	}
}