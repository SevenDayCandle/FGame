export module fab.Effect;

import fab.BaseContent;
import fab.EffectData;
import fab.FieldObject;
import fab.GameObject;
import fab.FUtil;
import fab.Variable;
import std;

namespace fab {
	export class Effect {
	public:
		Effect(EffectData& data): data(data) {}
		virtual ~Effect() = default;

		const EffectData& data;

		virtual str getText();
		virtual void refresh();

		virtual str getSubText() = 0;
		virtual void use(GameObject* source, FieldObject* target) = 0;
	protected:
		Effect* parent;
		GameObject* owner;
		uptr<Variable> variable;
		vec<uptr<Effect>> childEffects;
	};
}