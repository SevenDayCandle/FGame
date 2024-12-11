export module fab.FVariableCard;

import fab.Card;
import fab.CombatInstance;
import fab.FieldObject;
import fab.FUtil;
import fab.FVariable;
import fab.GameObject;
import fab.PileType;
import std;

namespace fab {
	export class FVariableCard : public FVariable {
	public:
		static constexpr cstr FIELD_PILES = "piles";
		inline static auto DATA = DataD<FVariableCard>();

		FVariableCard(): FVariable(DATA) {}
		FVariableCard(const Save& save) : FVariable(DATA, save) {}
		FVariableCard(const FVariable& other) : FVariable(other) {}
		virtual ~FVariableCard() final = default;

		vec<PileType*> piles;

		virtual int getResult(CombatInstance* instance, GameObject* source, FieldObject* target, any* payload, any* outputPayload) override;
		virtual opt<strumap<str>> serializeFields() const final;
	protected:
		virtual void loadFields(const Save& save) final;
	};

	int FVariableCard::getResult(CombatInstance* instance, GameObject* source, FieldObject* target, any* payload, any* outputPayload) {
		return 0;
	}

	opt<strumap<str>> FVariableCard::serializeFields() const {
		strumap<str> fields;
		fields[FIELD_PILES] = futil::toString(piles);
		return fields;
	}

	void FVariableCard::loadFields(const Save& save) {
		if (save.fields) {
			piles = futil::fromString<vec<PileType*>>(save.fields->at(FIELD_PILES));
		}
	}
}