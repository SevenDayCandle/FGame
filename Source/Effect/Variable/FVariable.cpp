module;

import fab.FieldObject;
import fab.GameObject;

module fab.FVariable;

namespace fab {
	uptr<FVariable> FVariable::create(const Save& save) {
		FVariable::Data* data = FVariable::Data::get(save.id);
		if (data) {
			return data->create(save);
		}
		return uptr<FVariable>();
	}
}