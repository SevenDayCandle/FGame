export module fab.FAct;

import fab.CallbackAction;
import fab.CombatInstance;
import fab.FEffect;
import fab.FFilter;
import fab.FieldObject;
import fab.GameObject;
import fab.FUtil;
import std;

namespace fab {
	export class FAct : public FEffect {
	public:
		FAct(FEffect::Data & data) : FEffect(data) {}
		FAct(Data& data, const Save& save) : FEffect(data, save) {}
		FAct(const FEffect& other) : FEffect(other) {}
		FAct(FAct&& other) noexcept = default;
		virtual ~FAct() = default;

		void use(CombatInstance* instance, GameObject* source, FieldObject* target, any* payload) final override;

		virtual any getPayload(CallbackAction& act, CombatInstance* instance, GameObject* source, FieldObject* target) = 0;
		virtual CallbackAction& runAction(CombatInstance& instance, GameObject* source, FieldObject* target, any* payload) = 0;
	};
}