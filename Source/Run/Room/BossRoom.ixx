export module fab.BossRoom;

import fab.FUtil;
import fab.RunEncounter;
import fab.RunRoom;
import fab.RunZone;
import std;

namespace fab {
	export class BossRoom : public RunRoom::RoomType {
	public:
		BossRoom() : RunRoom::RoomType(ID, 0) {}

		static constexpr strv ID = "BOSS";
		static const BossRoom instance;

		inline virtual bool isEncounterValid(RunEncounter* encounter) override { return encounter->data.boss;}
	};

	const BossRoom BossRoom::instance = BossRoom();
}