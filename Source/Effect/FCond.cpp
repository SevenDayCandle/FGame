module;

import fab.FEffect;
import fab.FieldObject;
import fab.GameObject;

module fab.FCond;

namespace fab {
	void FCond::use(GameObject* source, FieldObject* target, any* payload) {
		// TODO change output of check based on vars
		any value = getPayload(source, target, payload);
		if (check(source, target, &value)) {
			for (uptr<FEffect>& eff : children) {
				eff->use(source, target, &value);
			}
		}
	}
}