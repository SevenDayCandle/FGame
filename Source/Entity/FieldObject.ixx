export module fab.FieldObject;

import fab.AttributeObject;
import fab.FUtil;
import fab.IDrawable;
import std;

namespace fab {
	export class FieldObject {
	public:
		FieldObject() {}
		FieldObject(FieldObject&& other) = default;
		virtual ~FieldObject() = default;

		vec<uptr<AttributeObject>> statuses;
	};
}