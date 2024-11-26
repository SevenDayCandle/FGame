
import fab.FEffect;
import fab.FieldObject;
import fab.FTrigger;
import fab.GameObject;

module fab.FPrimary;

namespace fab {
	FPrimary::Save FPrimary::serialize() const {
		Save save;
		for (const uptr<FTrigger>& trigger : triggers) {
			save.triggers.emplace_back(trigger->data.id);
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

	void FPrimary::subscribe(CombatInstance& instance) {
		for (const uptr<FTrigger>& trigger : triggers) {
			trigger->subscribe(instance);
		}
	}

	void FPrimary::unsubscribe() {
		for (const uptr<FTrigger>& trigger : triggers) {
			trigger->unsubscribe();
		}
	}

	void FPrimary::use(CombatInstance* instance, GameObject* source, FieldObject* target, any* payload) {
		for (uptr<FMove>& eff : children) {
			eff->use(instance, source, target, payload);
		}
	}
	void FPrimary::load(const Save& save) {
		for (const FEffect::Save& childSave : save.children) {
			uptr<FEffect> child = FEffect::create(childSave);
			if (child) {
				addChild(move(child));
			}
		}
		for (const str& id : save.triggers) {
			FTrigger::Data* data = FTrigger::Data::get(id);
			if (data) {
				triggers.push_back(data->create(*this));
			}
		}
	}
}