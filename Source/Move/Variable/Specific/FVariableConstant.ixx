export module fab.FVariableConstant;

import fab.CombatInstance;
import fab.FieldObject;
import fab.FUtil;
import fab.FVariable;
import fab.GameObject;
import std;

namespace fab {
	constexpr cstr FIELD_VALUE = "value";
	constexpr cstr FIELD_VALUE_UPGRADE = "valueUpgrade";

	export class FVariableConstant : public FVariable {
	public:
		inline static auto DATA = DataT<FVariableConstant>();

		FVariableConstant(): FVariable(DATA) {}
		FVariableConstant(const Save& save) : FVariable(DATA, save) {}
		FVariableConstant(const FVariable& other) : FVariable(other) {}
		virtual ~FVariableConstant() final = default;

		int value;
		int valueUpgrade; // TODO use this

		virtual any getValue(CombatInstance* instance, GameObject* source, FieldObject* target, void* payload) final;
		virtual strumap<str> serializeFields() const final;
	protected:
		virtual void loadFields(const Save& save) final;
	};

	any FVariableConstant::getValue(CombatInstance* instance, GameObject* source, FieldObject* target, void* payload) {
		return value;
	}

	strumap<str> FVariableConstant::serializeFields() const {
		strumap<str> fields;
		fields[FIELD_VALUE] = futil::toString(value);
		return fields;
	}

	void FVariableConstant::loadFields(const Save& save) {
		value = futil::fromString<int>(save.fields.at(FIELD_VALUE));
	}
}