
import fab.FEffect;
import fab.FieldObject;
import fab.GameObject;
import fab.FVariable;
import fab.FUtil;

module fab.FMove;

namespace fab {
	FMove& FMove::addChild(uptr<FMove>&& child) {
		FMove& ref = *child;
		if (ref.setParent(this)) {
			children.push_back(move(child));
			if (owner) {
				ref.setOwner(owner);
			}
		}
		return *this;
	}

	FMove& FMove::setOwner(GameObject* owner) {
		this->owner = owner;
		for (uptr<FMove>& child : children) {
			child->setOwner(owner);
		}
		return *this;
	}
}