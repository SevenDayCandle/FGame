export module fab.CreatureInfoDisplay;

import fab.CoreContent;
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
	export class CreatureInfoDisplay : public UIDialog {
	public:
		CreatureInfoDisplay(FWindow& window, uptr<Hitbox> hb) : UIDialog(window, move(hb),
			window.props.defaultBackground()),
			nameText(window.props.fontRegular()),
			hpText(window.props.fontSmall()) 
		{
			enabled = false;
		}

		const OccupantObject* creature;
		
		CreatureInfoDisplay& setCreature(OccupantObject* creature);
		void refreshDisplay();
		void renderImpl(sdl::SDLBatchRenderPass& rp) override;
	private:
		UIImage& portrait = this->addNew<UIImage>(relhb(15, hb->getScaledOffSizeY(0.15f), 300, 200), EMPTY);
		TextDrawable hpText;
		TextDrawable nameText;
	};

	CreatureInfoDisplay& CreatureInfoDisplay::setCreature(OccupantObject* creature) {
		if (this->creature != creature) {
			this->creature = creature;
			refreshDisplay();
		}
		return *this;
	}

	void CreatureInfoDisplay::refreshDisplay() {
		this->enabled = creature;
		if (creature) {
			portrait.setImage(&creature->getImagePortrait());
			nameText.setText(creature->name());
			hpText.setText(std::to_string(creature->health) + "/" + std::to_string(creature->healthMax));
		}
	}

	void CreatureInfoDisplay::renderImpl(sdl::SDLBatchRenderPass& rp) {
		UIDialog::renderImpl(rp);
		nameText.drawOffsetScaled(rp, *hb, 340, 12);
		hpText.drawOffsetScaled(rp, *hb, 340, 90);
	}
}