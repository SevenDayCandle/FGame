export module fab.EffectKeyword;

import fab.BaseContent;
import fab.Effect;
import fab.FUtil;
import fab.Keyword;
import std;

namespace fab {
	export class EffectKeyword : public Keyword {
	public:
		EffectKeyword(BaseContent& content, strv id): Keyword(content, id) {}

		vec<uptr<Effect>> effects;
	};
}