module;

#include "glaze/glaze.hpp"

export module fab.DynamicContent;

import fab.BaseContent;
import fab.CardData;
import fab.CoreConfig;
import fab.CoreContent;
import fab.CreatureData;
import fab.DynamicLoadables;
import fab.DynamicStrings;
import fab.FMusic;
import fab.FSound;
import fab.FTexture;
import fab.FUtil;
import fab.Keyword;
import fab.KeywordStrings;
import fab.ObjectStrings;
import fab.PassiveData;
import fab.RunEncounter;
import fab.RunZone;
import fab.StatusData;
import sdl.SDLBase;
import std;

namespace fab {
	export constexpr strv PATH_OBJECTS = "Objects";
	export constexpr strv FILE_ENCOUNTER = "Encounters.json";
	export constexpr strv FILE_ZONE = "Zones.json";
	export constexpr strv MANIFEST = "manifest.json";

	export class DynamicContent : public BaseContent {
	public:
		DynamicContent(CoreConfig& cfg, strv id, strv folder): BaseContent(cfg, id, folder) {}
		~DynamicContent() override {}

		DynamicStrings strings = DynamicStrings(*this);
		DynamicLoadables<FMusic> music = DynamicLoadables<FMusic>(*this);
		DynamicLoadables<FSound> sounds = DynamicLoadables<FSound>(*this);
		DynamicLoadables<FTexture> images = DynamicLoadables<FTexture>(*this);

		inline FMusic* getMusic(strv key) const final { return music.get(key); }
		inline FSound* getSound(strv key) const final { return sounds.get(key); }
		inline FTexture* getTexture(strv key) const final { return images.get(key); }
		inline KeywordStrings* getKeywordStrings(strv path) const final { return strings.getKeywordStrings(path); }
		inline void processCards() { setupContentFolder(CardData::FOLDER, [this](const path& entry) {processCard(entry); }); }
		inline void processCreatures() { setupContentFolder(CreatureData::FOLDER, [this](const path& entry) {processCreature(entry); }); }
		inline void processKeywords() { setupContentFolder(PATH_KEYWORDS, [this](const path& entry) {processKeyword(entry); }); }
		inline void processPassives() { setupContentFolder(PassiveData::FOLDER, [this](const path& entry) {processKeyword(entry); }); }
		inline void processStatuses() { setupContentFolder(StatusData::FOLDER, [this](const path& entry) {processKeyword(entry); }); }


		void dispose() override;
		void initialize() override;
		void postInitialize() override;
		void reloadAudio() override;
		void reloadImages() override;
		void unloadObjects();

		static CoreContent generate(CoreConfig& cfg, strv id);
		static void loadDynamicContent(CoreContent& cct);
	private:
		void processCard(const path& entry);
		void processCreature(const path& entry);
		void processEncounters();
		void processKeyword(const path& entry);
		void processPassive(const path& entry);
		void processStatus(const path& entry);
		void processZones();
		void setupContentFolder(strv base, const func<void(const path&)>& onRead);
	};

	void DynamicContent::dispose() {
		// TODO de-register creatures, objects, etc.
		images.dispose();
		music.dispose();
		sounds.dispose();
		strings.dispose();
	}

	void DynamicContent::initialize() {
		strings.initialize();
		images.initialize();
		music.initialize();
		sounds.initialize();
	}

	void DynamicContent::postInitialize()
	{
		processCards();
		processCreatures();
		processZones();
		processEncounters();
		// TODO process other types of stuff
	}

	void DynamicContent::reloadAudio() {
		music.initialize();
		sounds.initialize();
	}

	void DynamicContent::reloadImages() { images.initialize(); }

	// Attempt to parse a card entry
	void DynamicContent::processCard(const path& entry)
	{
		CardData::ExportFields fields;

		auto error = glz::read_file_json(fields, entry.string(), str{});
		if (error) {
			sdl::logError("Failed to load card at path %s: %d, %d", entry.string().data(), error.ec, error.location);
		}
		else {
			CardData* data = CardData::registerData(make_unique<CardData>(*this, entry.stem().string(), fields));
			sdl::logInfo("Loaded card at path %s: %s", entry.string().data(), data->id);
		}
	}

	// Attempt to parse a creature entry
	void DynamicContent::processCreature(const path& entry)
	{
		CreatureData::ExportFields fields;
		
		auto error = glz::read_file_json(fields, entry.string(), str{});
		if (error) {
			sdl::logError("Failed to load creature at path %s", entry.string().data());
		}
		else {
			CreatureData::registerData(make_unique<CreatureData>(*this, entry.stem().string(), fields));
			sdl::logInfo("Loaded creature at path %s", entry.string().data());
		}
		
	}

	// Attempt to parse a encounter entry
	void DynamicContent::processEncounters()
	{
		path file = contentFolder / PATH_OBJECTS / FILE_ENCOUNTER;
		if (std::filesystem::exists(file)) {
			umap<str, RunEncounter::Data> zones;
			str fn = file.string();
			auto error = glz::read_file_json(zones, fn, str{});
			if (error) {
				sdl::logError("Failed to load encounters at path %s", fn.data());
			}
			else {
				for (auto& pair : zones) {
					RunEncounter* encounter = RunEncounter::registerData(make_unique<RunEncounter>(*this, pair.first, pair.second));
					sdl::logInfo("Loaded encounter with ID %s", encounter->id.data());
				}
			}
		}
	}

	// TODO Attempt to parse a keyword entry
	void DynamicContent::processKeyword(const path& entry)
	{

	}

	// Attempt to parse a zone entry
	void DynamicContent::processZones()
	{
		path file = contentFolder / PATH_OBJECTS / FILE_ZONE;
		if (std::filesystem::exists(file)) {
			umap<str, RunZone::Data> zones;
			str fn = file.string();
			auto error = glz::read_file_json(zones, fn, str{});
			if (error) {
				sdl::logError("Failed to load zones at path %s", fn.data());
			}
			else {
				for (auto& pair : zones) {
					RunZone* zone = RunZone::registerData(make_unique<RunZone>(*this, pair.first, pair.second));
					sdl::logInfo("Loaded zone with ID %s", zone->id.data());
				}
			}
		}
	}

	void DynamicContent::setupContentFolder(strv base, const func<void(const path&)>& onRead)
	{
		path folder = contentFolder / PATH_OBJECTS / base;
		if (std::filesystem::exists(folder)) {
			for (const dir_entry& entry : std::filesystem::directory_iterator(folder)) {
				if (entry.is_regular_file()) {
					onRead(entry.path());
				}
			}
		}
	}

	// De-register creatures, objects, etc.
	void DynamicContent::unloadObjects()
	{
		// TODO
	}

	/* STATICS */

	CoreContent DynamicContent::generate(CoreConfig& cfg, strv id) {
		CoreContent cct = CoreContent(cfg, id);
		cct.initialize();
		loadDynamicContent(cct);
		cct.initializeContents();
		cct.postInitialize();
		return cct;
	}

	// Generate dynamic content from each folder in the content root folder
	void DynamicContent::loadDynamicContent(CoreContent& cct)
	{
		for (const dir_entry& entry : std::filesystem::directory_iterator(sdl::dirBase() + str(CONTENT_ROOT))) {
			if (std::filesystem::is_directory(entry.status())) {
				path subdir = entry.path();
				path manifestPath = subdir / MANIFEST;

				sdl::logInfo("Loading content at path %s", subdir.string().data());

				// TODO process content manifest
				if (std::filesystem::exists(manifestPath)) {
					//processManifest(manifestPath, id);
				}
				else {
					str id = sanitizeID(subdir.filename().string());
					cct.registerContent(std::make_unique<DynamicContent>(cct.cfg, id, subdir.string()));
					sdl::logInfo("Registered content without manifest with ID %s", id.data());
				}
			}
		}
	}
}
