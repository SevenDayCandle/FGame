export module fab.EffectKeyword;

import fab.BaseContent;
import fab.FEffect;
import fab.FUtil;
import fab.Keyword;
import std;

namespace fab {
	export class EffectKeyword : public Keyword {
	public:
		EffectKeyword(BaseContent& content, strv id): Keyword(content, id) {}

		vec<uptr<FEffect>> effects;
	};
}