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

		virtual BaseContent& source() const = 0;
		virtual ObjectStrings& text() const = 0;
		virtual strv id() const = 0;
		virtual strv name() const = 0;
	};

	export template <typename T> requires c_ext<T, GameObjectData<T>> class GameObjectD : public virtual GameObject {
	public:
		GameObjectD(T& data) : data(data), GameObject() {}

		T& data;

		inline BaseContent& source() const final { return data.source; }
		inline ObjectStrings& text() const final { return data.text; }
		inline strv id() const final { return data.id; }
		inline strv name() const final { return data.text.NAME; }
		inline strv textAt(int ind) const { return ind < data.text.TEXT.size() ? data.text.TEXT[ind] : futil::BLANK; }
	};
}