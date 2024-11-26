export module fab.FAct;

import fab.CallbackAction;
import fab.CombatInstance;
import fab.FEffect;
import fab.FieldObject;
import fab.GameObject;
import fab.FUtil;
import std;

namespace fab {
	export class FAct : public FEffect {
	public:
		FAct(FEffect::Data & data) : FEffect(data) {}
		virtual ~FAct() = default;

		void use(CombatInstance* instance, GameObject* source, FieldObject* target, any* payload) final override;

		virtual CallbackAction& runAction(GameObject* source, FieldObject* target, any* payload) = 0;
	};
}