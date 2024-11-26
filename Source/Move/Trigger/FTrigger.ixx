export module fab.FTrigger;

import fab.CombatInstance;
import fab.FMove;
import fab.FUtil;
import fab.KeyedItem;
import std;

namespace fab {
	export class FTrigger : public virtual CombatInstance::Subscriber {
	public:
		class Data : public KeyedItem<Data> {
		public:
			Data(strv name) : KeyedItem(name) {}

			virtual uptr<FTrigger> create(FMove& source) const = 0;
		};

		template <c_ext<FTrigger> T> class DataD : public Data {
		public:
			DataD() : Data(typeid(T).name()) {}

			inline uptr<T> create(FMove& source) const final { return make_unique<T>(source); }
		};

		FTrigger(Data& data, FMove& source): data(data), source(source) {}
		virtual ~FTrigger() = default;

		Data& data;
		FMove& source;
	};
}