import fab.FModifier;
import fab.FGetter;
import fab.FEffect;
import fab.FieldObject;
import fab.GameObject;
import fab.FVariable;
import fab.FUtil;

module fab.FVariable;

namespace fab {
	// Create a JSON representation of this variable
	FVariable::Save FVariable::serialize() const {
		Save save = { str(getId()) };
		serializeImpl(save);
		return save;
	}

	// Rebuild this variable from a JSON representation
	uptr<FVariable> FVariable::create(const Save& save) {
		if (FGetter::Data* ad = FGetter::Data::get(save.id)) {
			return ad->create(save);
		}
		if (FModifier::Data* cd = FModifier::Data::get(save.id)) {
			return cd->create(save);
		}
		return uptr<FVariable>();
	}
}