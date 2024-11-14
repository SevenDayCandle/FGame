export module fab.PileReshuffleAction;

import fab.Card;
import fab.CardPile;
import fab.CallbackAction;
import fab.CallbackVFX;
import fab.CombatInstance;
import fab.FUtil;
import fab.GameRun;
import fab.PileGroup;
import fab.PileType;
import fab.UIScreen;
import fab.VFXAction;
import std;

namespace fab {
	export class PileReshuffleAction : public VFXAction {
	public:
		PileReshuffleAction(CombatInstance& instance, CardPile& sourcePile, CardPile& destPile): VFXAction(instance), destPile(destPile), sourcePile(sourcePile) {}
		PileReshuffleAction(CombatInstance& instance, PileGroup& group) : PileReshuffleAction(instance, group.discardPile, group.drawPile) {}
		virtual ~PileReshuffleAction() = default;

		virtual void start() override;
	protected:
		CardPile& destPile;
		CardPile& sourcePile;

		uptr<CallbackVFX> getVfx() override;
	};

	void PileReshuffleAction::start() {
		auto it = sourcePile.begin();
		while (it != sourcePile.end()) {
			destPile.push_back(move(*it));
			sourcePile.erase(it);
			it = sourcePile.begin();
		}
		GameRun::current->rngCard.shuffle(destPile);
		// TODO visuals
		// TODO hooks
	}

	uptr<CallbackVFX> PileReshuffleAction::getVfx() {
		return instance.viewSubscriber ? instance.viewSubscriber->shuffleVFX(sourcePile.type, destPile.type) : uptr<CallbackVFX>();
	}
}