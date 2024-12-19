export module fab.FAct;

import fab.CallbackAction;
import fab.CombatInstance;
import fab.FEffect;
import fab.FFilter;
import fab.FFilterGroup;
import fab.FieldObject;
import fab.FUtil;
import fab.FVariable;
import fab.FVariableGroup;
import fab.GameObject;
import fab.KeyedItem;
import std;

namespace fab {
	export class FAct : public FEffect {
	public:
		class Data : public KeyedItem<Data> {
		public:
			Data(strv name) : KeyedItem(name) {}

			virtual uptr<FAct> create() const = 0;
			virtual uptr<FAct> create(const Save& save) const = 0;

			// TODO additional attributes for restricting possible variables, inferring category
		};

		template <typename T> class DataD : public Data {
		public:
			DataD() : Data(typeid(T).name()) {}

			inline uptr<FAct> create() const final { return make_unique<T>(); }
			inline uptr<FAct> create(const Save& save) const final { return make_unique<T>(save); }
		};

		FAct(Data& data): data(data) {}
		FAct(Data& data, const Save& save) : FEffect(save), data(data) {}
		FAct(const FAct& other) : FEffect(other), data(other.data), filters(other.filters), vars(other.vars) {}
		FAct(FAct&& other) noexcept = default;
		virtual ~FAct() = default;

		const Data& data;
		FFilterGroup filters;
		FVariableGroup vars;

		inline strv getId() const final { return data.id; }
		inline virtual int invokeVars(CombatInstance* instance, GameObject* source, FieldObject* target, any* payload, any* retrieval = nullptr) { return vars.getResult(0, instance, source, target, payload, retrieval); }

		void serializeImpl(Save& save) const override;
		void use(CombatInstance* instance, GameObject* source, FieldObject* target, any* payload) final override;

		virtual any getPayload(CallbackAction& act, CombatInstance* instance, GameObject* source, FieldObject* target) = 0;
		virtual CallbackAction& runAction(CombatInstance& instance, GameObject* source, FieldObject* target, any* payload) = 0;
	protected:
		virtual void loadImpl(const Save& save) override;
	};
}