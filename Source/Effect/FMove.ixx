export module fab.FMove;

import fab.CallbackAction;
import fab.FEffect;
import fab.FieldObject;
import fab.GameObject;
import fab.FUtil;
import std;

namespace fab {
	export class FMove : public FEffect {
	public:
		FMove(FEffect::Data & data) : FEffect(data) {}
		virtual ~FMove() = default;

		void use(GameObject* source, FieldObject* target, any* payload) final override;

		virtual CallbackAction& runAction(GameObject* source, FieldObject* target, any* payload) = 0;
	};
}