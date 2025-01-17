export module fab.PassiveData;

import fab.BaseContent;
import fab.FEffect;
import fab.FUtil;
import fab.GameObjectData;
import fab.ObjectRarity;
import fab.ObjectStrings;
import sdl.SDLBase;
import sdl.SDLRunner;
import std;

namespace fab {
	export class PassiveData : public GameObjectData<PassiveData> {
	public:
		static constexpr cstr FOLDER = "Passives";

		struct ExportFields {
			int maxUpgrades;
			str effects;
			str rarity;
			strumap<strumap<ObjectStrings>> text;
			vec<str> upgradeBranches;
		};

		struct RuntimeFields {
			RuntimeFields() {}
			RuntimeFields(const ExportFields& fields) {
				maxUpgrades = fields.maxUpgrades;
				rarity = ObjectRarity::get(fields.rarity);
				// TODO: Populate effects from fields.effects
				upgradeBranches = fields.upgradeBranches;
				text = fields.text;
			}
			RuntimeFields(RuntimeFields&& other) = default;

			int maxUpgrades = 0;
			ObjectRarity* rarity;
			strumap<strumap<ObjectStrings>> text;
			vec<str> upgradeBranches;
			vec<uptr<FEffect>> effects;
		};

		PassiveData(BaseContent& source, strv id) : GameObjectData(source, id) {}
		PassiveData(BaseContent& source, strv id, const ExportFields& fields) : GameObjectData(source, id), data(fields) {}

		RuntimeFields data;
	};
}