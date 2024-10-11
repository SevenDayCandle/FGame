export module fab.ContentManifest;

import fab.FUtil;

namespace fab {
	export struct ContentManifest {
		str id;
		str name;
		str description;
		str version;
		str fVersion;
		str credits;
		vec<str> dependencies;
	};
}