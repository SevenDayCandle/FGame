
import fab.FEffect;
import fab.FieldObject;
import fab.FTrigger;
import fab.GameObject;

module fab.FPrimary;

namespace fab {
	FPrimary::Save FPrimary::serialize() const {
		Save save;
		if (triggers.size() > 0) {
			save.triggers = futil::transform(triggers, [](const uptr<FTrigger>& t) {return t->data.id; });
		}
		if (filters.size() > 0) {
			save.filters = futil::transform(filters, [](const uptr<FFilter>& t) {return t->serialize(); });
		}

		vec<FEffect::Save> childrenToSave;
		for (const uptr<FMove>& child : children) {
			// TODO enforce that children are FEffect
			FEffect* eff = dynamic_cast<FEffect*>(child.get());
			if (eff) {
				childrenToSave.emplace_back(eff->serialize());
			}
		}
		if (childrenToSave.size() > 0) {
			save.children = move(childrenToSave);
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
		if (save.children) {
			for (const FEffect::Save& childSave : save.children.value()) {
				uptr<FEffect> child = FEffect::create(childSave);
				if (child) {
					addChild(move(child));
				}
			}
		}
		if (save.filters) {
			for (const FFilter::Save& filterSave : save.filters.value()) {
				uptr<FFilter> filter = FFilter::create(filterSave);
				if (filter) {
					filters.push_back(move(filter));
				}
			}
		}
		if (save.triggers) {
			for (const str& id : save.triggers.value()) {
				FTrigger::Data* data = FTrigger::Data::get(id);
				if (data) {
					triggers.push_back(data->create(*this));
				}
			}
		}
	}
}