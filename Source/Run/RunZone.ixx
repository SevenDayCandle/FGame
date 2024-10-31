export module fab.RunZone;

import fab.BaseContent;
import fab.ContentRegisterable;
import fab.FTexture;
import fab.FUtil;
import fab.IDrawable;
import std;

namespace fab {
	export class RunZone : public ContentRegisterable<RunZone> {
	public:
		static constexpr cstr FOLDER = "Zones";

		struct Data {
			bool allowCommonEncounters = true;
			int sizeCols = 5;
			int sizeRows = 8;
			umap<str, int> roomWeights;
			vec<int> levels;
		};
		RunZone(BaseContent& source, strv id, Data& data) : ContentRegisterable(source, id), data(data) {}
		virtual ~RunZone() = default;

		Data data;

		IDrawable& getImageGrid(IDrawable* fallback = nullptr) const;
	protected:
		mutable IDrawable* imageGrid;
	};

	IDrawable& RunZone::getImageGrid(IDrawable* fallback) const {
		if (!imageGrid) {
			FTexture* tex = source.getTexture(str(FOLDER) + "/" + id + ".png");
			if (tex && tex->loaded()) {
				imageGrid = tex;
			}
			if (!imageGrid) {
				imageGrid = fallback;
			}
		}
		return *imageGrid;
	}
}