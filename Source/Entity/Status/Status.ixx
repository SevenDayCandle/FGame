export module fab.Status;

import fab.AttributeObject;
import fab.Effect;
import fab.FUtil;
import fab.GameObject;
import fab.StatusData;
import sdl.SDLBase;
import sdl.SDLRunner;
import std;

namespace fab {
	export class Status : public GameObjectD<StatusData>, public AttributeObject {
	public:
		Status(StatusData& data): GameObjectD<StatusData>(data) {}

		int stacks;
		int turns;
	};
}