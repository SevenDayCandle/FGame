export module fab.RunEncounter;

import fab.BaseContent;
import fab.ContentRegisterable;
import fab.EncounterCreatureEntry;
import fab.FUtil;

namespace fab {
	export class RunEncounter : public ContentRegisterable<RunEncounter> {
	public:
		struct Data {
			bool boss = false;
			int fieldCols = 6;
			int fieldRows = 6;
			int level = 1;
			int time = 100;
			str zone;
			vec<pair<int, int>> startPos;
			vec<EncounterCreatureEntry> creatures;
		};

		RunEncounter(BaseContent& source, strv id, Data& data): ContentRegisterable(source, id), data(data) {}
		virtual ~RunEncounter() {}

		Data data;
	};
}