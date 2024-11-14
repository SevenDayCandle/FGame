export module fab.Keyword;

import fab.BaseContent;
import fab.FEffect;
import fab.FUtil;
import fab.ContentRegisterable;
import std;

namespace fab {
	export constexpr strv PATH_KEYWORDS = "Keywords";

	export class Keyword : public ContentRegisterable<Keyword> {
	public:
		Keyword(BaseContent& source, strv id) : ContentRegisterable(source, id) {}
	};
}