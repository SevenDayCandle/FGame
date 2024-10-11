export module fab.RunScreen;

import fab.CoreContent;
import fab.FWindow;
import fab.GameRun;
import fab.RelativeHitbox;
import fab.RunRoom;
import fab.SettingsDialog;
import fab.UIButton;
import fab.UIScreen;
import fab.UITextButton;
import fab.FUtil;
import std;

namespace fab {
	constexpr float TILE_OFFSET = 300;
	constexpr float ROOM_SIZE = 128;

	export class RunScreen : public UIScreen {
	public:
		RunScreen(FWindow& window, CoreContent& cct): UIScreen(window), cct(cct) {}

		CoreContent& cct;

		void open() override;
	private:
		uptr<UIButton> makeRoomButton(RunRoom& room);
		void onRoomClick(RunRoom& room);
	};
}