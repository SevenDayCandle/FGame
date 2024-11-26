export module fab.FMove;

import fab.BaseContent;
import fab.CombatInstance;
import fab.FieldObject;
import fab.FUtil;
import fab.FVariable;
import fab.GameObject;
import fab.KeyedItem;
import std;

namespace fab {
	export class FMove {
	public:
		FMove() {}
		FMove(const FMove& other): owner(other.owner) {
			this->children.reserve(other.children.size());
			for (const uptr<FMove>& u : other.children) {
				children.emplace_back(u->clone());
			}
		}
		virtual ~FMove() = default;

		inline auto getChildren() const { return std::views::transform(children, [](const uptr<FMove>& child) { return child.get(); }); }
		inline GameObject* getOwner() const { return owner; }
		virtual inline bool setParent(FMove* parent) { return false; }
		virtual inline FMove* getParent() const { return nullptr; }

		FMove& addChild(uptr<FMove>&& child);
		FMove& setOwner(GameObject* owner);

		virtual uptr<FMove> clone() = 0;
		virtual void use(CombatInstance* instance, GameObject* source, FieldObject* target, any* payload) = 0;
	protected:
		GameObject* owner;
		vec<uptr<FMove>> children;
	};
}