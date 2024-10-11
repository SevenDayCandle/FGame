export module fab.EffectData;

import fab.BaseContent;
import fab.FUtil;
import fab.ContentRegisterable;
import std;

namespace fab {
	export class EffectData : public ContentRegisterable<EffectData> {
	public:
		EffectData(BaseContent& source, strv id) : ContentRegisterable(source, id) {}
	};
}