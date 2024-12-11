export module fab.FEffect;

import fab.BaseContent;
import fab.CombatInstance;
import fab.FFilter;
import fab.FieldObject;
import fab.FMove;
import fab.FUtil;
import fab.FVariable;
import fab.GameObject;
import fab.KeyedItem;
import std;

namespace fab {
	export class FEffect : public FMove {
	public:
		struct Save {
			str id;
			opt<vec<FFilter::Save>> filters;
			opt<vec<FVariable::Save>> vars;
			opt<vec<Save>> children;
		};

		class Data : public KeyedItem<Data> {
		public:
			Data(strv name) : KeyedItem(name) {}
			
			virtual uptr<FEffect> create() const = 0;
			virtual uptr<FEffect> create(const Save& save) const = 0;
			virtual uptr<FEffect> create(const FEffect& other) const = 0;

			// TODO additional attributes for restricting possible variables, inferring category
		};

		template <typename T> class DataD : public Data {
		public:
			DataD() : Data(typeid(T).name()) {}

			inline uptr<FEffect> create() const final { return make_unique<T>(); }
			inline uptr<FEffect> create(const Save& save) const final { return make_unique<T>(save); }
			inline uptr<FEffect> create(const FEffect& other) const final { return make_unique<T>(other); }
		};

		FEffect(Data& data): data(data) {}
		FEffect(Data& data, const Save& save) : data(data) {
			load(save);
		}
		FEffect(const FEffect& other) : FMove(other), data(other.data),
			filters(other.copyFilters()),
			vars(futil::transform(other.vars, [](const uptr<FVariable>& u) {return u->data.create(*u); })) {
		}
		FEffect(FEffect&& other) noexcept = default;
		virtual ~FEffect() = default;

		const Data& data;
		FMove* parent;

		inline auto getFilters() const { return std::views::transform(filters, [](const uptr<FFilter>& x) {return x.get(); }); }
		inline auto getVars() const { return std::views::transform(vars, [](const uptr<FVariable>& x) {return x.get(); }); }
		inline bool passes(void* object) const { return std::ranges::any_of(filters, [object](const uptr<FFilter>& filter) {return filter->passes(object); }); }
		inline bool setParent(FMove* parent) final { return this->parent = parent, true; }
		inline FMove* getParent() const final { return parent; }
		inline int invokeVars(CombatInstance* instance, GameObject* source, FieldObject* target, any* payload, any* retrieval = nullptr) { return vars.empty() ? 0 : vars[0]->getResult(instance, source, target, payload, retrieval); }
		inline uptr<FMove> clone() final { return data.create(*this); }
		inline vec<uptr<FFilter>> copyFilters() const { return futil::transform(filters, [](const uptr<FFilter>& u) {return u->data.create(*u); }); }

		Save serialize() const;

		static uptr<FEffect> create(const Save& save);
	protected:
		vec<uptr<FFilter>> filters;
		vec<uptr<FVariable>> vars;

		void load(const Save& save);
	};
}