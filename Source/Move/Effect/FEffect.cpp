import fab.FAct;
import fab.FCond;
import fab.FEffect;
import fab.FieldObject;
import fab.FPrimary;
import fab.FVariable;
import fab.FUtil;
import fab.GameObject;
import std;

module fab.FEffect;

namespace fab {
	// Add a new child effect to this effect, assigning it this effect's owner
	FEffect& FEffect::addChild(uptr<FEffect>&& child) {
		FEffect& ref = *child;
		if (ref.setParent(this)) {
			children.push_back(move(child));
			if (owner) {
				ref.setOwner(owner);
			}
		}
		return *this;
	}

	// Set the owner for this effect as well as all child effects
	FEffect& FEffect::setOwner(GameObject* owner) {
		this->owner = owner;
		for (uptr<FEffect>& child : children) {
			child->setOwner(owner);
		}
		return *this;
	}

	// Generate a JSON-serializable representation of this effect
	FEffect::Save FEffect::serialize() const {
		Save save = { str(getId()) };

		serializeImpl(save);

		vec<Save> childrenToSave;
		for (const uptr<FEffect>& child : children) {
			childrenToSave.emplace_back(child->serialize());
		}
		if (childrenToSave.size() > 0) {
			save.children = move(childrenToSave);
		}

		return save;
	}

	// Create the proper type of effect based on the id from the save
	uptr<FEffect> FEffect::create(const Save& save) {
		if (save.id == FPrimary::ID) {
			return make_unique<FPrimary>(save);
		}
		if (FAct::Data* ad = FAct::Data::get(save.id)) {
			return ad->create(save);
		}
		if (FCond::Data* cd = FCond::Data::get(save.id)) {
			return cd->create(save);
		}
		return uptr<FEffect>();
	}

	// Rebuild this effect from a JSON representation
	void FEffect::load(const Save& save) {
		loadImpl(save);
		if (save.children) {
			for (const Save& childSave : save.children.value()) {
				if (uptr<FEffect> child = FEffect::create(childSave)) {
					addChild(move(child));
				}
			}
		}
	}
}