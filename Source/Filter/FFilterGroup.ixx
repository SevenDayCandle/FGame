export module fab.FFilterGroup;

import fab.FFilter;
import fab.FUtil;
import std;

namespace fab {
	export class FFilterGroup : private vec<uptr<FFilter>> {
	public:
		using std::vector<uptr<FFilter>>::const_iterator;
		using std::vector<uptr<FFilter>>::iterator;

		using std::vector<uptr<FFilter>>::operator[];

		using std::vector<uptr<FFilter>>::back;
		using std::vector<uptr<FFilter>>::begin;
		using std::vector<uptr<FFilter>>::clear;
		using std::vector<uptr<FFilter>>::emplace_back;
		using std::vector<uptr<FFilter>>::empty;
		using std::vector<uptr<FFilter>>::end;
		using std::vector<uptr<FFilter>>::erase;
		using std::vector<uptr<FFilter>>::front;
		using std::vector<uptr<FFilter>>::insert;
		using std::vector<uptr<FFilter>>::pop_back;
		using std::vector<uptr<FFilter>>::push_back;
		using std::vector<uptr<FFilter>>::size;

		FFilterGroup() {}
		FFilterGroup(const FFilterGroup& other): vec<uptr<FFilter>>(other.size()) {
			for (const uptr<FFilter>& u : other) {
				emplace_back(u->clone());
			}
		}
		virtual ~FFilterGroup() = default;

		inline bool passes(void* object) const { return std::ranges::any_of(*this, [object](const uptr<FFilter>& filter) {return filter->passes(object); }); }

		vec<FFilter::Save> serialize() const;
		void addSave(const FFilter::Save& save);
		void addSaves(const vec<FFilter::Save>& saves);
	};

	vec<FFilter::Save> FFilterGroup::serialize() const {
		vec<FFilter::Save> res(size());
		for (const uptr<FFilter>& t : *this) {
			res.emplace_back(t->serialize());
		}
		return res;
	}

	void FFilterGroup::addSave(const FFilter::Save& save) {
		if (uptr<FFilter> filter = FFilter::create(save)) {
			push_back(move(filter));
		}
	}

	void FFilterGroup::addSaves(const vec<FFilter::Save>& saves) {
		for (const FFilter::Save& save : saves) {
			addSave(save);
		}
	}
}