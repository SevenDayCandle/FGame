export module fab.CreatureTurnStatsDisplay;

import fab.CoreContent;
import fab.Creature;
import fab.EmptyDrawable;
import fab.FUtil;
import fab.FWindow;
import fab.Hitbox;
import fab.IDrawable;
import fab.OccupantObject;
import fab.TextDrawable;
import fab.UIDialog;
import fab.UIImage;
import fab.UIInteractable;
import sdl.SDLBase; 
import sdl.SDLBatchRenderPass;
import sdl.SDLRunner;
import std;

namespace fab {
	export class CreatureTurnStatsDisplay : public UIDialog {
	public:
		CreatureTurnStatsDisplay(FWindow& window, uptr<Hitbox> hb): UIDialog(window, move(hb), window.props.defaultBackground()),
			energyText(window.props.fontSmall()),
			hpText(window.props.fontSmall()),
			moveText(window.props.fontSmall()),
			nameText(window.props.fontSmall())
		{
			enabled = false;
		}
		virtual ~CreatureTurnStatsDisplay() = default;

		const Creature* creature;

		CreatureTurnStatsDisplay& setCreature(Creature* creature);
		void refreshDisplay();
		void refreshEnergyText();
		void refreshHpText();
		void refreshMoveText();
		void renderImpl(sdl::SDLBatchRenderPass& rp) override;
	private:
		TextDrawable energyText;
		TextDrawable hpText;
		TextDrawable moveText;
		TextDrawable nameText;
	};

	CreatureTurnStatsDisplay& CreatureTurnStatsDisplay::setCreature(Creature* creature) {
		if (this->creature != creature) {
			this->creature = creature;
			refreshDisplay();
		}
		return *this;
	}

	void CreatureTurnStatsDisplay::refreshDisplay() {
		this->enabled = creature;
		if (creature) {
			nameText.setText(creature->name());
			refreshEnergyText();
			refreshHpText();
			refreshMoveText();
		}
	}

	void CreatureTurnStatsDisplay::refreshEnergyText() {
		if (creature) {
			energyText.setText(std::to_string(creature->energy) + "/" + std::to_string(creature->energyMax));
		}
	}

	void CreatureTurnStatsDisplay::refreshHpText() {
		if (creature) {
			hpText.setText(std::to_string(creature->health) + "/" + std::to_string(creature->healthMax));
		}
	}

	void CreatureTurnStatsDisplay::refreshMoveText() {
		if (creature) {
			moveText.setText(std::to_string(creature->movement) + "/" + std::to_string(creature->movementMax));
		}
	}

	void CreatureTurnStatsDisplay::renderImpl(sdl::SDLBatchRenderPass& rp) {
		UIDialog::renderImpl(rp);
		nameText.drawOffsetScaled(rp, *hb, 20, 12);
		energyText.drawOffsetScaled(rp, *hb, 20, 90);
		moveText.drawOffsetScaled(rp, *hb, 20, 168);
		hpText.drawOffsetScaled(rp, *hb, 20, 246);
	}
}