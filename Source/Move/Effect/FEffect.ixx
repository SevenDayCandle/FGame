export module fab.FEffect;

import fab.BaseContent;
import fab.CombatInstance;
import fab.FFilter;
import fab.FieldObject;
import fab.FUtil;
import fab.FVariable;
import fab.GameObject;
import fab.KeyedItem;
import std;

namespace fab {
	export class FEffect {
	public:
		struct Save {
			str id;
			opt<vec<FFilter::Save>> filters;
			opt<vec<FVariable::Save>> vars;
			opt<vec<Save>> children;
			opt<vec<str>> triggers;
		};

		FEffect() {}
		FEffect(const Save& save) {
			load(save);
		}
		FEffect(const FEffect& other): owner(other.owner) {
			this->children.reserve(other.children.size());
			for (const uptr<FEffect>& u : other.children) {
				children.emplace_back(u->clone());
			}
		}
		virtual ~FEffect() = default;

		inline auto getChildren() const { return std::views::transform(children, [](const uptr<FEffect>& child) { return child.get(); }); } // Iterate over the effects directly underneath this effect
		inline GameObject* getOwner() const { return owner; } // Get the game object that owns this effect
		virtual inline bool setParent(FEffect* parent) { return this->parent = parent, true; } // Assign this effect as a child effect to another effect
		virtual inline FEffect* getParent() const { return parent; } // Get the effect that is above this effect

		FEffect& addChild(uptr<FEffect>&& child);
		FEffect& setOwner(GameObject* owner);
		Save serialize() const;

		static uptr<FEffect> create(const Save& save);

		virtual uptr<FEffect> clone() = 0;
		virtual strv getId() const = 0;
		virtual void serializeImpl(Save& save) const = 0;
		virtual void use(CombatInstance* instance, GameObject* source, FieldObject* target, any* payload) = 0;
	protected:
		FEffect* parent;
		GameObject* owner;
		vec<uptr<FEffect>> children;

		void load(const Save& save);

		virtual void loadImpl(const Save& save) = 0;
	};
}