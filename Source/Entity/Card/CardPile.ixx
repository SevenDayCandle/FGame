export module fab.CardPile;

import fab.Card;
import fab.FieldObject;
import fab.FUtil;
import fab.PileType;
import std;

namespace fab {
	export class CardPile : private vec<uptr<Card>> {
	public:
		using std::vector<uptr<Card>>::const_iterator;
		using std::vector<uptr<Card>>::iterator;

		using std::vector<uptr<Card>>::operator[];

		using std::vector<uptr<Card>>::back;
		using std::vector<uptr<Card>>::begin;
		using std::vector<uptr<Card>>::clear;
		using std::vector<uptr<Card>>::empty;
		using std::vector<uptr<Card>>::end;
		using std::vector<uptr<Card>>::erase;
		using std::vector<uptr<Card>>::front;
		using std::vector<uptr<Card>>::size;

		CardPile(const PileType& type, FieldObject* source = nullptr, int maxSize = futil::INT_MAX) : type(type), source(source), maxSize(maxSize) {}
		CardPile(const CardPile& other): vec<uptr<Card>>(other.size()), type(other.type), source(other.source), maxSize(other.maxSize) {
			for (const uptr<Card>& u : other) {
				emplace_back(make_unique<Card>(*u));
			}
		}

		const PileType& type;
		FieldObject* source;

		inline bool canAdd() const { return size() < maxSize; }
		inline int getMaxSize() const { return maxSize; }

		Card* add(CardPile::iterator it);
		Card* add(uptr<Card>&& card);
		CardPile::iterator find(Card* card);
		CardPile::iterator insert(CardPile::iterator source, CardPile::iterator pos);
		CardPile::iterator insert(uptr<Card>&& card, CardPile::iterator pos);
		int findPos(const Card* card);
		vec<uptr<Card>> resize(int newSize);
	protected:
		int maxSize;
	};

	// Attempt to add the card from an iterator, returning a pointer to the card if successful
	Card* CardPile::add(CardPile::iterator it) {
		if (canAdd()) {
			return emplace_back(move(*it)).get();
		}
		return nullptr;
	}

	// Attempt to add the card, returning a pointer to the card if successful. Be sure to check if the card can be added first, as this destroys the card if it fails
	Card* CardPile::add(uptr<Card>&& card) {
		if (canAdd()) {
			return emplace_back(move(card)).get();
		}
		return nullptr;
	}

	// Get the iterator position of the given card
	CardPile::iterator CardPile::find(Card* card) {
		return std::ranges::find_if(*this, [&card](const uptr<Card>& ptr) {return ptr.get() == card; });
	}

	// Attempt to add the card from an iterator at the specified position, returning a pointer to the card if successful and the end otherwise
	CardPile::iterator CardPile::insert(CardPile::iterator source, CardPile::iterator pos) {
		if (canAdd()) {
			return vector::insert(pos, move(*source));
		}
		return end();
	}


	// Attempt to add the card, returning a pointer to the card if successful and the end otherwise. Be sure to check if the card can be added first, as this destroys the card if it fails
	CardPile::iterator CardPile::insert(uptr<Card>&& card, CardPile::iterator pos) {
		if (canAdd()) {
			return vector::insert(pos, move(card));
		}
		return end();
	}

	// Get the number position of the given card. Returns -1 if not found
	int CardPile::findPos(const Card* card) {
		int i = 0;
		for (uptr<Card>& entry : *this) {
			if (entry.get() == card) {
				return i;
			}
			++i;
		}
		return -1;
	}

	// Enforce a new size limit. Excess cards are moved out and returned
	vec<uptr<Card>> CardPile::resize(int newSize) {
		vec<uptr<Card>> excess;
		maxSize = newSize;
		int currSize = size();
		if (currSize > maxSize) {
			auto start = end() - (currSize - maxSize);
			excess.insert(excess.end(),
				std::make_move_iterator(start),
				std::make_move_iterator(end()));
			erase(start, end());
		}
		return excess;
	}
}