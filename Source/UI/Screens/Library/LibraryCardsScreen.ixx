export module fab.LibraryCardsScreen;

import fab.Card;
import fab.CardData;
import fab.CardDataRenderable;
import fab.CardRenderable;
import fab.CoreContent;
import fab.FUtil;
import fab.FWindow;
import fab.UIScrollGrid;
import fab.UIScreen;
import std;
import fab.ScreenSizeHitbox;
import fab.UIButton;
import fab.UITextButton;
import fab.ScreenPosHitbox;

namespace fab {
	export class LibraryCardsScreen : public UIScreen {
	public:
		LibraryCardsScreen(FWindow& win, CoreContent& cct) : UIScreen(win),
			cct(cct),
			grid(addNew<UIScrollGrid<CardDataRenderable>>(make_unique<ScreenSizeHitbox>(win, 0.25f, 0.25f, 0.5f, 0.5f), CARD_W, CARD_H))
		{
			add(cct.makeBackButton(win));

			reloadItems();
		}
		virtual ~LibraryCardsScreen() = default;

		void reloadItems();
	private:
		CoreContent& cct;
		UIScrollGrid<CardDataRenderable>& grid;
		vec<uptr<Card>> cards;
	};

	void LibraryCardsScreen::reloadItems() {
		grid.clear();
		cards.clear();
		for (CardData* data : CardData::all()) {
			grid.addNew<CardDataRenderable>(*grid.hb, *data);
		}
	}
}