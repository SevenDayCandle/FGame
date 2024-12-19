export module fab.FVariableConstant;

import fab.CombatInstance;
import fab.FGetter;
import fab.FieldObject;
import fab.FUtil;
import fab.GameObject;
import std;

namespace fab {
	export class FVariableConstant : public FGetter {
	public:
		static constexpr cstr FIELD_VALUE = "value";
		static constexpr cstr FIELD_VALUE_UPGRADE = "valueUpgrade";
		inline static auto DATA = DataD<FVariableConstant>();

		FVariableConstant(): FGetter(DATA) {}
		FVariableConstant(const Save& save) : FGetter(DATA, save) {}
		FVariableConstant(const FVariableConstant& other) : FGetter(other) {}
		virtual ~FVariableConstant() final = default;

		int value;
		int valueUpgrade;

		inline uptr<FVariable> clone() final { return make_unique<FVariableConstant>(*this); }

		int getResult(CombatInstance* instance, GameObject* source, FieldObject* target, any* payload, any* outputPayload) final;
		void serializeImpl(Save& save) const final;
	protected:
		virtual void loadFields(const Save& save) final;
	};

	int FVariableConstant::getResult(CombatInstance* instance, GameObject* source, FieldObject* target, any* payload, any* outputPayload) {
		return value; // TODO use upgrade
	}

	void FVariableConstant::serializeImpl(Save& save) const {
		save.fields = {
			{FIELD_VALUE , futil::toString(value)},
			{ FIELD_VALUE_UPGRADE , futil::toString(valueUpgrade) }
		};
	}

	void FVariableConstant::loadFields(const Save& save) {
		if (save.fields) {
			value = futil::fromString<int>(save.fields->at(FIELD_VALUE));
			valueUpgrade = futil::fromString<int>(save.fields->at(FIELD_VALUE_UPGRADE));
		}
	}
}