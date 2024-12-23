export module fab.FFilterCardRarity;

import fab.Card;
import fab.FFilter;
import fab.FUtil;
import fab.ObjectRarity;
import std;

namespace fab {
	export class FFilterCardRarity : public FFilterD<Card> {
	public:
		inline static auto DATA = DataD<FFilterCardRarity>();

		FFilterCardRarity() : FFilterD(DATA) {}
		FFilterCardRarity(const FFilter::Save& save) : FFilterD(DATA, save) { loadFields(save); }
		FFilterCardRarity(const FFilterCardRarity& other) : FFilterD(other) {}
		FFilterCardRarity(FFilterCardRarity&& other) noexcept = default;
		virtual ~FFilterCardRarity() = default;

		set<ObjectRarity*> rarities;

		inline uptr<FFilter> clone() final { return make_unique<FFilterCardRarity>(*this); }

		bool passesAs(Card* card) const final;
	protected:
		void loadFields(const Save& save);
		void serializeImpl(Save& save) const final;
	};

	bool FFilterCardRarity::passesAs(Card* card) const {
		if (card) {
			return rarities.contains(card->rarity());
		}
		return false;
	}

	void FFilterCardRarity::loadFields(const Save& save) {
		if (save.data) {
			rarities = futil::fromString<set<ObjectRarity*>>(save.data.value());
		}
	}

	void FFilterCardRarity::serializeImpl(Save& save) const {
		save.data = futil::toString(rarities);
	}
}