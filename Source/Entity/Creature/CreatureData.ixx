export module fab.CreatureData;

import fab.BaseContent;
import fab.EmptyDrawable;
import fab.GameObjectData;
import fab.FTexture;
import fab.FUtil;
import fab.IDrawable;
import fab.ItemListing;
import std;

namespace fab {
	export class CreatureData : public GameObjectData<CreatureData> {
	public:
		static constexpr cstr FOLDER = "Creatures";
		static constexpr cstr LOCPATH = "CreatureStrings";

		struct Fields {
			int actSpeed = 100;
			int actSpeedUp = 0;
			int energyGain = 9;
			int energyGainUp = 0;
			int energyMax = 9;
			int energyMaxUp = 0;
			int handDraw = 5;
			int handDrawUp = 0;
			int handSize = 5;
			int handSizeUp = 0;
			int health = 300;
			int healthUp = 0;
			int movement = 3;
			int movementUp = 0;
			str behavior;
			str imageField;
			vec<ItemListing> cards;
			vec<ItemListing> passives;
		};

		CreatureData(BaseContent& source, strv id): GameObjectData(source, id) {}
		CreatureData(BaseContent& source, strv id, const Fields& fields) : GameObjectData(source, id), data(fields) {}
		Fields data;

		inline int getResultActSpeed(int upgrade) const { return data.actSpeed + upgrade * data.actSpeedUp; }
		inline int getResultEnergyGain(int upgrade) const { return data.energyGain + upgrade * data.energyGainUp; }
		inline int getResultEnergyMax(int upgrade) const { return data.energyMax + upgrade * data.energyMaxUp; }
		inline int getResultHandDraw(int upgrade) const { return data.handDraw + upgrade * data.handDrawUp; }
		inline int getResultHandSize(int upgrade) const { return data.handSize + upgrade * data.handSizeUp; }
		inline int getResultHealth(int upgrade) const { return data.health + upgrade * data.healthUp; }
		inline int getResultMovement(int upgrade) const { return data.movement + upgrade * data.movementUp; }
		inline void clearImageField() { imageField = nullptr; }
		inline void clearImagePortrait() { imagePortrait = nullptr; }

		IDrawable& getImageField() const;
		IDrawable& getImagePortrait() const;
	protected:
		mutable IDrawable* imageField;
		mutable IDrawable* imagePortrait;
	};

	IDrawable& CreatureData::getImageField() const
	{
		if (!imageField) {
			if (data.imageField.size() > 0) {
				FTexture* tex = source.getTexture(str(FOLDER) + "/" + data.imageField);
				if (tex && tex->loaded()) {
					imageField = tex;
				}
			}
			if (!imageField) {
				FTexture* tex = source.getTexture(str(FOLDER) + "/" + id + "_f.png");
				if (tex && tex->loaded()) {
					imageField = tex;
				}
				if (!imageField) {
					imageField = &EMPTY;
				}
			}
		}
		return *imageField;
	}

	IDrawable& CreatureData::getImagePortrait() const
	{
		if (!imagePortrait) {
			FTexture* tex = source.getTexture(str(FOLDER) + "/" + id + ".png");
			if (tex && tex->loaded()) {
				imagePortrait = tex;
			}
			if (!imagePortrait) {
				imagePortrait = &EMPTY;
			}
		}
		return *imagePortrait;
	}
}