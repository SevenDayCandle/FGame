module;

import fab.Action;
import fab.CombatInstance;
import fab.CombatTurn;
import fab.Card;
import fab.CombatSquare;
import fab.Creature;
import fab.CreatureData;
import fab.EncounterCreatureEntry;
import fab.FieldObject;
import fab.FUtil;
import fab.GameRun;
import fab.OccupantObject;
import fab.OnTurnBeginSubscription;
import fab.OnTurnEndSubscription;
import fab.RunEncounter;
import fab.RunZone;
import fab.SavedCreatureEntry;
import fab.TurnObject;
import sdl.SDLBase; 
import sdl.SDLBatchRenderPass; 
import sdl.SDLRunner;
import std;

module fab.CombatInstance;

namespace fab {

	// Sets up a combat instance based on the given data
	void CombatInstance::initialize(RunEncounter& encounter, vec<SavedCreatureEntry>& runCreatures, int playerFaction)
	{
		// Generates squares based on the given dimensions
		this->fieldColumns = encounter.data.fieldCols;
		this->fieldRows = encounter.data.fieldRows;
		this->zone = RunZone::get(encounter.id);
		this->roundTime = encounter.data.time;
		squares.clear();
		for (int j = 0; j < fieldRows; j++) {
			for (int i = 0; i < fieldColumns; i++) {
				squares.emplace_back(i, j);
			}
		}

		// Creates field members based on the room parameters
		// TODO handle other fieldobject types besides creature
		int squarePos;
		for (EncounterCreatureEntry& entry : encounter.data.creatures) {
			CreatureData* data = CreatureData::get(entry.id);
			if (data) {
				Creature::Behavior* behavior = Creature::Behavior::get(data->data.behavior);
				squarePos = getSquareIndexAllowRandom(entry.posCol, entry.posRow);

				// If square is occupied, find an unoccupied one
				CombatSquare* square = squarePos < squares.size() ? &squares[squarePos] : nullptr;
				while (squarePos < squares.size() - 1 && square->getOccupant() != nullptr) {
					squarePos += 1;
					square = &squares[squarePos];
				}

				if (square) {
					uptr<Creature> pt = make_unique<Creature>(*data, behavior, entry.faction, entry.upgrades);
					Creature& creature = *pt;
					occupants.push_back(move(pt));
					square->setOccupant(&creature);
					creature.initialize(data->data.cards, data->data.passives);
					sdl::logInfo("Added creature %s on combat grid to square %d", entry.id.data(), squarePos);
				}
				else {
					sdl::logInfo("Failed to allocate creature %s on combat grid", entry.id.data());
				}
			}
		}

		// Places player characters on the field based on specified available spots in the room. If available spots are not enough, just stack horizontally (starting from the map center if there were no spots period)
		int index = 0;
		if (encounter.data.startPos.size() > index) {
			squarePos = getSquareIndexAllowRandom(encounter.data.startPos[index].first, encounter.data.startPos[index].second);
		}
		else {
			squarePos = getSquareIndex(encounter.data.fieldCols / 2, encounter.data.fieldRows / 2);
		}

		for (SavedCreatureEntry& entry : runCreatures) {
			// Do not spawn dead creatures
			if (entry.health > 0) {
				CreatureData* data = CreatureData::get(entry.content, entry.id);
				if (data) {
					Creature::Behavior* behavior = Creature::Behavior::get(entry.behaviorId);

					// If square is occupied, find an unoccupied one
					CombatSquare* square = squarePos < squares.size() ? &squares[squarePos] : nullptr;
					while (squarePos < squares.size() - 1 && square->getOccupant() != nullptr) {
						squarePos += 1;
						square = &squares[squarePos];
					}

					if (square) {
						uptr<Creature> pt = make_unique<Creature>(*data, behavior, playerFaction, entry.upgrades, entry.health);
						Creature& creature = *pt;
						occupants.push_back(move(pt));
						square->setOccupant(&creature);
						creature.initialize(entry.cards, entry.passives);
						sdl::logInfo("Added creature %s on combat grid to square %d", entry.id.data(), squarePos);
					}
					else {
						sdl::logInfo("Failed to allocate creature %s on combat grid", entry.id.data());
					}

					++index;
					if (encounter.data.startPos.size() > index) {
						squarePos = getSquareIndexAllowRandom(encounter.data.startPos[index].first, encounter.data.startPos[index].second);
					}
					else {
						squarePos += 1;
					}
				}
			}
		}

		// TODO Setup subscribers

		// Setup initial turns
		for (OccupantObject* occupant : getOccupants()) {
			requestQueueTurn(*occupant);
		}

		// TODO Start of combat hooks for all field members
		for (OccupantObject* occupant : getOccupants()) {
			occupant->postInitialize();
		}
	}

	// Queue an action to be executed
	void CombatInstance::queueActionImpl(uptr<Action>&& action)
	{
		if (action->isLowPriority()) {
			actionQueueLow.push_back(move(action));
		}
		else {
			actionQueue.push_back(move(action));
		}
	}

	// Queue an action to be executed right after the current one
	void CombatInstance::queueActionImplFront(uptr<Action>&& action) {
		if (action->isLowPriority()) {
			if (actionQueueLow.size() <= 1) {
				actionQueueLow.push_back(move(action));
			}
			else {
				actionQueueLow.insert(actionQueueLow.begin() + 1, move(action));
			}
		}
		else {
			if (actionQueue.size() <= 1) {
				actionQueue.push_back(move(action));
			}
			else {
				actionQueue.insert(actionQueue.begin() + 1, move(action));
			}
		}
	}

	// Marks the current turn as being completed, so that it can be pushed out after actions are done
	void CombatInstance::queueCompleteTurn()
	{
		if (currentTurn) {
			currentTurn->isDone = true;
		}
	}

	// Queue a turn to be executed. Turn queue is always sorted by ascending action order. ActionValue is relative to the total action time that has already elapsed
	void CombatInstance::queueTurn(TurnObject& source, int actionValue)
	{
		const CombatTurn& turn = *turns.emplace(*this, source, totalActionTime + actionValue);
		if (viewSubscriber) {
			viewSubscriber->onTurnAdded(turn);
			viewSubscriber->onTurnChanged(getTurns());
		}
		// TODO hooks
	}

	// Attempt to create a turn for this occupant based on its action speed
	void CombatInstance::requestQueueTurn(TurnObject& occupant) {
		int actSpeed = occupant.getActionSpeed();
		if (actSpeed > 0) {
			int actionValue = roundTime * 100 / (actSpeed);
			queueTurn(occupant, actionValue);
		}
	}

	// Ends the combat
	void CombatInstance::endCombat()
	{
		// TODO hooks
		completed = true;
	}

	// Ends the current turn
	void CombatInstance::endCurrentTurn()
	{
		if (!turns.empty()) {
			auto it = turns.begin();
			CombatTurn& turn = const_cast<CombatTurn&>(*it);
			CombatTurn* pt = &turn;
			requestQueueTurn(turn.source);
			turn.source.onTurnEnd(*currentTurn);
			// TODO hooks
			totalActionTime = turn.actionValue;
			if (!turn.isDone && viewSubscriber) {
				viewSubscriber->onPlayerTurnEnd(pt);
			}
			turns.erase(it);
			if (viewSubscriber) {
				viewSubscriber->onTurnRemoved(pt);
				viewSubscriber->onTurnChanged(getTurns());
			}
			// TODO round end logic (whenever totalActionTime exceeds a multiple of round time)
		}
		currentTurn = nullptr;
	}

	// Generate a distance map from the source to all other squares
	void CombatInstance::fillDistances(CombatSquare* origin)
	{
		if (origin != distanceOrigin && origin != nullptr) {
			distanceOrigin = origin;
			deque<CombatSquare*> queue { origin };
			for (CombatSquare& sq : squares) {
				sq.sDist = futil::INT_MAX;
			}
			origin->sDist = 0;
			OccupantObject* source = origin->getOccupant();

			while (!queue.empty()) {
				CombatSquare* square = queue.front();
				queue.pop_front();
				int scol = square->col;
				int srow = square->row;
				int tDist = square->sDist + 1;
				for (int i = 0; i < 4; i++) {
					int tcol = scol + DIR_X[i];
					int trow = srow + DIR_Y[i];
					CombatSquare* tsquare = getSquare(tcol, trow);
					if (tsquare && tsquare->sDist > tDist) {
						if (tsquare->passable(source)) {
							tsquare->sDist = tDist;
							queue.push_back(tsquare);
						}
						else {
							tsquare->sDist = -1; // Unreachable
						}
					}
				}
			}
		}
	}

	// Change the action order for all turns corresponding with the target
	bool CombatInstance::modifyTurnOrder(const TurnObject& target, int diff)
	{
		for (auto it = turns.begin(); it != turns.end(); ) {
			if (&(it->source) == &target) {
				CombatTurn modifiedTurn = move(*it);
				modifiedTurn.actionValue += diff;
				it = turns.erase(it);
				turns.insert(move(modifiedTurn));
				if (viewSubscriber) {
					viewSubscriber->onTurnChanged(getTurns());
				}
			}
			else {
				++it;
			}
		}
		return false;
	}

	// Drops the current turn and starts the next turn in the queue. Returns true if there are no more turns left or if combat condition is satisfied
	bool CombatInstance::nextTurn()
	{
		// TODO combat end condition check (i.e. only one faction alive)
		if (!turns.empty()) {
			currentTurn = const_cast<CombatTurn*>(&*turns.begin());
			bool isPlayer = currentTurn->source.onTurnBegin(*currentTurn);
			for (OnTurnBeginSubscription* s : getSubscribers<OnTurnBeginSubscription>()) {
				s->onTurnBegin(*currentTurn);
			}
			// TODO hooks
			if (isPlayer && viewSubscriber) {
				viewSubscriber->onPlayerTurnBegin(currentTurn);
			}
			return false;
		}
		return true;
	}

	/* Runs a loop of the combat
	 * Returns true if the combat instance should stop running
	 */
	bool CombatInstance::update()
	{
		// Run current action
		if (currentAction) {
			if (currentAction->run()) {
				currentAction->complete();
				if (viewSubscriber) {
					viewSubscriber->onActionEnd(currentAction, actionQueue.size() <= 1 && actionQueueLow.size() <= 1);
				}
				currentAction = nullptr;
				actionQueue.pop_front();
			}
			return false;
		}
		// Otherwise poll actions if any are present
		else if (!actionQueue.empty()) {
			currentAction = actionQueue.front().get();
			currentAction->start();
			if (viewSubscriber) {
				viewSubscriber->onActionBegin(currentAction);
			}
			return false;
		}
		// Otherwise run current low action
		if (currentActionLow) {
			if (currentActionLow->run()) {
				currentActionLow->complete();
				if (viewSubscriber) {
					viewSubscriber->onActionEnd(currentActionLow, actionQueueLow.size() <= 1);
				}
				currentActionLow = nullptr;
				actionQueueLow.pop_front();
			}
			return false;
		}
		// Otherwise poll low actions if any are present
		else if (!actionQueueLow.empty()) {
			currentActionLow = actionQueueLow.front().get();
			currentActionLow->start();
			if (viewSubscriber) {
				viewSubscriber->onActionBegin(currentActionLow);
			}
			return false;
		}
		// Otherwise run current turn
		else if (currentTurn) {
			if (currentTurn->source.onTurnRun(*currentTurn) || currentTurn->isDone) {
				endCurrentTurn();
			}
			return false;
		}
		// Otherwise poll the next turn if it is present
		return nextTurn();
	}

	// Wrapper around getSquareIndex that treats negative values as random
	int CombatInstance::getSquareIndexAllowRandom(int col, int row)
	{
		int squareCol = col < 0 ? GameRun::current->rngEncounter.random(0, fieldColumns - 1) : col;
		int squareRow = row < 0 ? GameRun::current->rngEncounter.random(0, fieldRows - 1) : row;
		return getSquareIndex(squareCol, squareRow);
	}

	// Get the square at the col/row position
	CombatSquare* CombatInstance::getSquare(int col, int row)
	{
		if (col >= 0 && col < fieldColumns && row >= 0 && row < fieldRows) {
			int targetSquare = getSquareIndex(col, row);
			return (targetSquare < squares.size() && targetSquare >= 0 ? &squares[targetSquare] : nullptr);
		}
		return nullptr;
	}

	// Get the distance from the distance source to the given square. Return -1 if unreachable
	int CombatInstance::getDistanceTo(CombatSquare* square)
	{
		CombatSquare* sq = getSquare(square->col, square->row);
		return sq != nullptr ? sq->sDist : -1;
	}

	// Get the creature who is currently acting, if it exists
	OccupantObject* CombatInstance::getCurrentActor() const {
		CombatTurn* currentTurn = getCurrentTurn();
		if (currentTurn) {
			return dynamic_cast<OccupantObject*>(&currentTurn->source);
		}
		return nullptr;
	}

	// Find a shortest path from the source square (exclusive) to the target square (inclusive)
	vec<CombatSquare*> CombatInstance::findShortestPath(const CombatSquare* targ, int limit)
	{
		vec<CombatSquare*> path;
		int targDist = targ->sDist;
		// Distance of -1 or INT_MAX means that the square cannot be reached
		if (targDist >= 0 && targDist != futil::INT_MAX) {
			CombatSquare* current = const_cast<CombatSquare*>(targ);
			while (current != distanceOrigin) {
				if (current->sDist <= limit) {
					path.push_back(current);
				}
				int scol = current->col;
				int srow = current->row;
				for (int i = 0; i < 4; i++) {
					int tcol = scol + DIR_X[i];
					int trow = srow + DIR_Y[i];
					CombatSquare* tsquare = getSquare(tcol, trow);
					if (tsquare && tsquare->sDist >= 0 && tsquare->sDist < current->sDist) {
						current = tsquare;
						break;
					}
				}
			}
		}
		std::ranges::reverse(path);
		return path;
	}
}