export module fab.LibraryMainScreen;

import fab.CoreContent;
import fab.FUtil;
import fab.FWindow;
import fab.LibraryCardsScreen;
import fab.UIScreen;
import std;
import fab.UITextButton;
import fab.ScreenPosHitbox;
import fab.UIButton;

namespace fab {
	export class LibraryMainScreen : public UIScreen {
	public:
		LibraryMainScreen(FWindow& window, CoreContent& cct): UIScreen(window) {
			add(cct.makeBackButton(window));

			UITextButton& b = addNew<UITextButton>(
				make_unique<ScreenPosHitbox>(win, 0.5f, 0.5f, 290, 170),
				cct.images.uiPanel,
				cct.fontRegular(),
				cct.strings.screen_title_header());
			b.setOnClick([this, &cct](UIButton& i) {
				win.openNewScreen<LibraryCardsScreen>(cct);
			});
		}
		virtual ~LibraryMainScreen() = default;
	};
}