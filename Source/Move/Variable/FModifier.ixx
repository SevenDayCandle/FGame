export module fab.FModifier;

import fab.CombatInstance;
import fab.FFilter;
import fab.FieldObject;
import fab.FUtil;
import fab.GameObject;
import fab.FVariable;
import std;

namespace fab {
	export class FModifier : public FVariable {
	public:
		FModifier(Data& data) : FVariable(data) {}
		FModifier(Data& data, const Save& save) : FVariable(data, save) {}
		FModifier(const FModifier& other) : FVariable(other) {}
		FModifier(FModifier&& other) noexcept = default;
		virtual ~FModifier() = default;

		virtual int getResult(CombatInstance* instance, GameObject* source, FieldObject* target, any* payload, any* outputPayload) override;
		virtual strumap<str> serializeFields() final;
	protected:
		vec<uptr<FVariable>> vars;

		virtual void loadFields(const Save& save) final;

		virtual int modifyResult(int res) = 0;
	};

	strumap<str> FModifier::serializeFields() {
		strumap<str> res;
		vec<Save> childVars = futil::transform(vars, [](const uptr<FVariable>& var) {return var->serialize(); });
		return res;
	}

	void FModifier::loadFields(const Save& save) {
	}
}