export module fab.FVariableGroup;

import fab.CombatInstance;
import fab.FieldObject;
import fab.FUtil;
import fab.FVariable;
import fab.GameObject;
import std;

namespace fab {
	export class FVariableGroup : private vec<uptr<FVariable>> {
	public:
		using std::vector<uptr<FVariable>>::const_iterator;
		using std::vector<uptr<FVariable>>::iterator;

		using std::vector<uptr<FVariable>>::operator[];

		using std::vector<uptr<FVariable>>::back;
		using std::vector<uptr<FVariable>>::begin;
		using std::vector<uptr<FVariable>>::clear;
		using std::vector<uptr<FVariable>>::emplace_back;
		using std::vector<uptr<FVariable>>::empty;
		using std::vector<uptr<FVariable>>::end;
		using std::vector<uptr<FVariable>>::erase;
		using std::vector<uptr<FVariable>>::front;
		using std::vector<uptr<FVariable>>::insert;
		using std::vector<uptr<FVariable>>::pop_back;
		using std::vector<uptr<FVariable>>::push_back;
		using std::vector<uptr<FVariable>>::size;

		FVariableGroup() {}
		FVariableGroup(const FVariableGroup& other) : vec<uptr<FVariable>>(other.size()) {
			for (const uptr<FVariable>& u : other) {
				emplace_back(u->clone());
			}
		}
		virtual ~FVariableGroup() = default;

		inline int getResult(int ind, CombatInstance* instance, GameObject* source, FieldObject* target, any* payload, any* outputPayload) { return size() < ind ? 0 : at(ind)->getResult(instance, source, target, payload, outputPayload); }

		vec<FVariable::Save> serialize() const;
		vec<int> getResults(CombatInstance* instance, GameObject* source, FieldObject* target, any* payload, any* outputPayload);
		void addSave(const FVariable::Save& save);
		void addSaves(const vec<FVariable::Save>& saves);
	};

	vec<FVariable::Save> FVariableGroup::serialize() const {
		vec<FVariable::Save> res(size());
		for (const uptr<FVariable>& t : *this) {
			res.emplace_back(t->serialize());
		}
		return res;
	}

	vec<int> FVariableGroup::getResults(CombatInstance* instance, GameObject* source, FieldObject* target, any* payload, any* outputPayload) {
		vec<int> res(size());
		for (const uptr<FVariable>& t : *this) {
			res.emplace_back(t->getResult(instance, source, target, payload, outputPayload));
		}
		return res;
	}

	void FVariableGroup::addSave(const FVariable::Save& save) {
		if (uptr<FVariable> var = FVariable::create(save)) {
			push_back(move(var));
		}
	}

	void FVariableGroup::addSaves(const vec<FVariable::Save>& saves) {
		for (const FVariable::Save& save : saves) {
			addSave(save);
		}
	}
}