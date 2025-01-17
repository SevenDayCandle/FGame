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
		CreatureInfoDisplay(FWindow& window, uptr<Hitbox> hb) : UIDialog(window, move(hb), window.props.defaultBackground()), text(window.props.fontRegular()) {
		}

		const OccupantObject* creature;
		
		CreatureInfoDisplay& setCreature(OccupantObject* creature);
	private:
		UIImage& portrait = this->addNew<UIImage>(relhb(0, hb->getScaledOffSizeY() * 0.88f, 300, 100), EMPTY);
		TextDrawable text;
	};

	CreatureInfoDisplay& CreatureInfoDisplay::setCreature(OccupantObject* creature) {
		this->creature = creature;
		this->enabled = creature;
		if (creature) {
			portrait.setImage(&creature->getImagePortrait());
			this->text.setText(creature->name());
		}
		return *this;
	}
}