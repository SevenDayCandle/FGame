export module fab.FPrimary;

import fab.CombatInstance;
import fab.FEffect;
import fab.FieldObject;
import fab.FMove;
import fab.FUtil;
import fab.FTrigger;
import fab.GameObject;
import std;

namespace fab {
	export class FPrimary : public FMove {
	public:
		struct Save {
			vec<str> triggers;
			vec<FEffect::Save> children;
		};

		FPrimary() {}
		virtual ~FPrimary() = default;

		Save serialize() const;
		void subscribe(CombatInstance& instance);
		void unsubscribe();
		void use(CombatInstance* instance, GameObject* source, FieldObject* target, any* payload) final;
	protected:
		vec<uptr<FTrigger>> triggers;

		void load(const Save& save);
	};
}