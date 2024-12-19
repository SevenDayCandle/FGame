
import fab.FEffect;
import fab.FieldObject;
import fab.FTrigger;
import fab.GameObject;

module fab.FPrimary;

namespace fab {
	void FPrimary::serializeImpl(Save& save) const {
		if (triggers.size() > 0) {
			save.triggers = futil::transform(triggers, [](const uptr<FTrigger>& t) {return t->data.id; });
		}
		if (filters.size() > 0) {
			save.filters = filters.serialize();
		}
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
		for (uptr<FEffect>& eff : children) {
			eff->use(instance, source, target, payload);
		}
	}

	void FPrimary::loadImpl(const Save& save) {
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