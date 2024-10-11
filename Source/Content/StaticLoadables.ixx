export module fab.StaticLoadables;

import fab.BaseContent;
import fab.Cache;
import fab.ContentLoadables;
import fab.FMusic;
import fab.FSound;
import fab.FTexture;
import fab.FUtil;
import fab.ILoadable;
import fab.RBordered;
import fab.RHorizontal;
import fab.RVertical;
import std;

namespace fab {
	export class StaticLoadables : public ContentLoadables {
	public:
		StaticLoadables(const BaseContent& content) : ContentLoadables(content) {}
		StaticLoadables(StaticLoadables&& other) noexcept : ContentLoadables(move(other)), items(move(other.items)) {}

		virtual void dispose() override;
		virtual void initialize() override;
	protected:
		inline FMusic& cacheMusic(const strv& key) { return static_cast<FMusic&>(*items.emplace_back(std::make_unique<FMusic>(getPath(key, AUDIO_PATH)))); }
		inline FSound& cacheSound(const strv& key) { return static_cast<FSound&>(*items.emplace_back(std::make_unique<FSound>(getPath(key, AUDIO_PATH)))); }
		inline FTexture& cacheTexture(const strv& key) { return static_cast<FTexture&>(*items.emplace_back(std::make_unique<FTexture>(getPath(key, IMAGE_PATH)))); }
		inline RBordered& cacheBordered(const strv& key) { return static_cast<RBordered&>(*items.emplace_back(std::make_unique<RBordered>(getPath(key, IMAGE_PATH)))); }
		inline RHorizontal& cacheHorizontal(const strv& key) { return static_cast<RHorizontal&>(*items.emplace_back(std::make_unique<RHorizontal>(getPath(key, IMAGE_PATH)))); }
		inline RVertical& cacheVertical(const strv& key) { return static_cast<RVertical&>(*items.emplace_back(std::make_unique<RVertical>(getPath(key, IMAGE_PATH)))); }
	private:
		vec<uptr<ILoadable>> items;

		str getPath(const strv& key, const strv& folder) const;
	};

	// Clear out all caches
	void StaticLoadables::dispose()
	{
		for (const uptr<ILoadable>& disposable : items) {
			disposable->dispose();
		}
	}

	// Reload all caches
	void StaticLoadables::initialize()
	{
		for (const uptr<ILoadable>& disposable : items) {
			disposable->reload();
		}
	}

	// Get the path for a texture
	str StaticLoadables::getPath(const strv& key, const strv& folder) const
	{
		path pathImpl = content.contentFolder / folder / key;
		return pathImpl.string();
	}
}