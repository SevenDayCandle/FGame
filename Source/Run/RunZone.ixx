export module fab.RunZone;

import fab.BaseContent;
import fab.ContentRegisterable;
import fab.FUtil;
import std;

namespace fab {
	export class RunZone : public ContentRegisterable<RunZone> {
	public:
		struct Data {
			bool allowCommonEncounters = true;
			int sizeCols = 5;
			int sizeRows = 8;
			umap<str, int> roomWeights;
			vec<int> levels;
		};
		RunZone(BaseContent& source, strv id, Data& data) : ContentRegisterable(source, id), data(data) {}
		virtual ~RunZone() = default;

		Data data;
	};
}