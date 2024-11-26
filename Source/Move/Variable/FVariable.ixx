export module fab.FVariable;

import fab.CombatInstance;
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
			strumap<str> fields;
		};

		class Data : public KeyedItem<Data> {
		public:
			Data(strv name) : KeyedItem(name) {}

			virtual uptr<FVariable> create() const = 0;
			virtual uptr<FVariable> create(const Save& save) const = 0;
			virtual uptr<FVariable> create(const FVariable& other) const = 0;

			// TODO additional attributes
		};

		template <typename T> class DataT : public Data {
		public:
			DataT() : Data(typeid(T).name()) {}

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

		inline void load(const Save& save) { loadFields(save); }
		inline Save serialize() const { return Save(data.id, serializeFields()); }

		static uptr<FVariable> create(const Save& save);

		virtual any getValue(CombatInstance* instance, GameObject* source, FieldObject* target, void* payload) = 0;
		virtual strumap<str> serializeFields() const = 0;
	protected:
		virtual void loadFields(const Save& save) = 0;
	};
}