export module fab.FFilterRarity;

import fab.Card;
import fab.FFilter;
import fab.FUtil;
import fab.ObjectRarity;
import std;

namespace fab {
	export class FFilterRarity : public FFilter {
	public:
		inline static auto DATA = DataD<FFilterRarity>();

		FFilterRarity() : FFilter(DATA) {}
		FFilterRarity(const FFilter::Save& save) : FFilter(DATA, save) {}
		FFilterRarity(const FFilterRarity& other) : FFilter(other) {}
		FFilterRarity(FFilterRarity&& other) noexcept = default;
		virtual ~FFilterRarity() = default;

		vec<ObjectRarity*> rarities;

		inline uptr<FFilter> clone() final { return make_unique<FFilterRarity>(*this); }

		bool passes(void* object) final;
		void serializeImpl(Save& save) const final;
	};

	bool FFilterRarity::passes(void* object) {
		if (Card* card = reinterpret_cast<Card*>(object)) {

		}
		return false;
	}

	void FFilterRarity::serializeImpl(Save& save) const {
		save.data = futil::toString(rarities);
	}
}