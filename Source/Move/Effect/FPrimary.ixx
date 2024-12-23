export module fab.FPrimary;

import fab.CombatInstance;
import fab.FEffect;
import fab.FFilter;
import fab.FFilterGroup;
import fab.FieldObject;
import fab.FEffect;
import fab.FUtil;
import fab.FTrigger;
import fab.GameObject;
import std;

namespace fab {
	export class FPrimary : public FEffect {
	public:
		static constexpr strv ID = "ROOT";

		FPrimary() {}
		FPrimary(const FPrimary& other): filters(other.filters),
			triggers(futil::transform(other.triggers, [this](const uptr<FTrigger>& u) {return u->data.create(*this); })) {}
		FPrimary(const Save& save) : FEffect(save) {
			FPrimary::loadImpl(save);
		}
		virtual ~FPrimary() = default;

		FFilterGroup filters;
		vec<uptr<FTrigger>> triggers;

		inline bool allowActivateOnPlay() const {return triggers.empty() || std::ranges::any_of(triggers, [](const uptr<FTrigger>& i) { return i->allowActivateOnPlay(); }); }
		inline bool setParent(FEffect* parent) final { return false; } // Cannot have parents
		inline strv getId() const final { return ID; }
		inline uptr<FEffect> clone() final { return make_unique<FPrimary>(*this); }

		void serializeImpl(Save& save) const;
		void subscribe(CombatInstance& instance);
		void unsubscribe();
		void use(CombatInstance* instance, GameObject* source, FieldObject* target, any* payload) final;
	protected:
		void loadImpl(const Save& save) override;
	};
}