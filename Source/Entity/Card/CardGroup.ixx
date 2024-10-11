export module fab.CardGroup;

import fab.BaseContent;
import fab.FUtil;
import fab.ContentRegisterable;

namespace fab {
	export class CardGroup : public ContentRegisterable<CardGroup> {
	public:
		CardGroup(BaseContent& source, strv id) : ContentRegisterable(source, id) {}
	};
}