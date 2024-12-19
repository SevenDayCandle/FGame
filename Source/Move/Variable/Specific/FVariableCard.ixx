export module fab.FVariableCard;

import fab.Card;
import fab.CombatInstance;
import fab.FGetter;
import fab.FieldObject;
import fab.FUtil;
import fab.GameObject;
import fab.PileType;
import std;

namespace fab {
	export class FVariableCard : public FGetter {
	public:
		static constexpr cstr FIELD_PILES = "piles";
		inline static auto DATA = DataD<FVariableCard>();

		FVariableCard(): FGetter(DATA) {}
		FVariableCard(const Save& save) : FGetter(DATA, save) {}
		FVariableCard(const FVariableCard& other) : FGetter(other) {}
		virtual ~FVariableCard() final = default;

		vec<PileType*> piles;

		inline uptr<FVariable> clone() final { return make_unique<FVariableCard>(*this); }

		int getResult(CombatInstance* instance, GameObject* source, FieldObject* target, any* payload, any* outputPayload) final;
		void serializeImpl(Save& save) const final;
	protected:
		virtual void loadFields(const Save& save) final;
	};

	int FVariableCard::getResult(CombatInstance* instance, GameObject* source, FieldObject* target, any* payload, any* outputPayload) {
		// TODO
		return 0;
	}

	void FVariableCard::serializeImpl(Save& save) const {
		save.fields = { 
			{FIELD_PILES , futil::toString(piles)}
		};
	}

	void FVariableCard::loadFields(const Save& save) {
		if (save.fields) {
			piles = futil::fromString<vec<PileType*>>(save.fields->at(FIELD_PILES));
		}
	}
}