export module fab.GameObject;

import fab.BaseContent;
import fab.FUtil;
import fab.GameObjectData;
import fab.ObjectStrings;
import std;

/*
 * Objects that make up the game's "content"
 */
namespace fab {
	export class GameObject {
	public:
		GameObject() {}
		GameObject(GameObject&& other) = default;
		virtual ~GameObject() = default;

		virtual BaseContent& source() = 0;
		virtual ObjectStrings* strings() = 0;
		virtual strv id() = 0;
	};

	export template <typename T> requires c_ext<T, GameObjectData<T>> class GameObjectD : public GameObject {
	public:
		GameObjectD(T& data) : data(data), GameObject() {}

		T& data;

		inline BaseContent& source() final override { return data.source; }
		inline ObjectStrings* strings() override { return data.strings; }
		inline strv description(int ind) { return data.strings && ind < data.strings->DESCRIPTION.size() ? data.strings->DESCRIPTION[ind] : futil::BLANK; }
		inline strv id() override { return data.id; }
		inline strv name() { return data.strings ? data.strings.NAME : futil::BLANK; }
	};
}