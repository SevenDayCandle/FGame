module;

import fab.FieldObject;
import fab.FFilter;
import fab.GameObject;
import fab.FVariable;

module fab.FEffect;

namespace fab {
	FEffect::Save FEffect::serialize() const {
		Save save = { data.id };

		if (vars.size() > 0) {
			save.vars = futil::transform(vars, [](const uptr<FVariable>& t) {return t->serialize(); });
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

	void FEffect::load(const Save& save) {
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
		if (save.vars) {
			for (const FVariable::Save& varSave : save.vars.value()) {
				uptr<FVariable> var = FVariable::create(varSave);
				if (var) {
					vars.push_back(move(var));
				}
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
