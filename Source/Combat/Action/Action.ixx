export module fab.Action;

import fab.FUtil;
import std;

namespace fab {
	export struct Action {
	public:
		Action() {}
		virtual ~Action() = default;

		virtual inline bool isSuccess() { return true; }
		virtual inline bool run() { return true; }
		virtual inline void complete() {}

		virtual void start() = 0;
	};
}