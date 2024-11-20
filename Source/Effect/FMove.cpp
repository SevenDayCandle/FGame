module;

import fab.CombatInstance;
import fab.FEffect;
import fab.FieldObject;
import fab.GameObject;

module fab.FMove;

namespace fab {
	void FMove::use(CombatInstance* instance, GameObject* source, FieldObject* target, any* payload) {
		runAction(source, target, payload)
			.addOnComplete([this, instance, source, target, value = getPayload(instance, source, target, payload)](CallbackAction& act) mutable {
				// TODO base payload off of ran action as well as previous payload; may require passing payload by value
				if (act.isSuccess()) {
					for (uptr<FEffect>& eff : children) {
						eff->use(instance, source, target, &value);
					}
				}
			});
	}
}
