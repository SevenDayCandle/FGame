export module fab.FGetter;

import fab.FFilter;
import fab.FFilterGroup;
import fab.FUtil;
import fab.FVariable;
import fab.KeyedItem;
import std;

namespace fab {
	export class FGetter : public FVariable {
	public:
		class Data : public KeyedItem<Data> {
		public:
			Data(strv name) : KeyedItem(name) {}

			virtual uptr<FGetter> create() const = 0;
			virtual uptr<FGetter> create(const Save& save) const = 0;

			// TODO additional attributes
		};

		template <typename T> class DataD : public Data {
		public:
			DataD() : Data(typeid(T).name()) {}

			inline uptr<FGetter> create() const final { return make_unique<T>(); }
			inline uptr<FGetter> create(const Save& save) const final { return make_unique<T>(save); }
		};

		FGetter(Data& data) : data(data) {}
		FGetter(Data& data, const Save& save) : data(data) {
			load(save);
		}
		FGetter(const FGetter& other) : data(other.data), filters(other.filters) {}
		FGetter(FGetter&& other) noexcept = default;
		virtual ~FGetter() = default;

		FFilterGroup filters;

		inline strv getId() const final { return data.id; }

		const Data& data;
	protected:
		inline virtual void loadFields(const Save& save) {}

		void load(const Save& save);
	};
}