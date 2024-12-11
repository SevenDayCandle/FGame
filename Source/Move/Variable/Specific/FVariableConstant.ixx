export module fab.FVariableConstant;

import fab.CombatInstance;
import fab.FieldObject;
import fab.FUtil;
import fab.FVariable;
import fab.GameObject;
import std;

namespace fab {
	export class FVariableConstant : public FVariable {
	public:
		static constexpr cstr FIELD_VALUE = "value";
		static constexpr cstr FIELD_VALUE_UPGRADE = "valueUpgrade";
		inline static auto DATA = DataD<FVariableConstant>();

		FVariableConstant(): FVariable(DATA) {}
		FVariableConstant(const Save& save) : FVariable(DATA, save) {}
		FVariableConstant(const FVariable& other) : FVariable(other) {}
		virtual ~FVariableConstant() final = default;

		int value;
		int valueUpgrade;

		virtual int getResult(CombatInstance* instance, GameObject* source, FieldObject* target, any* payload, any* outputPayload) override;
		virtual opt<strumap<str>> serializeFields() const final;
	protected:
		virtual void loadFields(const Save& save) final;
	};

	int FVariableConstant::getResult(CombatInstance* instance, GameObject* source, FieldObject* target, any* payload, any* outputPayload) {
		return value; // TODO use upgrade
	}

	opt<strumap<str>> FVariableConstant::serializeFields() const {
		strumap<str> fields;
		fields[FIELD_VALUE] = futil::toString(value);
		fields[FIELD_VALUE_UPGRADE] = futil::toString(valueUpgrade);
		return fields;
	}

	void FVariableConstant::loadFields(const Save& save) {
		if (save.fields) {
			value = futil::fromString<int>(save.fields->at(FIELD_VALUE));
			valueUpgrade = futil::fromString<int>(save.fields->at(FIELD_VALUE_UPGRADE));
		}
	}
}