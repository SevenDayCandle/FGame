export module fab.CombatRoom;

import fab.FUtil;
import fab.FWindow;
import fab.GameRun;
import fab.RunRoom;
import fab.RunZone;
import std;

namespace fab {
	export class CombatRoom : public RunRoom::RoomType {
	public:
		CombatRoom(): RunRoom::RoomType(ID, 45) {}

		static constexpr strv ID = "COMBAT";
		static const CombatRoom instance;

		void onEnter() override;
	};

	const CombatRoom CombatRoom::instance = CombatRoom();

	// Start combat upon entering
	void CombatRoom::onEnter()
	{
		GameRun::current->startCombat();
	}
}