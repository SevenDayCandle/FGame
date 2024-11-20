export module fab.CombatScreen;

import fab.CombatInstance;
import fab.CallbackVFX;
import fab.Card;
import fab.CardMoveAction;
import fab.CardUseAction;
import fab.CardPile;
import fab.CardRenderable;
import fab.CombatInstance;
import fab.CombatSquare;
import fab.CombatSquareRenderable;
import fab.CombatTurnRenderable;
import fab.CoreContent;
import fab.CreatureMoveAction;
import fab.CreatureRenderable;
import fab.FUtil;
import fab.FWindow;
import fab.Hoverable;
import fab.PileReshuffleAction;
import fab.PileType;
import fab.RelativeHitbox;
import fab.UICanvas;
import fab.UIScreen;
import fab.UITextButton;
import sdl.SDLBase;
import std;

namespace fab {
	constexpr float CARD_HAND_POS_X_PCT = 0.25f;
	constexpr float CARD_HAND_POS_Y_DIFF = 100;
	constexpr float CARD_PLAY_POS_X_PCT = 0.6f;
	constexpr float CARD_PLAY_POS_Y_PCT = 0.45f;
	constexpr float END_TURN_SIZE = 300;
	constexpr float PILE_SIZE = 150;
	constexpr float TILE_OFFSET = 300;
	constexpr float TILE_SIZE = 128;
	constexpr float TURN_W = 280;
	constexpr sdl::Color COLOR_ARROW = { 1.2f, 1.7, 0.8f, 1 };
	constexpr sdl::Color COLOR_HIGHLIGHT_PATH = { 1.0f, 1.5, 1.9f, 1 };
	constexpr sdl::Color COLOR_HIGHLIGHT_TARGET = { 1.9f, 1.5, 1.0f, 1 };

	export class CombatScreen : public UIScreen, 
		public CombatInstance::IViewListener, 
		public CardMoveAction::Listener, 
		public CardUseAction::Listener,
		public CreatureMoveAction::Listener {
	public:
		CombatScreen(FWindow& window, CoreContent& cct) : UIScreen(window), cct(cct),
			cardUI(addNew<UICanvas<CardRenderable>>(relhb(hb->getScaledOffSizeX(CARD_HAND_POS_X_PCT), hb->getScaledOffSizeY() - CARD_HAND_POS_Y_DIFF - CARD_H, hb->getScaledOffSizeX(), hb->getScaledOffSizeY()))),
			fieldUI(addNew<UICanvas<CombatSquareRenderable>>(relhb(TILE_OFFSET, TILE_OFFSET, hb->getScaledOffSizeX(), hb->getScaledOffSizeY()))),
			turnUI(addNew<UICanvas<CombatTurnRenderable>>(relhb(0, 0, TILE_SIZE, hb->getScaledOffSizeY()))),
			occupantUI(addNew<UICanvas<CreatureRenderable>>(fieldUI.relhb(0, 0, hb->getScaledOffSizeX(), hb->getScaledOffSizeY()))),
			discardPileButton(addNew<UITextButton>(relhb(hb->getScaledOffSizeX(0.8), hb->getScaledOffSizeY(0.6), PILE_SIZE, PILE_SIZE), window.props.defaultButton(), window.props.fontRegular(), cct.strings.combat_pile_discard())),
			drawPileButton(addNew<UITextButton>(relhb(hb->getScaledOffSizeX(0.8), hb->getScaledOffSizeY(0.7), PILE_SIZE, PILE_SIZE), window.props.defaultButton(), window.props.fontRegular(), cct.strings.combat_pile_draw())),
			endTurnButton(addNew<UITextButton>(relhb(hb->getScaledOffSizeX(0.8), hb->getScaledOffSizeY(0.8), END_TURN_SIZE, END_TURN_SIZE), window.props.defaultButton(), window.props.fontRegular(), cct.strings.combat_end_turn())) {}

		CoreContent& cct;

		inline CombatSquareRenderable* getSquareRender(const CombatSquare* square) { return fieldUI.at(instance->getSquareIndex(square->col, square->row)); }
		inline uptr<CardRenderable> createLooseCard(const Card& card) { return make_unique<CardRenderable>(win, *cardUI.hb, card, 0, -CARD_H); }

		CardRenderable& createCardUIRender(const Card& card, float sOffX = 0, float sOffY = CARD_H * 2);
		CombatTurnRenderable& createTurnRender(const CombatInstance::Turn& turn);
		CreatureRenderable& createOccupantRender(const OccupantObject& occupant);
		float getRotationFromFacing(const CombatSquare& src, const CombatSquare& dst) const;
		uptr<CallbackVFX> cardMoveVFX(const Card& card, const CardPile& type, bool isManual) override;
		uptr<CallbackVFX> cardUseVFX(const Card& card, const CombatSquare& target) override;
		uptr<CallbackVFX> creatureMoveVFX(const OccupantObject* occupant, const CombatSquare* target) override;
		uptr<CardRenderable> removeCardRender(const Card* card);
		uptr<CardRenderable> removeCardRender(CardRenderable* card);
		virtual void onActionBegin(const CombatInstance::Action* act) override;
		virtual void onActionEnd(const CombatInstance::Action* act, bool isLast) override;
		virtual void onPlayerTurnBegin(const CombatInstance::Turn* turn) override;
		virtual void onPlayerTurnEnd(const CombatInstance::Turn* turn) override;
		virtual void onTurnAdded(const CombatInstance::Turn& turn) override;
		virtual void onTurnChanged(ref_view<const mset<CombatInstance::Turn>> turns) override;
		virtual void onTurnRemoved(const CombatInstance::Turn* turn) override;
		void clearHighlights();
		void clearSelectedPath();
		void hoverSquareUpdate(CombatSquareRenderable* newHovered);
		void open() override;
		void previewMovement(CombatSquare* object, const sdl::Color& color, const sdl::Color& highlightColor, int movementRange);
		void previewTargeting(CombatSquare* object, const sdl::Color& color, const sdl::Color& highlightColor, int highlightRangeBegin, int highlightRangeEnd, int targetSizeX, int targetSizeY);
		void queueCard(CardRenderable& card, CombatSquareRenderable& square);
		void queueMove(CombatSquareRenderable& square);
		void recolorSquare(CombatSquareRenderable& square, const sdl::Color& color);
		void refreshCardLayout();
		void resetHighlights();
		void selectCardRender(CardRenderable* card);
		void selectSquare(CombatSquareRenderable* square);
		void setSelectedPath(vec<CombatSquare*>&& squares);
		void updateImpl() override;
	private:
		Hoverable& uiForPile(const PileType& type);

		CardRenderable* selectedCard;
		CombatInstance* instance;
		CombatSquareRenderable* hovered;
		const sdl::Color* hoverColor = &sdl::COLOR_STANDARD;
		int targetSizeX;
		int targetSizeY;
		OccupantObject* activeOccupant;
		UITextButton& discardPileButton;
		UITextButton& drawPileButton;
		UITextButton& endTurnButton;
		UICanvas<CardRenderable>& cardUI;
		UICanvas<CombatSquareRenderable>& fieldUI;
		UICanvas<CombatTurnRenderable>& turnUI;
		UICanvas<CreatureRenderable>& occupantUI;
		umap<const Card*, CardRenderable*> cardUIMap;
		umap<const CombatInstance::Turn*, CombatTurnRenderable*> turnUIMap;
		umap<const OccupantObject*, CreatureRenderable*> occupantUIMap;
		vec<CombatSquare*> selectedPath;
	};
}