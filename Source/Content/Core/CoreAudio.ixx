export module fab.CoreAudio;

import fab.BaseContent;
import fab.FUtil;
import fab.StaticLoadables;
import fab.FSound;
import std;

namespace fab {
	export class CoreAudio : public StaticLoadables {
	public:
		CoreAudio(BaseContent& cnt) : StaticLoadables(cnt) {}

		FSound& uiClick = cacheSound("UI/Click1.wav");
		FSound& uiHover = cacheSound("UI/Hover1.wav");
	};
}