export module fab.FieldObject;

import fab.AttributeObject;
import fab.FUtil;
import fab.IDrawable;
import std;

/*
 * Objects that can exist on the game grid
 */
namespace fab {
	export class FieldObject {
	public:
		FieldObject() {}
		FieldObject(FieldObject&& other) = default;
		virtual ~FieldObject() = default;

		vec<uptr<AttributeObject>> statuses;
	};
}