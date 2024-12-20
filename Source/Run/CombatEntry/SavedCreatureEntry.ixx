export module fab.SavedCreatureEntry;

import fab.CreatureData;
import fab.FUtil;
import fab.ItemListing;
import std;

export namespace fab {
	export class SavedCreatureEntry {
	public:
		SavedCreatureEntry() = default;
		SavedCreatureEntry(CreatureData& data, strv behavior, int upgrades) : content(data.source.id), id(data.id), behaviorId(behavior), health(data.getResultHealth(upgrades)), upgrades(upgrades), cards(data.data.cards) {}
		SavedCreatureEntry(strv contentId, strv dataId, strv behavior, int upgrades) : SavedCreatureEntry(*CreatureData::get(contentId, dataId), behavior, upgrades) {}
		SavedCreatureEntry(const SavedCreatureEntry& other) = default;
		virtual ~SavedCreatureEntry() = default;

		int health;
		int upgrades = 0;
		str behaviorId;
		str content;
		str id;
		vec<ItemListing> cards;
		vec<ItemListing> passives;
	};
}