export module fab.ItemListing;

import fab.FUtil;

namespace fab {
	export struct ItemListing {
		str id;
		int count = 1;
		int upgrades = 0;
	};
}