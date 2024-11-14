export module fab.CardData;

import fab.BaseContent;
import fab.BaseStrings;
import fab.CardGroup;
import fab.CardType;
import fab.FEffect;
import fab.FUtil;
import fab.GameObjectData;
import fab.ObjectRarity;
import std;

namespace fab {
	export class CardData : public GameObjectData<CardData> {
	public:
		static constexpr cstr FOLDER = "Cards";
		static constexpr cstr LOCPATH = "CardStrings";

		struct CardTargeting {
			bool targetAlly = false;
			bool targetEnemy = true;
			bool targetSquare = false;
			int targetRangeBegin = 1;
			int targetRangeBeginUp = 0;
			int targetRangeEnd = 1;
			int targetRangeEndUp = 0;
			int targetSizeX = 1;
			int targetSizeXUp = 0;
			int targetSizeY = 1;
			int targetSizeYUp = 0;
		};

		struct ExportFields {
			bool isUpgradeBranchOptional = true;
			bool targetAlly = false;
			bool targetEnemy = true;
			bool targetSquare = false;
			int costBase = 3;
			int costUp = 0;
			int BeginCopies = -1;
			int BeginUpgrades = 1;
			int targetRangeBegin = 1;
			int targetRangeBeginUp = 0;
			int targetRangeEnd = 1;
			int targetRangeEndUp = 0;
			int targetSizeX = 1;
			int targetSizeXUp = 0;
			int targetSizeY = 1;
			int targetSizeYUp = 0;
			str group;
			str type;
			str rarity;
			vec<FEffect::Save> effects;
			vec<str> tags;
			vec<str> upgradeBranches;
		};

		struct RuntimeFields {
			RuntimeFields() {}
			RuntimeFields(const ExportFields& fields) {
				isUpgradeBranchOptional = fields.isUpgradeBranchOptional;
				costBase = fields.costBase;
				costUp = fields.costUp;
				BeginCopies = fields.BeginCopies;
				BeginUpgrades = fields.BeginUpgrades;
				target.targetAlly = fields.targetAlly;
				target.targetEnemy = fields.targetEnemy;
				target.targetSquare = fields.targetSquare;
				target.targetRangeBegin = fields.targetRangeBegin;
				target.targetRangeBeginUp = fields.targetRangeBeginUp;
				target.targetRangeEnd = fields.targetRangeEnd;
				target.targetRangeEndUp = fields.targetRangeEndUp;
				target.targetSizeX = fields.targetSizeX;
				target.targetSizeXUp = fields.targetSizeXUp;
				target.targetSizeY = fields.targetSizeY;
				target.targetSizeYUp = fields.targetSizeYUp;
				group = CardGroup::get(fields.group);
				type = CardType::get(fields.type);
				rarity = ObjectRarity::get(fields.rarity);
				tags = fields.tags;
				upgradeBranches = fields.upgradeBranches;

				for (const FEffect::Save& effectStr : fields.effects) {
					uptr<FEffect> effect = FEffect::create(effectStr);
					if (effect) {
						effects.push_back(move(effect));
					}
				}
			}
			RuntimeFields(RuntimeFields&& other) = default;

			bool isUpgradeBranchOptional = true;
			CardGroup* group;
			CardTargeting target;
			CardType* type;
			int costBase = 3;
			int costUp = 0;
			int BeginCopies = -1;
			int BeginUpgrades = 1;
			ObjectRarity* rarity;
			vec<uptr<FEffect>> effects;
			vec<str> tags;
			vec<str> upgradeBranches;
		};

		CardData(BaseContent& source, strv id) : GameObjectData(source, id) {}
		CardData(BaseContent& source, strv id, const ExportFields& fields) : GameObjectData(source, id) {}

		RuntimeFields data;

		inline int getCost(int level) const { return data.costBase + data.costUp * level; }
		inline int getTargetRangeBegin(int level) const { return data.target.targetRangeBegin + data.target.targetRangeBeginUp * level; }
		inline int getTargetRangeEnd(int level) const { return data.target.targetRangeEnd + data.target.targetRangeEndUp * level; }
		inline int getTargetSizeX(int level) const { return data.target.targetSizeX + data.target.targetSizeXUp * level; }
		inline int getTargetSizeY(int level) const { return data.target.targetSizeY + data.target.targetSizeYUp * level; }

		ExportFields createExport() const;
	};


	CardData::ExportFields CardData::createExport() const
	{
		ExportFields fields = ExportFields();
		fields.isUpgradeBranchOptional = data.isUpgradeBranchOptional;
		fields.costBase = data.costBase;
		fields.costUp = data.costUp;
		fields.BeginCopies = data.BeginCopies;
		fields.BeginUpgrades = data.BeginUpgrades;
		fields.targetAlly = data.target.targetAlly;
		fields.targetEnemy = data.target.targetEnemy;
		fields.targetSquare = data.target.targetSquare;
		fields.targetRangeBegin = data.target.targetRangeBegin;
		fields.targetRangeBeginUp = data.target.targetRangeBeginUp;
		fields.targetRangeEnd = data.target.targetRangeEnd;
		fields.targetRangeEndUp = data.target.targetRangeEndUp;
		fields.targetSizeX = data.target.targetSizeX;
		fields.targetSizeXUp = data.target.targetSizeXUp;
		fields.targetSizeY = data.target.targetSizeY;
		fields.targetSizeYUp = data.target.targetSizeYUp;
		fields.group = data.group ? data.group->toString() : "";
		fields.type = data.type ? data.type->id : "";
		fields.rarity = data.rarity ? data.rarity->id : "";
		fields.tags = data.tags;
		fields.upgradeBranches = data.upgradeBranches;
		// TODO effects
		return fields;
	}
}