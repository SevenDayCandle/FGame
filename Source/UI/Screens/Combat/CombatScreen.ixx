export module fab.CombatScreen;

import fab.CallbackVFX;
import fab.Card;
import fab.CardRenderable;
import fab.CombatInstance;
import fab.CombatSquare;
import fab.CombatSquareRenderable;
import fab.CombatTurn;
import fab.CombatTurnRenderable;
import fab.CoreContent;
import fab.CreatureRenderable;
import fab.FUtil;
import fab.FWindow;
import fab.RelativeHitbox;
import fab.UICanvas;
import fab.UIScreen;
import fab.UITextButton;
import sdl.SDLBase;
import std;

namespace fab {
	constexpr float CARD_HAND_POS_X_PCT = 0.25f;
	constexpr float CARD_HAND_POS_Y_DIFF = 100;
	constexpr float END_TURN_SIZE = 300;
	constexpr float TILE_OFFSET = 300;
	constexpr float TILE_SIZE = 128;
	constexpr float TURN_W = 280;

	export class CombatScreen : public UIScreen, public CombatInstance::IViewSubscriber {
	public:
		CombatScreen(FWindow& window, CoreContent& cct) : UIScreen(window), cct(cct),
			cardUI(addNew<UICanvas<CardRenderable>>(relhb(hb->getScaleOffSizeX()* CARD_HAND_POS_X_PCT, hb->getScaleOffSizeY() - CARD_HAND_POS_Y_DIFF - CARD_H, hb->getScaleOffSizeX(), hb->getScaleOffSizeY()))),
			fieldUI(addNew<UICanvas<CombatSquareRenderable>>(relhb(TILE_OFFSET, TILE_OFFSET, hb->getScaleOffSizeX(), hb->getScaleOffSizeY()))),
			turnUI(addNew<UICanvas<CombatTurnRenderable>>(relhb(0, 0, TILE_SIZE, hb->getScaleOffSizeY()))),
			occupantUI(addNew<UICanvas<CreatureRenderable>>(fieldUI.relhb(0, 0, hb->getScaleOffSizeX(), hb->getScaleOffSizeY()))),
			endTurnButton(add(make_unique<UITextButton>(win, relhb(hb->getScaleOffSizeX(0.8), hb->getScaleOffSizeY(0.8), END_TURN_SIZE, END_TURN_SIZE), window.props.defaultButton(), window.props.fontRegular(), cct.strings.combat_end_turn()))) {}

		CoreContent& cct;

		inline CombatSquareRenderable* getSquareRender(const CombatSquare* square) { return fieldUI.at(instance->getSquareIndex(square->col, square->row)); }

		CardRenderable& createCardRender(const Card& card, float tOffX, float sOffX = 0, float sOffY = CARD_H * 2);
		CombatTurnRenderable& createTurnRender(const CombatTurn& turn);
		CreatureRenderable& createOccupantRender(const OccupantObject& occupant);
		float getRotationFromFacing(const CombatSquare& src, const CombatSquare& dst) const;
		virtual uptr<CallbackVFX> creatureMoveVFX(const OccupantObject* occupant, const CombatSquare* target) override;
		virtual void onPlayerTurnBegin(const CombatTurn* turn) override;
		virtual void onPlayerTurnEnd(const CombatTurn* turn) override;
		virtual void onTurnAdded(const CombatTurn& turn) override;
		virtual void onTurnChanged(ref_view<const mset<CombatTurn>> turns) override;
		virtual void onTurnRemoved(const CombatTurn* turn) override;
		void clearHighlights();
		void clearSelectedPath();
		void hoverSquareUpdate(CombatSquareRenderable* newHovered);
		void open() override;
		void previewMovement(CombatSquare* object, const sdl::Color& color, int movementRange);
		void previewTargeting(CombatSquare* object, const sdl::Color& color, int highlightRangeBegin, int highlightRangeEnd, int targetSizeX, int targetSizeY);
		void queueMove(CombatSquareRenderable& square);
		void recolorSquare(CombatSquareRenderable& square, const sdl::Color& color);
		void removeCardRender(Card* card);
		void removeCardRender(CardRenderable* card);
		void resetHighlights();
		void selectCardRender(CardRenderable* card);
		void selectSquare(CombatSquareRenderable* square);
		void setSelectedPath(vec<const CombatSquare*>&& squares);
		void updateImpl() override;
	private:
		CardRenderable* selectedCard;
		CombatInstance* instance;
		CombatSquareRenderable* hovered;
		const sdl::Color* hoverColor = &sdl::COLOR_STANDARD;
		int targetSizeX;
		int targetSizeY;
		OccupantObject* activeOccupant;
		UITextButton& endTurnButton;
		UICanvas<CardRenderable>& cardUI;
		UICanvas<CombatSquareRenderable>& fieldUI;
		UICanvas<CombatTurnRenderable>& turnUI;
		UICanvas<CreatureRenderable>& occupantUI;
		umap<const Card*, CardRenderable*> cardUIMap;
		umap<const CombatTurn*, CombatTurnRenderable*> turnUIMap;
		umap<const OccupantObject*, CreatureRenderable*> occupantUIMap;
		vec<const CombatSquare*> selectedPath;
	};
}