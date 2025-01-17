export module fab.DynamicStrings;

import fab.BaseContent;
import fab.BaseStrings;
import fab.FUtil;
import fab.KeywordStrings;
import fab.ObjectStrings;
import std;

namespace fab {
	export class DynamicStrings : public BaseStrings {
	public:
		DynamicStrings(const BaseContent& content): BaseStrings(content) {}

		KeywordStrings* getKeywordStrings(strv path) const override;
		void initialize() override;
	private:
		mutable strumap<KeywordStrings> keywordStrings;
		mutable strumap<strumap<ObjectStrings>> objectStrings;
	};

	KeywordStrings* DynamicStrings::getKeywordStrings(strv path) const
	{
		auto it = keywordStrings.find(path);
		if (it != keywordStrings.end()) {
			return &it->second;
		}
		return nullptr;
	}

	// Core contains UI strings, keyword strings
	void DynamicStrings::initialize()
	{
		loadKeywordStrings(keywordStrings);
	}
}