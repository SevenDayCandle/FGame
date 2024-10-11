export module fab.EventRoom;

import fab.FUtil;
import fab.GameRun;
import fab.RunRoom;
import fab.RunZone;
import std;

namespace fab {
	export class EventRoom : public RunRoom::RoomType {
	public:
		EventRoom() : RunRoom::RoomType(ID, 25) {}

		static constexpr strv ID = "EVENT";
		static const EventRoom instance;
	};

	const EventRoom EventRoom::instance = EventRoom();
}