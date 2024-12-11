module;

import fab.CombatInstance;
import fab.FEffect;
import fab.FieldObject;
import fab.FMove;
import fab.GameObject;

module fab.FAct;

namespace fab {
	void FAct::use(CombatInstance* instance, GameObject* source, FieldObject* target, any* payload) {
		// TODO throw or warn user if instance is null
		runAction(*instance, source, target, payload)
			.addOnComplete([this, instance, source, target](CallbackAction& act) mutable {
				any payload = getPayload(act, instance, source, target);
				if (act.isSuccess()) {
					for (uptr<FMove>& eff : children) {
						eff->use(instance, source, target, &payload);
					}
				}
			});
	}
}
