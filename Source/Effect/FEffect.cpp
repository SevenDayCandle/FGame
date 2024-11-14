module;

import fab.FieldObject;
import fab.GameObject;
import fab.FVariable;

module fab.FEffect;

namespace fab {
	FEffect& FEffect::addChild(uptr<FEffect>&& child) {
		FEffect& ref = *child;
		children.push_back(move(child));
		ref.parent = this;
		if (owner) {
			ref.setOwner(owner);
		}
		return *this;
	}

	FEffect& FEffect::setOwner(GameObject* owner) {
		this->owner = owner;
		for (uptr<FEffect>& child : children) {
			child->setOwner(owner);
		}
		return *this;
	}

	any FEffect::getPayload(GameObject* source, FieldObject* target, any* payload) {
		if (vars.size() > 0) {
			return vars[0]->getValue(source, target, payload);
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
