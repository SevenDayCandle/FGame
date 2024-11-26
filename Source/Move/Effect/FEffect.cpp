module;

import fab.FieldObject;
import fab.GameObject;
import fab.FVariable;

module fab.FEffect;

namespace fab {
	FEffect::Save FEffect::serialize() const {
		Save save = { data.id };
		for (const uptr<FVariable>& var : vars) {
			save.vars.emplace_back(var->serialize());
		}
		for (const uptr<FMove>& child : children) {
			// TODO enforce that children are FEffect
			FEffect* eff = dynamic_cast<FEffect*>(child.get());
			if (eff) {
				save.children.emplace_back(eff->serialize());
			}
		}
		return save;
	}

	any FEffect::getPayload(CombatInstance* instance, GameObject* source, FieldObject* target, any* payload) {
		if (vars.size() > 0) {
			return vars[0]->getValue(instance, source, target, payload);
		}
		return any();
	}

	void FEffect::load(const Save& save) {
		for (const FEffect::Save& childSave : save.children) {
			uptr<FEffect> child = FEffect::create(childSave);
			if (child) {
				addChild(move(child));
			}
		}
		for (const FVariable::Save& varSave : save.vars) {
			uptr<FVariable> var = FVariable::create(varSave);
			if (var) {
				vars.push_back(move(var));
			}
		}
	}
}

/* STATICS */
namespace fab {
	uptr<FEffect> FEffect::create(const Save& save) {
		FEffect::Data* data = FEffect::Data::get(save.id);
		if (data) {
			return data->create(save);
		}
		return uptr<FEffect>();
	}
}
