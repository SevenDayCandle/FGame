export module fab.FModifier;

import fab.CombatInstance;
import fab.FFilter;
import fab.FieldObject;
import fab.FUtil;
import fab.FVariable;
import fab.FVariableGroup;
import fab.GameObject;
import fab.KeyedItem;
import std;

namespace fab {
	export class FModifier : public FVariable {
	public:
		class Data : public KeyedItem<Data> {
		public:
			Data(strv name) : KeyedItem(name) {}

			virtual uptr<FModifier> create() const = 0;
			virtual uptr<FModifier> create(const Save& save) const = 0;
		};

		template <typename T> class DataD : public Data {
		public:
			DataD() : Data(typeid(T).name()) {}

			inline uptr<FModifier> create() const final { return make_unique<T>(); }
			inline uptr<FModifier> create(const Save& save) const final { return make_unique<T>(save); }
		};

		FModifier(Data& data) : data(data) {}
		FModifier(Data& data, const Save& save) : data(data) {
			load(save);
		}
		FModifier(const FModifier& other) : data(other.data), vars(other.vars) {}
		FModifier(FModifier&& other) noexcept = default;
		virtual ~FModifier() = default;

		const Data& data;
		FVariableGroup vars;

		inline strv getId() const final { return data.id; }

		int getResult(CombatInstance* instance, GameObject* source, FieldObject* target, any* payload, any* outputPayload) final;
		void serializeImpl(Save& save) const final;
	protected:
		void load(const Save& save);

		virtual int modifyResult(vec<int>& res) = 0;
	};
}