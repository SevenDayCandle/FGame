export module fab.GreedyBehavior;

import fab.Card;
import fab.CardUseAction;
import fab.CombatInstance;
import fab.CombatSquare;
import fab.Creature;
import fab.CreatureMoveAction;
import fab.FUtil;
import fab.GameRun;
import fab.KeyedItem;
import std;

/*
 * Creature behavior that uses the most expensive card possible on the closest target possible. If not in range to use a card, move towards the closest target.
 */
namespace fab {
	struct CardChoice {
		Card* card;
		OccupantObject* target;
		int distance;
	};

	export class GreedyBehavior : public Creature::Behavior {
	public:
		GreedyBehavior(): Behavior(ID) {}

		static constexpr strv ID = "GREEDY";
		static const GreedyBehavior instance;

		bool act(Creature& source) override;
	};

	const GreedyBehavior GreedyBehavior::instance = GreedyBehavior();

	// TODO cache creature/card pairs
	bool GreedyBehavior::act(Creature& source)
	{
		CombatInstance& instance = *GameRun::current->getCombatInstance();

		vec<pair<OccupantObject*, int>> creatures;
		for (OccupantObject* occupant : instance.getOccupants()) {
			if (occupant->currentSquare) {
				int dist = source.currentSquare ? source.currentSquare->getLineDistance(*occupant->currentSquare) : futil::INT_MAX;
				creatures.emplace_back(occupant, dist);
			}
		}
		std::ranges::sort(creatures, [&source](auto& obj1, auto& obj2) {
			return obj2.second - obj1.second;
		});

		vec<CardChoice> cards;
		for (uptr<Card>& ptr : source.pile.hand) {
			Card& card = *ptr.get();
			int halfSize = card.targetSizeX() / 2;
			int rangeBegin = card.targetRangeBegin() - halfSize;
			int rangeEnd = card.targetRangeEnd() + halfSize;
			for (pair<OccupantObject*, int>& creature : creatures) {
				bool validTarget = creature.first->faction == source.faction ? card.data.data.target.targetAlly : card.data.data.target.targetEnemy;
				bool inRange = creature.second >= rangeBegin && creature.second <= rangeEnd;
				bool usable = card.isPlayable();
				if (validTarget && inRange && usable) {
					cards.emplace_back(&card, creature.first, creature.second);
				}
			}
		}
		// TODO sort by target health
		std::ranges::sort(cards, [&source](CardChoice& obj1, CardChoice& obj2) {
			int diff = obj2.card->cost() - obj1.card->cost();
			if (diff != 0) {
				return diff;
			}
			diff = obj2.distance - obj1.distance;
			return diff;
		});


		if (cards.size() > 0) {
			CardChoice& choice = cards.back();
			instance.queueNew<CardUseAction>(*choice.card, *choice.target->currentSquare, &source.pile);
			cards.pop_back();
			return false;
		}
		else if (source.currentSquare && creatures.size() > 0 && source.getMovement() > 0) {
			pair<OccupantObject*, int>& closest = creatures.front();
			if (closest.second > 1) {
				instance.fillDistances(source.currentSquare);
				vec<CombatSquare*> path = instance.findShortestPath(closest.first->currentSquare);
				instance.queueAction(CreatureMoveAction::pathMove(instance, &source, path, true));
			}
			else {
				return true;
			}
			
			return false;
		}
		else {
			return true;
		}


		return true;
	}
}