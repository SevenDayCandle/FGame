export module fab.GameObjectData;

import fab.BaseContent;
import fab.ContentRegisterable;
import fab.FUtil;
import fab.ObjectStrings;

namespace fab {
	export template <typename T> class GameObjectData : public ContentRegisterable<T> {
	public:
		ObjectStrings text;
	protected:
		GameObjectData(BaseContent& source, strv id) : ContentRegisterable<T>(source, id) {}
		GameObjectData(BaseContent& source, strv id, const ObjectStrings& text) : ContentRegisterable<T>(source, id), text(text) {}
		GameObjectData(BaseContent& source, strv id, const strumap<ObjectStrings>& textMap) : ContentRegisterable<T>(source, id), textMap(textMap), text(getStrings(textMap)) {}

		strumap<ObjectStrings> textMap;
	private:
		ObjectStrings getStrings(const strumap<ObjectStrings>& textMap);
		//map<str,str> customFields;
	};

	template<typename T> ObjectStrings GameObjectData<T>::getStrings(const strumap<ObjectStrings>& textMap) {
		auto mapit = textMap.find(this->source.cfg.getLanguage().id);
		if (mapit != textMap.end()) {
			return mapit->second;
		}
		return ObjectStrings();
	}
}