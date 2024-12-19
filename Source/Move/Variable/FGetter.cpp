import fab.FUtil;
import std;

module fab.FGetter;

namespace fab {
	void FGetter::load(const Save& save) {
		if (save.filters) {
			filters.addSaves(save.filters.value());
		}
	}
}