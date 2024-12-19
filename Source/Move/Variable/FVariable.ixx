export module fab.FVariable;

import fab.CombatInstance;
import fab.FFilter;
import fab.FieldObject;
import fab.FUtil;
import fab.GameObject;
import fab.KeyedItem;
import std;

namespace fab {
	export class FVariable {
	public:
		struct Save {
			str id;
			opt<strumap<str>> fields;
			opt<vec<FFilter::Save>> filters;
			opt<vec<Save>> vars;
		};

		FVariable() {}
		FVariable(FVariable&& other) noexcept = default;
		virtual ~FVariable() = default;

		Save serialize() const;

		static uptr<FVariable> create(const Save& save);

		virtual int getResult(CombatInstance* instance, GameObject* source, FieldObject* target, any* payload, any* outputPayload) = 0; // Generate a number output based on the game state. Records the objects used to calculate the output into outputPayload, if provided
		virtual strv getId() const = 0; // The ID used to identify this variable
		virtual uptr<FVariable> clone() = 0; // Make a unique pointer copy of this object
		virtual void serializeImpl(Save& save) const = 0; // Additional fields to add to the save
	};
}