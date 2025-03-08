export module fab.TitleScreen;

import fab.CoreContent;
import fab.FWindow;
import fab.LibraryMainScreen;
import fab.RunScreen;
import fab.ScreenPosHitbox;
import fab.ScreenSizeHitbox;
import fab.SettingsDialog;
import fab.UIButton;
import fab.UIScreen;
import fab.UITextButton;
import fab.FUtil;
import std;

namespace fab {
	export class TitleScreen : public UIScreen {
	public:
		TitleScreen(FWindow& window, CoreContent& cct): UIScreen(window, make_unique<ScreenSizeHitbox>(window)) {
			UITextButton& b = addNew<UITextButton>(
				make_unique<ScreenPosHitbox>(window, 0.5f, 0.5f, 290, 170),
				cct.images.uiPanel,
				cct.fontRegular(),
				cct.strings.screen_title_header());
			b.setOnClick([this, &cct](UIButton& i) {
				SettingsDialog::openNew(win, cct);
				});

			UITextButton& b2 = stackYDir(std::make_unique<UITextButton>(window,
				make_unique<ScreenPosHitbox>(window, 0.5f, 0.5f, 290, 170),
				cct.images.uiPanel,
				cct.fontRegular(),
				cct.strings.screen_setup_header()));
			b2.setOnClick([this, &cct](UIButton& i) {
				win.openNewScreen<RunScreen>(cct);
			});

			UITextButton& b3 = stackYDir(std::make_unique<UITextButton>(window,
				make_unique<ScreenPosHitbox>(window, 0.5f, 0.5f, 290, 170),
				cct.images.uiPanel,
				cct.fontRegular(),
				cct.strings.screen_library_header()));
			b3.setOnClick([this, &cct](UIButton& i) {
				win.openNewScreen<LibraryMainScreen>(cct);
			});
		}
	};
}