export module fab.CardMoveAction;

import fab.Card;
import fab.CallbackAction;
import fab.CallbackVFX;
import fab.CombatInstance;
import fab.FUtil;
import fab.CardPile;
import fab.PileType;
import fab.UIScreen;
import fab.VFXAction;
import std;

// TODO add result handle to get detected
namespace fab {
	export class CardMoveAction : public VFXAction {
	public:
		struct Listener {
			virtual ~Listener() = default;

			virtual uptr<CallbackVFX> cardMoveVFX(const Card& detected, const CardPile& destPile, bool manual) = 0;
		};

		CardMoveAction(CombatInstance& instance, CardPile& sourcePile, CardPile& destPile, CardPile::iterator it, bool manual = true):
			VFXAction(instance), destPile(destPile), sourcePile(sourcePile), it(it), manual(manual) {}
		CardMoveAction(CombatInstance& instance, CardPile& sourcePile, CardPile& destPile, Card& card, bool manual = true) :
			VFXAction(instance), destPile(destPile), sourcePile(sourcePile), it(sourcePile.find(&card)), manual(manual) {}
		virtual ~CardMoveAction() = default;

		bool manual;

		inline CardMoveAction& setManual(bool val) { return manual = val, *this; }

		virtual bool isSuccess() override;
		virtual void start() override;
	protected:
		uptr<CallbackVFX> getVfx() override;
	private:
		Card* detected;
		CardPile& destPile;
		CardPile& sourcePile;
		CardPile::iterator it;
		// TODO allow specify position in dest_pile
	};

	bool CardMoveAction::isSuccess() {
		return detected;
	}

	void CardMoveAction::start() {
		if (it >= sourcePile.begin() && it < sourcePile.end() && it->get() && destPile.size() < destPile.maxSize) {
			Card& card = *it->get();
			destPile.push_back(move(*it));
			sourcePile.erase(it);
			detected = &card;
			// TODO hooks
		}
		VFXAction::start();
	}

	uptr<CallbackVFX> CardMoveAction::getVfx() {
		Listener* listener = dynamic_cast<Listener*>(instance.viewSubscriber);
		return listener && detected ? listener->cardMoveVFX(*detected, destPile, manual) : uptr<CallbackVFX>();
	}
}