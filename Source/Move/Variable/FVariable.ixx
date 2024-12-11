export module fab.FVariable;

import fab.CombatInstance;
import fab.FFilter;
import fab.FieldObject;
import fab.FUtil;
import fab.GameObject;
import fab.KeyedItem;
import std;

namespace fab {
	export class FVariable {
	public:
		struct Save {
			str id;
			opt<strumap<str>> fields;
			opt<vec<Save>> vars;
		};

		class Data : public KeyedItem<Data> {
		public:
			Data(strv name) : KeyedItem(name) {}

			virtual uptr<FVariable> create() const = 0;
			virtual uptr<FVariable> create(const Save& save) const = 0;
			virtual uptr<FVariable> create(const FVariable& other) const = 0;

			// TODO additional attributes
		};

		template <typename T> class DataD : public Data {
		public:
			DataD() : Data(typeid(T).name()) {}

			inline uptr<FVariable> create() const final { return make_unique<T>(); }
			inline uptr<FVariable> create(const Save& save) const final { return make_unique<T>(save); }
			inline uptr<FVariable> create(const FVariable& other) const final { return make_unique<T>(other); }
		};

		FVariable(Data& data) : data(data) {}
		FVariable(Data& data, const Save& save) : data(data) {
			load(save);
		}
		FVariable(const FVariable& other) : data(other.data) {}
		FVariable(FVariable&& other) noexcept = default;
		virtual ~FVariable() = default;

		const Data& data;

		inline Save serialize() const { return Save(data.id, serializeFields(), serializeVars()); }
		inline virtual opt<strumap<str>> serializeFields() const { return opt<strumap<str>>(); };
		inline virtual opt<vec<Save>> serializeVars() const { return opt<vec<Save>>(); };
		inline void load(const Save& save) { loadFields(save); }

		static uptr<FVariable> create(const Save& save);

		virtual int getResult(CombatInstance* instance, GameObject* source, FieldObject* target, any* payload, any* outputPayload) = 0;
	protected:
		virtual void loadFields(const Save& save) = 0;
	};

	uptr<FVariable> FVariable::create(const Save& save) {
		FVariable::Data* data = FVariable::Data::get(save.id);
		if (data) {
			return data->create(save);
		}
		return uptr<FVariable>();
	}
}