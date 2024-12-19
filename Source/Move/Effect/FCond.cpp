module;

import fab.FEffect;
import fab.FieldObject;
import fab.FEffect;
import fab.FVariable;
import fab.GameObject;

module fab.FCond;

namespace fab {
	void FCond::use(CombatInstance* instance, GameObject* source, FieldObject* target, any* payload) {
		any p;
		for (int index : getIndices(instance, source, target, payload, p)) {
			if (index < children.size()) {
				children[index]->use(instance, source, target, &p);
			}
		}
	}
}