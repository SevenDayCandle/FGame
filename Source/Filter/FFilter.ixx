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

			virtual const type_info& inputType() const = 0;
			virtual uptr<FFilter> create() const = 0;
			virtual uptr<FFilter> create(const Save& save) const = 0;

			// TODO additional attributes
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

		Save serialize() const;
		void load(const Save& save);

		static uptr<FFilter> create(const Save& save);

		virtual bool passes(any* object) const = 0;
		virtual uptr<FFilter> clone() = 0; // Make a unique pointer copy of this object
	protected:
		virtual void serializeImpl(Save& save) const = 0; // Additional fields to add to the save
	};

	export template <typename T> class FFilterD : public FFilter {
	public:
		template <typename U> class DataD : public Data {
		public:
			DataD() : Data(typeid(U).name()) {}

			inline const type_info& inputType() const final { return typeid(T); }
			inline uptr<FFilter> create() const final { return make_unique<U>(); }
			inline uptr<FFilter> create(const Save& save) const final { return make_unique<U>(save); }
		};

		FFilterD(Data& data) : FFilter(data) {}
		FFilterD(Data& data, const Save& save) : FFilter(data, save) {}
		FFilterD(const FFilterD& other) : FFilter(other) {}
		FFilterD(FFilterD&& other) noexcept = default;
		virtual ~FFilterD() = default;

		inline bool passes(any* object) const final { return object && object->type() == typeid(T) && passesAs(std::any_cast<T>(object)); }

		virtual bool passesAs(T* object) const = 0;
	};
}

namespace fab {
	FFilter::Save FFilter::serialize() const {
		Save save = { data.id, isNot, isOr };
		serializeImpl(save);
		return save;
	}

	void FFilter::load(const Save& save) {
		isNot = save.isNot;
		isOr = save.isOr;
	}

	uptr<FFilter> FFilter::create(const Save& save) {
		if (FFilter::Data* data = FFilter::Data::get(save.id)) {
			return data->create(save);
		}
		return uptr<FFilter>();
	}
}