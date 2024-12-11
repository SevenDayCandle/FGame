export module fab.FPrimary;

import fab.CombatInstance;
import fab.FEffect;
import fab.FFilter;
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
			opt<vec<str>> triggers;
			opt<vec<FEffect::Save>> children;
			opt<vec<FFilter::Save>> filters;
		};

		FPrimary() {}
		FPrimary(const FPrimary& other):
			filters(futil::transform(other.filters, [](const uptr<FFilter>& u) {return u->data.create(*u); })),
			triggers(futil::transform(other.triggers, [this](const uptr<FTrigger>& u) {return u->data.create(*this); })) {}
		FPrimary(const Save& save) {
			load(save);
		}
		virtual ~FPrimary() = default;

		inline bool allowActivateOnPlay() const {return triggers.empty() || std::ranges::any_of(triggers, [](const uptr<FTrigger>& i) { return i->allowActivateOnPlay(); }); }
		inline bool passes(void* object) const { return std::ranges::any_of(filters, [object](const uptr<FFilter>& filter) {return filter->passes(object); }); }
		inline uptr<FMove> clone() final { return make_unique<FPrimary>(*this); }

		Save serialize() const;
		void subscribe(CombatInstance& instance);
		void unsubscribe();
		void use(CombatInstance* instance, GameObject* source, FieldObject* target, any* payload) final;
	protected:
		vec<uptr<FFilter>> filters;
		vec<uptr<FTrigger>> triggers;

		void load(const Save& save);
	};
}