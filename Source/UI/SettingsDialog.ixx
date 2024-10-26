export module fab.SettingsDialog;

import fab.CoreConfig;
import fab.CoreContent;
import fab.FUtil;
import fab.FWindow;
import fab.Hitbox;
import fab.RelativeHitbox;
import fab.ScreenSizeHitbox;
import fab.SettingsDialogPage;
import fab.UIButton;
import fab.UIDialog;
import fab.UIEntry;
import fab.UINavigation;
import fab.UITextButton;
import fab.WindowMode;
import std;

namespace fab {
	export class SettingsDialog : public UIDialog {
	public:
		SettingsDialog(FWindow& window, CoreContent& cct): UIDialog(window, make_unique<ScreenSizeHitbox>(window, 0.25, 0.25, 0.5, 0.5), window.props.defaultBackground()), cct(cct) {
			graphics.addDropdown<pair<int,int>, ilist<pair<int,int>>>(cct.cfg.graphicsResolution, cct.strings.options_graphics_resolution(), RESOLUTIONS, [](const pair<int,int>& item) { return futil::dimensionString(item); });
			graphics.addDropdown<WindowMode, ilist<WindowMode>>(cct.cfg.graphicsWindowMode, cct.strings.options_graphics_window_mode(), WINDOWMODE_ALL, [this](const WindowMode& item) { return str(windowScreenName(item)); });
			graphics.addSlider(cct.cfg.graphicsFPS, cct.strings.options_graphics_fps());
			graphics.addToggle(cct.cfg.graphicsVSync, cct.strings.options_graphics_vsync());

			sound.addSlider(cct.cfg.soundVolumeMaster, cct.strings.options_sound_master(), 300);
			sound.addSlider(cct.cfg.soundVolumeEffects, cct.strings.options_sound_effects(), 300);
			sound.addSlider(cct.cfg.soundVolumeMusic, cct.strings.options_sound_music(), 300);

			navigation.addItems(&graphics, &sound, &text);

			UIButton& cancel = this->add(std::make_unique<UITextButton>(win,
				relhb(0, hb->getScaledOffSizeY() * 0.88f, 300, 100),
				cct.images.uiPanelRound,
				cct.fontRegular(),
				cct.strings.ui_cancel())
			).setOnClick([this](UIButton& b) {this->win.closeOverlay(this);});
			UIButton& apply = this->stackXDir(std::make_unique<UITextButton>(win,
				relhb(300, 100),
				cct.images.uiPanelRound,
				cct.fontRegular(),
				cct.strings.ui_apply())
			).setOnClick([this](UIButton& b) { applyAll(); });
			UIButton& save = this->stackXDir(std::make_unique<UITextButton>(win,
				relhb(300, 100),
				cct.images.uiPanelRound,
				cct.fontRegular(),
				cct.strings.ui_save())
			).setOnClick([this](UIButton& b) {
					applyAll();
					this->win.closeOverlay(this);
				});
		}

		CoreContent& cct;
		SettingsDialogPage graphics = page(cct.strings.options_section_graphics());
		SettingsDialogPage sound = page(cct.strings.options_section_sound());
		SettingsDialogPage text = page(cct.strings.options_section_text());
		SettingsDialogPage hotkeys = page(cct.strings.options_section_graphics());
		UINavigation<SettingsDialogPage>& navigation = this->add(std::make_unique<UINavigation<SettingsDialogPage>>(win, relhb(-330, 0, 300, 100)));

		virtual bool isHovered() override;
		strv windowScreenName(WindowMode mode);

		inline static void openNew(FWindow& window, CoreContent& cct) { window.openOverlay(std::make_unique<SettingsDialog>(window, cct)); } // Displays a new Settings Dialog
	protected:
		inline SettingsDialogPage page(strv name) { return SettingsDialogPage(win, cct, relhb(hb->w * 0.1f, hb->h * 0.1f, hb->w * 0.8f, hb->h * 0.7f), name); }

		void applyAll();
		void resetCurrent();
	};

	bool SettingsDialog::isHovered()
	{
		return UIDialog::isHovered() || navigation.isHovered();
	}

	void SettingsDialog::applyAll()
	{
		for (const UIEntry<SettingsDialogPage>& page : navigation) {
			SettingsDialogPage& item = const_cast<SettingsDialogPage&>(page.item);
			item.commit();
		}

		cct.cfg.commit();
	}

	void SettingsDialog::resetCurrent()
	{
		SettingsDialogPage* selected = navigation.getSelectedItem();
		if (selected) {
			selected->reset();
		}
	}

	strv SettingsDialog::windowScreenName(WindowMode mode)
	{
		switch (mode) {
		case BORDERLESS_FULLSCREEN:
			return cct.strings.options_graphics_window_mode_borderless();
		case FULLSCREEN:
			return cct.strings.options_graphics_window_mode_fullscreen();
		default:
			return cct.strings.options_graphics_window_mode_windowed();
		}
	}
}