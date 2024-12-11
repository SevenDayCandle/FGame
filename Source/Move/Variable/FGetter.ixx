export module fab.FGetter;

import fab.FFilter;
import fab.FUtil;
import fab.FVariable;
import std;

namespace fab {
	export class FGetter : public FVariable {
	public:
		FGetter(Data& data) : FVariable(data) {}
		FGetter(Data& data, const Save& save) : FVariable(data, save) {}
		FGetter(const FGetter& other) : FVariable(other) {}
		FGetter(FGetter&& other) noexcept = default;
		virtual ~FGetter() = default;
	protected:
		vec<uptr<FFilter>> filters;
	};
}