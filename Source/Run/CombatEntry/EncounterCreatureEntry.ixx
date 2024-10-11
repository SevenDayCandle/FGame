export module fab.EncounterCreatureEntry;

import fab.FUtil;
import std;

namespace fab {
	export struct EncounterCreatureEntry {
		int faction = 1;
		int posCol = -1;
		int posRow = -1;
		int upgrades = 0;
		str id;
	};
}