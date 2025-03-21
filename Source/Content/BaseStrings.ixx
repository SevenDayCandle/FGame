module;

#include "glaze/glaze.hpp"

export module fab.BaseStrings;

import fab.BaseContent;
import fab.ContentLoadables;
import fab.CoreConfig;
import fab.FUtil;
import fab.KeywordStrings;
import fab.Language;
import fab.ObjectStrings;

namespace fab {
	export constexpr strv DEFAULT_KEYWORDS = "KeywordStrings";
	export constexpr strv DEFAULT_UI = "UIStrings";

	export class BaseStrings : public ContentLoadables {
	public:
		BaseStrings(const BaseContent& content) : ContentLoadables(content) {}

		inline virtual KeywordStrings* getKeywordStrings(strv path) const { return nullptr; }

		virtual void dispose() {};
		virtual void initialize() {};
	protected:
		inline path getBasePathForLanguage() const {
			return getBasePathForLanguage(content.cfg.getLanguage());
		}
		inline path getBasePathForLanguage(const Language& lang) const {
			return content.contentFolder / STRINGS_PATH / lang.id;
		}
		inline path getPathForLanguage(const strv& suffix) const {
			return getPathForLanguage(content.cfg.getLanguage(), suffix);
		};
		inline path getPathForLanguage(const Language& lang, const strv& suffix) const {
			return (content.contentFolder / STRINGS_PATH / lang.id / suffix).replace_extension(futil::JSON_EXT);
		};
		bool loadKeywordStrings(strumap<KeywordStrings>& res, const strv& suffix = DEFAULT_KEYWORDS);
		bool loadUIStrings(strumap<str>& res, const strv& suffix = DEFAULT_UI);
	};

	bool BaseStrings::loadKeywordStrings(strumap<KeywordStrings>& res, const strv& suffix)
	{
		return !glz::read_file_json(res, getPathForLanguage(suffix).string(), str{});
	}

	bool BaseStrings::loadUIStrings(strumap<str>& res, const strv& suffix) {
		return !glz::read_file_json(res, getPathForLanguage(suffix).string(), str{});
	}
}