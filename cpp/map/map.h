// map.h

#pragma once
#include <iostream>
#include <cstdint>
#include <string>
#include <fstream>

#define MAP_HEADER_LENGTH 0x0400
#define MAP_SIZE_X_BYTE 0x0000
#define MAP_SIZE_Y_BYTE 0x0004
#define MAP_PLAYER_START_X_BYTE 0x0008
#define MAP_PLAYER_START_Y_BYTE 0x000C
#define MAP_MUSIC_BYTE 0x0010
#define MAP_TITLE_STRING_BEGIN 0x0020
#define MAP_TITLE_STRING_LENGTH 0x0040
#define MAP_TILESET_STRING_BEGIN 0x0060
#define MAP_TILESET_STRING_LENGTH 0x0040
#define MAP_FLOOR_TYPE_BEGIN 0x0100
#define MAP_WALL_TYPE_BEGIN 0x0200

#define FLOOR_TYPE_NORMAL 0x00
#define FLOOR_TYPE_WATER 0x01

#define WALL_TYPE_NORMAL 0x00
#define WALL_TYPE_NO_WALL 0x01
#define WALL_TYPE_CLIFF_NORTH 0x02
#define WALL_TYPE_CLIFF_EAST 0x03
#define WALL_TYPE_CLIFF_SOUTH 0x04
#define WALL_TYPE_CLIFF_WEST 0x05

class Map
{
	// =========== PRIVATE ============
	private:

		uint32_t headerLength = MAP_HEADER_LENGTH;

		uint16_t sizeX =	0;
		uint16_t sizeY =	0;

		uint16_t startX =	0;
		uint16_t startY =	0;

		uint16_t mapMusic = 0;

		uint8_t** wall =	NULL;
		uint8_t** floor =	NULL;
		uint8_t** item =	NULL;

		uint8_t floorType[256];

		std::string title =	""; 
		std::string tileset = "";

	// =========== PUBLIC ============
	public:

		Map();
		Map(uint16_t x, uint16_t y);
		~Map();

		// get methods
		std::string GetTitle();
		std::string GetTileset();

		uint8_t GetWall(uint16_t x, uint16_t y);
		uint8_t GetFloor(uint16_t x, uint16_t y);
		uint8_t GetItem(uint16_t x, uint16_t y);

		uint8_t GetFloorType(uint8_t index);
		uint8_t GetFloorType(uint16_t x, uint16_t y);

		uint16_t GetSizeX();
		uint16_t GetSizeY();
		uint16_t GetStartX();
		uint16_t GetStartY();

		uint16_t GetMapMusic();

		// set methods
		bool SetTitle(std::string title);
		bool SetTileset(std::string tileset);

		void SetWall(uint16_t x, uint16_t y, uint8_t val);
		void SetFloor(uint16_t x, uint16_t y, uint8_t val);
		void SetItem(uint16_t x, uint16_t y, uint8_t val);

		void SetFloorType(uint8_t index, uint8_t value);

		bool SetStart(uint16_t x, uint16_t y);

		void SetMapMusic(uint16_t mapMusic);

		// other methods
		void New(uint16_t x, uint16_t y);
		void Resize(uint16_t x, uint16_t y);
		void Clear();

		bool Load(std::string filename);
		bool Save(std::string filename);

};