export module fab.Passive;

import fab.AttributeObject;
import fab.FEffect;
import fab.FUtil;
import fab.GameObject;
import fab.PassiveData;
import sdl.SDLBase;
import sdl.SDLRunner;
import std;

namespace fab {
	export class Passive : public GameObjectD<PassiveData>, public AttributeObject {
	public:
		Passive(PassiveData& data, int upgrades = 0) : GameObjectD<PassiveData>(data), upgrades(upgrades) {}

		int upgrades;

		virtual void act() {}
	};
}