export module fab.GameObjectData;

import fab.BaseContent;
import fab.ContentRegisterable;
import fab.FUtil;
import fab.ObjectStrings;

namespace fab {
	export template <typename T> class GameObjectData : public ContentRegisterable<T> {
	public:
		ObjectStrings* strings;
	protected:
		GameObjectData(BaseContent& source, strv id) : ContentRegisterable<T>(source, id), strings(source.getObjectStrings(T::LOCPATH, id)) {}
		GameObjectData(BaseContent& source, strv id, ObjectStrings* strings) : ContentRegisterable<T>(source, id), strings(strings) {}
	private:
		//map<str,str> customFields;
	};
}