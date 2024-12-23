export module fab.FCond;

import fab.CombatInstance;
import fab.FEffect;
import fab.FieldObject;
import fab.FUtil;
import fab.FVariable;
import fab.FVariableGroup;
import fab.GameObject;
import fab.KeyedItem;
import std;

namespace fab {
	export class FCond : public FEffect {
	public:
		class Data : public KeyedItem<Data> {
		public:
			Data(strv name) : KeyedItem(name) {}

			virtual uptr<FCond> create() const = 0;
			virtual uptr<FCond> create(const Save& save) const = 0;

			// TODO additional attributes for restricting possible variables, inferring category
		};

		template <typename T> class DataD : public Data {
		public:
			DataD() : Data(nameof<T>()) {}

			inline uptr<FCond> create() const final { return make_unique<T>(); }
			inline uptr<FCond> create(const Save& save) const final { return make_unique<T>(save); }
		};

		FCond(Data& data): data(data) {}
		FCond(Data& data, const Save& save) : FEffect(save), data(data)  {
			FCond::loadImpl(save);
		}
		FCond(const FCond& other) : FEffect(other), data(other.data), vars(other.vars) {}
		FCond(FCond&& other) noexcept = default;
		virtual ~FCond() = default;

		const Data& data;
		FVariableGroup vars;

		inline strv getId() const final { return data.id; }

		void use(CombatInstance* instance, GameObject* source, FieldObject* target, any* payload) final;

		virtual vec<int> getIndices(CombatInstance* instance, GameObject* source, FieldObject* target, any* payload, any& finalPayload) = 0;
	protected:
		virtual void loadImpl(const Save& save) override;
	};
}