module;

import fab.Action;
import fab.CardRenderable;
import fab.CardUseAction;
import fab.CombatInstance;
import fab.CombatSquare;
import fab.CombatSquareRenderable;
import fab.CombatTurn;
import fab.CombatTurnRenderable;
import fab.Creature;
import fab.CreatureMoveAction;
import fab.CreaturePathAction;
import fab.CreatureRenderable;
import fab.GameRun;
import fab.IDrawable;
import fab.ImageRenderVFX;
import fab.RelativeHitbox;
import fab.RunZone;
import fab.SequentialRenderVFX;
import fab.UIButton;
import fab.UIGrid;
import fab.UIRecolorVFX;
import fab.UITransformVFX;
import fab.VFXAction;
import sdl.SDLRunner;

module fab.CombatScreen;

namespace fab {

	void CombatScreen::onPlayerTurnBegin(const CombatTurn* turn)
	{
		endTurnButton.setEnabled(true);
		Creature* creature = dynamic_cast<Creature*>(&turn->source);
		if (creature) {
			activeOccupant = creature;
			int i = 0;
			for (const uptr<Card>& card : creature->pile.hand) {
				auto res = cardUIMap.find(card.get());
				if (res == cardUIMap.end()) {
					CardRenderable& render = createCardUIRender(*card);
					addVfxNew<UITransformVFX>(render)
						.setFade(0, 1)
						.setMoveByOffset(i * CARD_W, 0)
						.setInterpCubic();
				}
				++i;
			}
		}
		else {
			activeOccupant = dynamic_cast<OccupantObject*>(&turn->source);
		}
		resetHighlights();
	}

	void CombatScreen::onPlayerTurnEnd(const CombatTurn* turn)
	{
		activeOccupant = nullptr;
		endTurnButton.setInteractable(false).setEnabled(false);
		while (!cardUI.empty()) {
			uptr<CardRenderable> item = removeCardRender(cardUI.at(0));
			if (item) {
				addVfxNew<ImageRenderVFX>(move(item))
					.setScale(0)
					.setMoveRelative(0, -10)
					.setInterpCubic();
			}
		}
		selectCardRender(nullptr);
		clearSelectedPath();
		resetHighlights();
		cardUIMap.clear();
	}

	void CombatScreen::onTurnAdded(const CombatTurn& turn)
	{
		createTurnRender(turn);
	}

	void CombatScreen::onTurnChanged(ref_view<const mset<CombatTurn>> turns)
	{
		int i = 0;
		for (const CombatTurn& turn : turns) {
			auto res = turnUIMap.find(&turn);
			if (res != turnUIMap.end()) {
				CombatTurnRenderable* item = res->second;
				addVfxNew<UITransformVFX>(*item)
					.setMoveByOffset(0, i * TILE_SIZE)
					.setInterpCubic();
			}
			++i;
		}
	}

	void CombatScreen::onTurnRemoved(const CombatTurn* turn)
	{
		auto res = turnUIMap.find(turn);
		if (res != turnUIMap.end()) {
			uptr<CombatTurnRenderable> item = turnUI.extract(res->second);
			if (item) {
				addVfxNew<ImageRenderVFX>(move(item))
					.setInterpCubic();
			}
		}
		turnUIMap.erase(turn);
	}

	CardRenderable& CombatScreen::createCardUIRender(const Card& card, float sOffX, float sOffY) {
		CardRenderable& render = cardUI.addNew<CardRenderable>(*cardUI.hb, card, sOffX, sOffY);
		cardUIMap.emplace(&card, &render);
		render.setOnClick([this](CardRenderable& card) {selectCardRender(&card);});
		return render;
	}

	CombatTurnRenderable& CombatScreen::createTurnRender(const CombatTurn& turn)
	{
		CombatTurnRenderable& render = turnUI.addNew<CombatTurnRenderable>(turnUI.relhb(0, (turnUI.size() + 1) * TILE_SIZE, TURN_W, TILE_SIZE), turn);
		turnUIMap.emplace(&turn, &render);
		addVfxNew<UITransformVFX>(render)
			.setFade(0,1)
			.setInterpCubic();
		return render;
	}

	CreatureRenderable& CombatScreen::createOccupantRender(const OccupantObject& occupant) {
		CombatSquare* square = occupant.currentSquare;
		float offX = 0;
		float offY = 0;
		if (square) {
			offX = square->col * TILE_SIZE;
			offY = square->row * TILE_SIZE;
		}
		CreatureRenderable& render = occupantUI.addNew<CreatureRenderable>(occupantUI.relhb(offX, offY, TILE_SIZE, TILE_SIZE), occupant);
		occupantUIMap.emplace(&occupant, &render);
		return render;
	}

	// Return the rotation needed to make an object on src face dst. Assume that the object image is pointing downwards by default
	float CombatScreen::getRotationFromFacing(const CombatSquare& src, const CombatSquare& dst) const {
		// Right facing
		if (src.col < dst.col) {
			return std::numbers::pi_v<float> * 1.5f;
		}
		// Left facing
		if (src.col > dst.col) {
			return std::numbers::pi_v<float> * 0.5f;
		}
		// Up facing
		if (src.row > dst.row) {
			return std::numbers::pi_v<float>;
		}
		// Down facing is default
		return 0.0f;
	}

	uptr<CallbackVFX> CombatScreen::cardMoveVFX(const Card& card, const CardPile& pile, bool isManual) {
		// Effects should only play if it is the card owner's turn
		if (activeOccupant == pile.source) {
			uptr<CardRenderable> render = removeCardRender(&card);
			if (render) {
				render->setInteractable(true);
				Hoverable& target = uiForPile(pile.type);
				addVfxNew<ImageRenderVFX>(move(render))
					.setMove(target.getBeginX(), target.getBeginY())
					.setFade(1, 0)
					.setScale(1, 0);
				refreshCardLayout();
			}
			else {
				if (pile.type == piletype::HAND) {
					CardRenderable& newRender = createCardUIRender(card);
					addVfxNew<UITransformVFX>(newRender, 0.1f)
						.setFade(0, 1);
					refreshCardLayout();
				}
				else {
					Hoverable& target = uiForPile(pile.type);
					addVfxNew<ImageRenderVFX>(createLooseCard(card))
						.setMove(target.getHb()->x, target.getHb()->y)
						.setFade(1, 0)
						.setScale(0);
				}
			}
		}

		return make_unique<CallbackVFX>(win, isManual ? 0.2f : 0);
	}

	uptr<CallbackVFX> CombatScreen::cardUseVFX(const Card& card, const CombatSquare& target) {
		auto it = cardUIMap.find(&card);
		if (it != cardUIMap.end()) {
			it->second->setInteractable(false); // Ensure people can't accidentally click on the card
			uptr<UITransformVFX> result = make_unique<UITransformVFX>(win, *it->second);
			result->setMove(hb->w * (CARD_PLAY_POS_X_PCT), hb->h * (CARD_PLAY_POS_Y_PCT))
				.setInterpClampExp(0.3f, 3);
			return result;
		}
		else {
			uptr<SequentialRenderVFX> render = make_unique<SequentialRenderVFX>(createLooseCard(card));
			render->addNew<UITransformVFX>(*render->item)
				.setFade(0, 1)
				.setMove(hb->w * (CARD_PLAY_POS_X_PCT), hb->h * (CARD_PLAY_POS_Y_PCT))
				.setInterpClampExp(0.3f, 3);
			render->addNew<UITransformVFX>(*render->item, 0.1f)
				.setFade(1, 0);
			return render;
		}
	}

	uptr<CallbackVFX> CombatScreen::creatureMoveVFX(const OccupantObject* occupant, const CombatSquare* target) {
		auto it = occupantUIMap.find(occupant);
		CombatSquareRenderable* square = getSquareRender(target);
		if (it != occupantUIMap.end() && square) {
			uptr<UITransformVFX> result = make_unique<UITransformVFX>(win, *it->second, 0.1f);
			result->setMove(square->hb->x, square->hb->y);
			return result;
		}
		return uptr<CallbackVFX>();
	}

	void CombatScreen::onActionBegin(const Action* act) {
		clearHighlights();
	}

	void CombatScreen::onActionEnd(const Action* act, bool isLast) {
		if (isLast) {
			resetHighlights();
		}
	}

	void CombatScreen::clearHighlights() {
		for (CombatSquareRenderable& square : fieldUI) {
			square.valid = false;
			recolorSquare(square, sdl::COLOR_STANDARD);
		}
	}

	void CombatScreen::clearSelectedPath() {
		this->selectedPath.clear();
		for (CombatSquareRenderable& square : fieldUI) {
			square.arrow = nullptr;
		}
	}

	// Highlight the square that can be moved to by the owner of the current turn
	void CombatScreen::previewMovement(CombatSquare* source, const sdl::Color& color, const sdl::Color& hoverColor, int movementRange) {
		this->hoverColor = &hoverColor;
		instance->fillDistances(source);
		this->targetSizeX = 0;
		this->targetSizeY = 0;
		for (CombatSquareRenderable& square : fieldUI) {
			square.valid = square.square.sDist <= movementRange && square.square.getOccupant() == nullptr;
			recolorSquare(square, square.valid ? color : sdl::COLOR_STANDARD);
		}
	}

	// Highlight the squares that can be targeted by the current card
	void CombatScreen::previewTargeting(CombatSquare* source, const sdl::Color& color, const sdl::Color& hoverColor, int highlightRangeBegin, int highlightRangeEnd, int targetSizeX, int targetSizeY) {
		this->hoverColor = &hoverColor;
		this->targetSizeX = targetSizeX;
		this->targetSizeY = targetSizeY;
		for (CombatSquareRenderable& square : fieldUI) {
			int lineDistance = source ? square.square.getLineDistance(*source) : 0;
			square.valid = lineDistance <= highlightRangeEnd && lineDistance >= highlightRangeBegin;
			recolorSquare(square, square.valid ? color : sdl::COLOR_STANDARD);
		}
	}

	void CombatScreen::queueCard(CardRenderable& card, CombatSquareRenderable& square) {
		Creature* creature = dynamic_cast<Creature*>(activeOccupant);
		instance->queueNew<CardUseAction>(const_cast<Card&>(card.card), square.square, activeOccupant, creature ? &creature->pile : nullptr);
		selectCardRender(nullptr);
	}

	// Queue a character to be moved in the instance
	void CombatScreen::queueMove(CombatSquareRenderable& square) {
		instance->queueNew<CreaturePathAction>(*instance, activeOccupant, selectedPath, true);
		clearSelectedPath();
	}

	void CombatScreen::hoverSquareUpdate(CombatSquareRenderable* newHovered) {
		if (this->hovered != newHovered) {
			this->hovered = newHovered;
			if (newHovered && newHovered->valid) {
				if (targetSizeX > 0 && targetSizeY > 0) {
					int minCol = newHovered->square.col - targetSizeX / 2;
					int minRow = newHovered->square.row - targetSizeY / 2;
					int maxCol = minCol + targetSizeX;
					int maxRow = minRow + targetSizeY;
					for (CombatSquareRenderable& square : fieldUI) {
						const sdl::Color& target = square.square.col >= minCol && square.square.col < maxCol && square.square.row >= minRow && square.square.row < maxRow ? *hoverColor : *square.originalColor;
						if (target != square.color) {
							addVfxNew<UIRecolorVFX>(square, target);
						}
					}
				}
				else {
					for (CombatSquareRenderable& square : fieldUI) {
						const sdl::Color& target = &square == newHovered ? *hoverColor : *square.originalColor;
						if (target != square.color) {
							addVfxNew<UIRecolorVFX>(square, target);
						}
					}
				}
			}
			else {
				for (CombatSquareRenderable& square : fieldUI) {
					if (*square.originalColor != square.color) {
						addVfxNew<UIRecolorVFX>(square, *square.originalColor);
					}
				}
			}
		}
	}

	void CombatScreen::open()
	{
		GameRun* run = GameRun::current.get();
		instance = run->getCombatInstance();
		instance->viewSubscriber = this;
		endTurnButton.setInteractable(false).setEnabled(false);
		endTurnButton.setOnClick([this](UIButton& w) {
			instance->endCurrentTurn();
			w.setInteractable(false);
		});
		// Add buttons for each square
		fieldUI.setHbOffsetSize(instance->getFieldColumns() * TILE_SIZE, instance->getFieldRows() * TILE_SIZE);
		RunZone* zone = instance->getZone();
		IDrawable& squareImage = zone ? zone->getImageGrid(&cct.images.combatGridRegular) : cct.images.combatGridRegular;
		for (const CombatSquare& square : instance->getSquares()) {
			fieldUI.addNew<CombatSquareRenderable>(fieldUI.relhb(square.col * TILE_SIZE, square.row * TILE_SIZE, TILE_SIZE, TILE_SIZE), square, squareImage)
				.setOnClick([this](CombatSquareRenderable& card) {selectSquare(&card); });
		}
		// Add images for each creature
		occupantUI.setHbOffsetSize(fieldUI.hb->getOffSizeX(), fieldUI.hb->getOffSizeY());
		for (const OccupantObject* occupant : instance->getOccupants()) {
			if (occupant) {
				createOccupantRender(*occupant);
			}
		}
		// Add images for each turn
		for (const CombatTurn& turn : instance->getTurns()) {
			createTurnRender(turn);
		}
	}

	void CombatScreen::recolorSquare(CombatSquareRenderable& square, const sdl::Color& color) {
		square.originalColor = &color;
		if (square.color != color) {
			addVfxNew<UIRecolorVFX>(square, color);
		}
	}

	// Adjust the positions of cards on screen to match their actual positions in the current hand
	void CombatScreen::refreshCardLayout() {
		Creature* creature = dynamic_cast<Creature*>(activeOccupant);
		if (creature) {
			int i = 0;
			for (const uptr<Card>& card : creature->pile.hand) {
				auto res = cardUIMap.find(card.get());
				if (res != cardUIMap.end()) {
					CardRenderable* render = res->second;
					if (render) {
						addVfxNew<UITransformVFX>(*render)
							.setMoveByOffset(i * CARD_W, 0)
							.setInterpCubic();
					}
				}
				++i;
			}
		}
	}

	uptr<CardRenderable> CombatScreen::removeCardRender(const Card* card) {
		auto res = cardUIMap.find(card);
		if (res != cardUIMap.end()) {
			uptr<CardRenderable> ret = cardUI.extract(res->second);
			cardUIMap.erase(card);
			return ret;
		}
		return uptr<CardRenderable>();
	}

	uptr<CardRenderable> CombatScreen::removeCardRender(CardRenderable* card) {
		if (card) {
			cardUIMap.erase(&card->card);
		}
		return cardUI.extract(card);
	}

	// Reset the highlights to the default state for the turn (i.e. if you are not hovering over anything or clicking on anything)
	// If it is currently the turn of an actor and it is awaiting input, highlight movable squares. Otherwise, clear highlights
	void CombatScreen::resetHighlights() {
		if (activeOccupant) {
			previewMovement(activeOccupant->currentSquare, sdl::COLOR_SKY, COLOR_HIGHLIGHT_PATH, activeOccupant->getMovement());
		}
		else {
			clearHighlights();
		}
	}

	// When selecting a card, if it is actually playable, highlight the squares it can target
	void CombatScreen::selectCardRender(CardRenderable* card) {
		clearSelectedPath();
		selectedCard = card;
		if (selectedCard) {
			if (activeOccupant) {
				previewTargeting(activeOccupant->currentSquare, sdl::COLOR_GOLD, COLOR_HIGHLIGHT_TARGET, card->card.targetRangeBegin(), card->card.targetRangeEnd(), card->card.targetSizeX(), card->card.targetSizeY());
			}
		}
		else {
			resetHighlights();
		}
	}

	void CombatScreen::selectSquare(CombatSquareRenderable* square) {
		if (square && square->valid) {
			// When clicking on a square while a card is selected, play the card on the square
			if (selectedCard) {
				queueCard(*selectedCard, *square);
			}
			// When clicking on a square at the end of a highlighted path, move to that square
			else if (!this->selectedPath.empty() && this->selectedPath.back() == &square->square && activeOccupant) {
				queueMove(*square);
			}
			// Otherwise, highlight the path to be taken
			else {
				setSelectedPath(instance->findShortestPath(&square->square));
			}
		}
	}

	void CombatScreen::setSelectedPath(vec<CombatSquare*>&& squares) {
		this->selectedPath = squares;
		for (CombatSquareRenderable& square : fieldUI) {
			square.arrow = nullptr;
		}
		const CombatSquare* last = activeOccupant ? activeOccupant->currentSquare : nullptr;
		for (const CombatSquare* path : selectedPath) {
			CombatSquareRenderable* square = getSquareRender(path);
			if (square) {
				square->arrow = &cct.images.combatArrowMove;
				if (last) {
					square->arrowRotation = getRotationFromFacing(*last, square->square);
				}
				square->pathColor = COLOR_ARROW;
				last = path;
			}
		}
	}

	void CombatScreen::updateImpl()
	{
		instance->update();

		CombatTurn* currentTurn = instance->getCurrentTurn();
		Action* currentAction = instance->getCurrentAction();
		bool allowInteraction = currentTurn && !currentTurn->isDone && !currentAction;
		endTurnButton.setInteractable(allowInteraction);

		bool cHovered = false;
		for (CardRenderable& card : cardUI) {
			card.setInteractable(allowInteraction);
			if (card.isHovered()) {
				cHovered = true;
			}
		}

		// When hovering over a square, highlight squares. Otherwise, unhighlight them
		bool sqHovered = false;
		for (CombatSquareRenderable& sq : fieldUI) {
			sq.setInteractable(allowInteraction);
			if (sq.isHovered()) {
				sqHovered = true;
				hoverSquareUpdate(&sq);
			}
		}
		if (!sqHovered) {
			hoverSquareUpdate(nullptr);
		}

		// When clicking a non-card, unselect the card
		if (sdl::runner::mouseIsLeftJustClicked() && !cHovered && !sqHovered) {
			selectCardRender(nullptr);
		}


		UIScreen::updateImpl();
	}

	Hoverable& CombatScreen::uiForPile(const PileType& type) {
		if (type == piletype::DISCARD) {
			return discardPileButton;
		}
		if (type == piletype::HAND) {
			return drawPileButton;
		}
		return cardUI;
	}
}