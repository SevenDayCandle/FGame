export module fab.StatusTurnMode;

import fab.FUtil;

namespace fab {
	export enum StatusTurnMode {
		NONE,
		ROUND_BEGIN,
		ROUND_END,
		TURN_BEGIN,
		TURN_END,
	};

	export ilist<StatusTurnMode> WINDOWMODE_ALL = { NONE, ROUND_BEGIN, ROUND_END, TURN_BEGIN, TURN_END };
}