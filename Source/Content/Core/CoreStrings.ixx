export module fab.CoreStrings;

import fab.BaseContent;
import fab.BaseStrings;
import fab.CoreConfig;
import fab.FUtil;
import fab.KeywordStrings;
import std;

namespace fab {
	export class CoreStrings : public BaseStrings {
	public:
		CoreStrings(BaseContent& cnt) : BaseStrings(cnt) {}

		void initialize() override;

		inline strv combat_end_turn() const { return uiStrings.at("combat_end_turn"); };
		inline strv combat_pile_discard() const { return uiStrings.at("combat_pile_discard"); };
		inline strv combat_pile_draw() const { return uiStrings.at("combat_pile_draw"); };
		inline strv editor_copy() const { return uiStrings.at("editor_copy"); };
		inline strv editor_delete() const { return uiStrings.at("editor_delete"); };
		inline strv editor_duplicate() const { return uiStrings.at("editor_duplicate"); };
		inline strv editor_effects() const { return uiStrings.at("editor_effects"); };
		inline strv editor_load_image() const { return uiStrings.at("editor_load_image"); };
		inline strv editor_new() const { return uiStrings.at("editor_new"); };
		inline strv editor_paste() const { return uiStrings.at("editor_paste"); };
		inline strv editor_properties() const { return uiStrings.at("editor_properties"); };
		inline strv editor_reload() const { return uiStrings.at("editor_reload"); };
		inline strv editor_undo() const { return uiStrings.at("editor_undo"); };
		inline strv editor_value() const { return uiStrings.at("editor_value"); };
		inline strv options_graphics_fps() const { return uiStrings.at("options_graphics_fps"); };
		inline strv options_graphics_resolution() const { return uiStrings.at("options_graphics_resolution"); };
		inline strv options_graphics_vsync() const { return uiStrings.at("options_graphics_vsync"); };
		inline strv options_graphics_window_mode() const { return uiStrings.at("options_graphics_window_mode"); };
		inline strv options_graphics_window_mode_borderless() const { return uiStrings.at("options_graphics_window_mode_borderless"); };
		inline strv options_graphics_window_mode_fullscreen() const { return uiStrings.at("options_graphics_window_mode_fullscreen"); };
		inline strv options_graphics_window_mode_windowed() const { return uiStrings.at("options_graphics_window_mode_windowed"); };
		inline strv options_sound_effects() const { return uiStrings.at("options_sound_effects"); };
		inline strv options_sound_master() const { return uiStrings.at("options_sound_master"); };
		inline strv options_sound_music() const { return uiStrings.at("options_sound_music"); };
		inline strv options_section_graphics() const { return uiStrings.at("options_section_graphics"); };
		inline strv options_section_hotkeys() const { return uiStrings.at("options_section_hotkeys"); };
		inline strv options_section_sound() const { return uiStrings.at("options_section_sound"); };
		inline strv options_section_text() const { return uiStrings.at("options_section_text"); };
		inline strv screen_library_cards_header() const { return uiStrings.at("screen_library_cards_header"); };
		inline strv screen_library_header() const { return uiStrings.at("screen_library_header"); };
		inline strv screen_setup_header() const { return uiStrings.at("screen_setup_header"); };
		inline strv screen_title_header() const { return uiStrings.at("screen_title_header"); };
		inline strv ui_apply() const { return uiStrings.at("ui_apply"); };
		inline strv ui_back() const { return uiStrings.at("ui_back"); };
		inline strv ui_cancel() const { return uiStrings.at("ui_cancel"); };
		inline strv ui_close() const { return uiStrings.at("ui_close"); };
		inline strv ui_confirm() const { return uiStrings.at("ui_confirm"); };
		inline strv ui_items() const { return uiStrings.at("ui_items"); };
		inline strv ui_items(int val) const { return std::vformat(ui_items(), std::make_format_args(val)); };
		inline strv ui_save() const { return uiStrings.at("ui_save"); };
	private:
		strumap<str> uiStrings;
	};

	// Core contains UI strings, keyword strings
	void CoreStrings::initialize()
	{
		loadUIStrings(uiStrings);
	}
}