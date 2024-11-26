module;

import fab.FEffect;
import fab.FieldObject;
import fab.FMove;
import fab.GameObject;

module fab.FCond;

namespace fab {
	void FCond::use(CombatInstance* instance, GameObject* source, FieldObject* target, any* payload) {
		// TODO change output of check based on vars
		any value = getPayload(instance, source, target, payload);
		if (check(source, target, &value)) {
			for (uptr<FMove>& eff : children) {
				eff->use(instance, source, target, &value);
			}
		}
	}
}