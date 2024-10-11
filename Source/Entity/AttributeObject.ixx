export module fab.AttributeObject;

namespace fab {
	export class AttributeObject {
	public:
		AttributeObject() {}
		AttributeObject(AttributeObject&& other) = default;
		virtual ~AttributeObject() = default;

		virtual void act() = 0;
	};
}