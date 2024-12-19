export module fab.FTrigger;

import fab.CombatInstance;
import fab.FEffect;
import fab.FUtil;
import fab.KeyedItem;
import std;

namespace fab {
	export class FTrigger : public virtual CombatInstance::Subscriber {
	public:
		class Data : public KeyedItem<Data> {
		public:
			Data(strv name) : KeyedItem(name) {}

			virtual uptr<FTrigger> create(FEffect& source) const = 0;
		};

		template <typename T> class DataD : public Data {
		public:
			DataD() : Data(typeid(T).name()) {}

			inline uptr<FTrigger> create(FEffect& source) const final { return make_unique<T>(source); }
		};

		FTrigger(Data& data, FEffect& source): data(data), source(source) {}
		virtual ~FTrigger() = default;

		Data& data;
		FEffect& source;

		inline virtual bool allowActivateOnPlay() const { return true; }
	};
}