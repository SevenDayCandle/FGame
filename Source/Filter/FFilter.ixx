export module fab.FFilter;

import fab.FUtil;
import fab.KeyedItem;
import std;

namespace fab {
	export class FFilter {
	public:
		struct Save {
			str id;
			bool isNot;
			bool isOr;
			opt<strumap<str>> fields;
		};

		class Data : public KeyedItem<Data> {
		public:
			Data(strv name) : KeyedItem(name) {}

			virtual uptr<FFilter> create() const = 0;
			virtual uptr<FFilter> create(const Save& save) const = 0;
			virtual uptr<FFilter> create(const FFilter& other) const = 0;

			// TODO additional attributes
		};

		template <typename T> class DataD : public Data {
		public:
			DataD() : Data(typeid(T).name()) {}

			inline uptr<FFilter> create() const final { return make_unique<T>(); }
			inline uptr<FFilter> create(const Save& save) const final { return make_unique<T>(save); }
			inline uptr<FFilter> create(const FFilter& other) const final { return make_unique<T>(other); }
		};

		FFilter(Data& data) : data(data) {}
		FFilter(Data& data, const Save& save) : data(data) {
			load(save);
		}
		FFilter(const FFilter& other) : data(other.data) {}
		FFilter(FFilter&& other) noexcept = default;
		virtual ~FFilter() = default;

		bool isNot;
		bool isOr;
		const Data& data;

		inline void load(const Save& save) { loadFields(save); }
		inline Save serialize() const { return Save(data.id, isNot, isOr, serializeFields()); }

		static uptr<FFilter> create(const Save& save);

		virtual bool passes(void* object) = 0;
		virtual strumap<str> serializeFields() const = 0;
	protected:
		virtual void loadFields(const Save& save) = 0;
	};

	uptr<FFilter> FFilter::create(const Save& save) {
		FFilter::Data* data = FFilter::Data::get(save.id);
		if (data) {
			return data->create(save);
		}
		return uptr<FFilter>();
	}
}