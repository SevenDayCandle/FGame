export module fab.FCond;

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

		void use(GameObject* source, FieldObject* target, any* payload) final override;

		virtual bool check(const GameObject* source, const FieldObject* target, const any* payload) = 0;
	};
}