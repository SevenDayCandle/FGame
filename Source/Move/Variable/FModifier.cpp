import fab.FUtil;
import std;

module fab.FModifier;

namespace fab {
	int FModifier::getResult(CombatInstance* instance, GameObject* source, FieldObject* target, any* payload, any* outputPayload) {
		vec<int> results = vars.getResults(instance, source, target, payload, outputPayload);
		return modifyResult(results);
	}

	void FModifier::serializeImpl(Save& save) const {
		save.vars = vars.serialize();
	}

	void FModifier::load(const Save& save) {
		if (save.vars) {
			vars.addSaves(save.vars.value());
		}
	}
}