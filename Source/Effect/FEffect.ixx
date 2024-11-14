export module fab.FEffect;

import fab.BaseContent;
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
		FEffect(const FEffect& other) : data(other.data), owner(other.owner), 
			children(futil::transform<uptr<FEffect>, uptr<FEffect>>(other.children, [](const uptr<FEffect>& u) {return u->data.create(*u); })),
			vars(futil::transform<uptr<FVariable>, uptr<FVariable>>(other.vars, [](const uptr<FVariable>& u) {return u->data.create(*u); })) {}
		FEffect(FEffect&& other) noexcept = default;
		virtual ~FEffect() = default;

		const Data& data;

		inline FEffect* getParent() const { return parent; }
		inline GameObject* getOwner() const { return owner; }

		FEffect& addChild(uptr<FEffect>&& child);
		FEffect& setOwner(GameObject* owner);

		virtual any getPayload(GameObject* source, FieldObject* target, any* payload);

		static uptr<FEffect> create(const Save& save);

		virtual void use(GameObject* source, FieldObject* target, any* payload) = 0;
	protected:
		FEffect* parent;
		GameObject* owner;
		vec<uptr<FEffect>> children;
		vec<uptr<FVariable>> vars;

		void load(const Save& save);
	};
}