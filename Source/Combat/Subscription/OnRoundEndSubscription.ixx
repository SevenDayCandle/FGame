export module fab.OnRoundEndSubscription;

import fab.CombatInstance;
import fab.FUtil;
import std;

namespace fab {
	export class OnRoundEndSubscription : public CombatInstance::SubscriberD<OnRoundEndSubscription> {
	public:
		OnRoundEndSubscription() {}
		virtual ~OnRoundEndSubscription() = default;

		virtual void onRoundEnd() = 0;
	};
}