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
		FCond(Data& data, const Save& save) : FEffect(data, save) {}
		FCond(const FEffect& other) : FEffect(other) {}
		FCond(FCond&& other) noexcept = default;
		virtual ~FCond() = default;

		void use(CombatInstance* instance, GameObject* source, FieldObject* target, any* payload) final;

		virtual vec<int> getIndices(CombatInstance* instance, GameObject* source, FieldObject* target, any* payload, any& finalPayload) = 0;
	};
}