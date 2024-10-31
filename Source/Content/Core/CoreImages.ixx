export module fab.CoreImages;

import fab.BaseContent;
import fab.FTexture;
import fab.FUtil;
import fab.RBordered;
import fab.RHorizontal;
import fab.RVertical;
import fab.StaticLoadables;

namespace fab {
	export class CoreImages : public StaticLoadables {
	public:
		CoreImages(BaseContent& cnt) : StaticLoadables(cnt) {}

		FTexture& combatArrowMove = cacheTexture("Combat/Arrow_Move.png");
		FTexture& combatGridRegular = cacheTexture("Combat/Grid_Regular.png");

		FTexture& uiArrowIncrement = cacheTexture("UI/Arrow_Increment.png");
		FTexture& uiArrowLarge = cacheTexture("UI/Arrow_Large.png");
		FTexture& uiArrowSmall = cacheTexture("UI/Arrow_Small.png");
		FTexture& uiCheckboxEmpty = cacheTexture("UI/Checkbox_Empty.png");
		FTexture& uiCheckboxFilled = cacheTexture("UI/Checkbox_Filled.png");
		FTexture& uiClearLarge = cacheTexture("UI/Clear_Large.png");
		FTexture& uiClearSmall = cacheTexture("UI/Clear_Small.png");
		RBordered& uiDarkPanel = cacheBordered("UI/DarkPanelBase.png");
		RBordered& uiDarkPanelRound = cacheBordered("UI/DarkPanelRoundBase.png");
		RBordered& uiLargePanel = cacheBordered("UI/LargePanelBase.png");
		RBordered& uiLightPanel = cacheBordered("UI/LightPanelBase.png");
		RBordered& uiLightPanelRound = cacheBordered("UI/LightPanelRoundBase.png");
		RBordered& uiPanel = cacheBordered("UI/PanelBase.png");
		RBordered& uiPanelRound = cacheBordered("UI/PanelRoundBase.png");
		RBordered& uiThickPanel = cacheBordered("UI/ThickPanelBase.png");
		RHorizontal& uiSliderEmpty = cacheHorizontal("UI/SliderEmptyBase.png");
		RHorizontal& uiSliderFilled = cacheHorizontal("UI/SliderFilledBase.png");
		RVertical& uiScrollbar = cacheVertical("UI/ScrollbarBase.png");
		RVertical& uiScrollbutton = cacheVertical("UI/ScrollbuttonBase.png");
	};
}