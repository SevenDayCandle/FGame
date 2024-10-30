export module fab.AttributeObject;

/*
 * Object that can be applied to game objects to modify them
 */
namespace fab {
	export class AttributeObject {
	public:
		AttributeObject() {}
		AttributeObject(AttributeObject&& other) = default;
		virtual ~AttributeObject() = default;

		virtual void act() = 0;
	};
}