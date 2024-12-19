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
			opt<str> data;
		};

		class Data : public KeyedItem<Data> {
		public:
			Data(strv name) : KeyedItem(name) {}

			virtual uptr<FFilter> create() const = 0;
			virtual uptr<FFilter> create(const Save& save) const = 0;

			// TODO additional attributes
		};

		template <typename T> class DataD : public Data {
		public:
			DataD() : Data(typeid(T).name()) {}

			inline uptr<FFilter> create() const final { return make_unique<T>(); }
			inline uptr<FFilter> create(const Save& save) const final { return make_unique<T>(save); }
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

		Save serialize() const;

		static uptr<FFilter> create(const Save& save);

		virtual bool passes(void* object) = 0;
		virtual uptr<FFilter> clone() = 0; // Make a unique pointer copy of this object
		virtual void serializeImpl(Save& save) const = 0; // Additional fields to add to the save
	protected:
		virtual void loadFields(const Save& save) = 0;
	};

	FFilter::Save FFilter::serialize() const {
		Save save = { data.id, isNot, isOr };
		serializeImpl(save);
		return save;
	}

	uptr<FFilter> FFilter::create(const Save& save) {
		if (FFilter::Data* data = FFilter::Data::get(save.id)) {
			return data->create(save);
		}
		return uptr<FFilter>();
	}
}