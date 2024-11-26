export module fab.FCond;

import fab.CombatInstance;
import fab.FEffect;
import fab.FieldObject;
import fab.GameObject;
import fab.FUtil;
import std;

namespace fab {
	export class FCond : public FEffect {
	public:
		FCond(FEffect::Data& data): FEffect(data) {}
		virtual ~FCond() = default;

		void use(CombatInstance* instance, GameObject* source, FieldObject* target, any* payload) final;

		virtual bool check(const GameObject* source, const FieldObject* target, const any* payload) = 0;
	};
}