export module fab.FEffect;

import fab.BaseContent;
import fab.CombatInstance;
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
			vec<FVariable::Save> vars;
			vec<Save> children;
		};

		class Data : public KeyedItem<Data> {
		public:
			Data(strv name) : KeyedItem(name) {}
			
			virtual uptr<FEffect> create() const = 0;
			virtual uptr<FEffect> create(const Save& save) const = 0;
			virtual uptr<FEffect> create(const FEffect& other) const = 0;

			// TODO additional attributes for restricting possible variables, inferring category
		};

		template <c_ext<FEffect> T> class DataT : public Data {
		public:
			DataT() : Data(typeid(T).name()) {}

			inline uptr<T> create() const final { return make_unique<T>(); }
			inline uptr<T> create(const Save& save) const final { return make_unique<T>(save); }
			inline uptr<T> create(const FEffect& other) const final { return make_unique<T>(other); }
		};

		FEffect(Data& data): data(data) {}
		FEffect(Data& data, const Save& save) : data(data) {
			load(save);
		}
		FEffect(const FEffect& other) : FMove(other), data(other.data),
			vars(futil::transform<uptr<FVariable>, uptr<FVariable>>(other.vars, [](const uptr<FVariable>& u) {return u->data.create(*u); })) {
		}
		FEffect(FEffect&& other) noexcept = default;
		virtual ~FEffect() = default;

		const Data& data;
		FMove* parent;

		inline bool setParent(FMove* parent) final { return this->parent = parent, true; }
		inline FMove* getParent() const final { return parent; }
		inline uptr<FMove> clone() final { return data.create(*this); }

		Save serialize() const;
		virtual any getPayload(CombatInstance* instance, GameObject* source, FieldObject* target, any* payload);

		static uptr<FEffect> create(const Save& save);
	protected:
		vec<uptr<FVariable>> vars;

		void load(const Save& save);
	};
}