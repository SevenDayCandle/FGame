module;

import fab.CombatInstance;
import fab.FEffect;
import fab.FieldObject;
import fab.FEffect;
import fab.GameObject;

module fab.FAct;

namespace fab {
	void FAct::serializeImpl(Save& save) const {
		if (vars.size() > 0) {
			save.vars = vars.serialize();
		}
		if (filters.size() > 0) {
			save.filters = filters.serialize();
		}
	}

	void FAct::use(CombatInstance* instance, GameObject* source, FieldObject* target, any* payload) {
		if (instance) {
			runAction(*instance, source, target, payload)
				.addOnComplete([this, instance, source, target](CallbackAction& act) mutable {
				any payload = getPayload(act, instance, source, target);
				if (act.isSuccess()) {
					for (uptr<FEffect>& eff : children) {
						eff->use(instance, source, target, &payload);
					}
				}
			});
		}
	}

	void FAct::loadImpl(const Save& save) {
		if (save.filters) {
			filters.addSaves(save.filters.value());
		}
		if (save.vars) {
			vars.addSaves(save.vars.value());
		}
	}
}
