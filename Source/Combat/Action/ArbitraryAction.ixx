export module fab.ArbitraryAction;

import fab.Action;
import fab.FUtil;
import std;

namespace fab {
	export class ArbitraryAction : public Action {
	public:
		ArbitraryAction(const func<void()>& onStart): onStart(onStart) {}
		ArbitraryAction(func<void()>&& onStart) : onStart(move(onStart)) {}
		virtual ~ArbitraryAction() = default;

		func<void()> onStart;

		inline virtual void start() override { onStart(); }
	};
}