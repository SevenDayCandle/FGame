export module fab.GreedyBehavior;

import fab.Creature;
import fab.FUtil;
import fab.KeyedItem;
import std;

namespace fab {
	export class GreedyBehavior : public Creature::Behavior {
	public:
		GreedyBehavior(): Behavior(ID) {}

		static constexpr strv ID = "GREEDY";
		static const GreedyBehavior instance;

		void act(Creature& source) override;
	};

	const GreedyBehavior GreedyBehavior::instance = GreedyBehavior();

	void GreedyBehavior::act(Creature& source)
	{
		// TODO implement
	}
}