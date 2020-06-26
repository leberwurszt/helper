// map.cpp
#include "map.h"

Map::Map()
{
}

Map::Map(uint16_t x, uint16_t y)
{
	Map();
	New(x, y);
}

Map::~Map()
{
	for(uint16_t i = 0; i < sizeX; i++)
	{
		delete[] wall[i];
		delete[] floor[i];
		delete[] item[i];
	}

	delete[] wall;
	delete[] floor;
	delete[] item;
}

uint8_t Map::GetWall(uint16_t x, uint16_t y)
{
	if(x < sizeX && y < sizeY)
		return wall[x][y];
	else
	{
		//std::cerr << "Map::GetWall: Ungültige Koordinaten für wall (" << x << ", " << y << ")" << std::endl;
		return 0;
	}
}

uint8_t Map::GetFloor(uint16_t x, uint16_t y)
{
	if(x < sizeX && y < sizeY)
		return floor[x][y];
	else
	{
		//std::cerr << "Map::GetFloor: Ungültige Koordinaten für floor (" << x << ", " << y << ")" << std::endl;
		return 0;
	}
}

uint8_t Map::GetItem(uint16_t x, uint16_t y)
{
	if(x < sizeX && y < sizeY)
		return item[x][y];
	else
	{
		//std::cerr << "Map::GetItem: Ungültige Koordinaten für item (" << x << ", " << y << ")" << std::endl;
		return 0;
	}
}

uint16_t Map::GetSizeX()
{
	return sizeX;
}

uint16_t Map::GetSizeY()
{
	return sizeY;
}

uint16_t Map::GetMapMusic()
{
	return mapMusic;
}

void Map::SetMapMusic(uint16_t mapMusic)
{
	this->mapMusic = mapMusic;
}

void Map::SetWall(uint16_t x, uint16_t y, uint8_t val)
{
	if(x < sizeX && y < sizeY)
		wall[x][y] = val;
	else
		std::cerr << "invalid coordinates for wall (" << x << ", " << y << ")" << std::endl;
}

void Map::SetFloor(uint16_t x, uint16_t y, uint8_t val)
{
	if(x < sizeX && y < sizeY)
		floor[x][y] = val;
	else
		std::cerr << "invalid coordinates for floor (" << x << ", " << y << ")" << std::endl;
}

void Map::SetItem(uint16_t x, uint16_t y, uint8_t val)
{
	if(x < sizeX && y < sizeY)
		item[x][y] = val;
	else
		std::cerr << "invalid coordinates for tem (" << x << ", " << y << ")" << std::endl;
}

void Map::New(uint16_t x, uint16_t y)
{
	sizeX = x;
	sizeY = y;

	wall = new uint8_t*[sizeX];
	floor = new uint8_t*[sizeX];
	item = new uint8_t*[sizeX];

	for(uint16_t i = 0; i < sizeX; i++)
	{
		wall[i] = new uint8_t[sizeY];
		floor[i] = new uint8_t[sizeY];
		item[i] = new uint8_t[sizeY];
	}
	Clear();
}

void Map::Resize(uint16_t x, uint16_t y)
{
	uint16_t sizeXOld = sizeX;
	uint16_t sizeYOld = sizeY;

	uint8_t** wallOld = wall;
	uint8_t** floorOld = floor;
	uint8_t** itemOld = item;

	New(x, y);

	for(uint16_t i = 0; i < sizeX && i < sizeXOld; i++)
	{
		for(uint16_t k = 0; k < sizeY && k < sizeYOld; k++)
		{
			wall[i][k] = wallOld[i][k];
			floor[i][k] = floorOld[i][k];
			item[i][k] = itemOld[i][k];
		}

	}
	for(uint16_t i = 0; i < sizeXOld; i++)
	{
		delete[] wallOld[i];
		delete[] floorOld[i];
		delete[] itemOld[i];
	}

	delete[] wallOld;
	delete[] floorOld;
	delete[] itemOld;
}

// Set all Map tiles to 0, floor to 1
void Map::Clear()
{
	for(uint16_t i = 0; i < sizeX; i++)
		for(uint16_t k = 0; k < sizeY; k++)
		{
			wall[i][k] = 0;
			floor[i][k] = 1;
			item[i][k] = 0;
		}
}

bool Map::Load(std::string filename)
{
	uint16_t x;
	uint16_t y;

	std::ifstream file;
	file.open (filename, std::ios::in | std::ios::binary);
	
	if(!file)
		{
			std::cerr << "File " << filename << " does not exist!" << std::endl;
			return false;
		}

	try
	{
		// Read header
		uint8_t* header = new uint8_t[headerLength];
		file.read((char*) header, headerLength);

		// Read Map size from header
		x = (header[MAP_SIZE_X_BYTE + 3]) | (header[MAP_SIZE_X_BYTE + 2] << 8) | (header[MAP_SIZE_X_BYTE + 1] << 16) | (header[MAP_SIZE_X_BYTE + 0] << 24);
		y = (header[MAP_SIZE_Y_BYTE + 3]) | (header[MAP_SIZE_Y_BYTE + 2] << 8) | (header[MAP_SIZE_Y_BYTE + 1] << 16) | (header[MAP_SIZE_Y_BYTE    ] << 24);
		New(x, y);

		// Read player's start position from header
		x = (header[MAP_PLAYER_START_X_BYTE + 3]) | (header[MAP_PLAYER_START_X_BYTE + 2] << 8) | (header[MAP_PLAYER_START_X_BYTE + 1] << 16) | (header[MAP_PLAYER_START_X_BYTE + 0] << 24);
		y = (header[MAP_PLAYER_START_Y_BYTE + 3]) | (header[MAP_PLAYER_START_Y_BYTE + 2] << 8) | (header[MAP_PLAYER_START_Y_BYTE + 1] << 16) | (header[MAP_PLAYER_START_Y_BYTE    ] << 24);
		SetStart(x, y);

		mapMusic = (header[MAP_MUSIC_BYTE + 3]) | (header[MAP_MUSIC_BYTE + 2] << 8) | (header[MAP_MUSIC_BYTE + 1] << 16) | (header[MAP_MUSIC_BYTE    ] << 24);

		// Read map title
		for(uint16_t i = 0; i < MAP_TITLE_STRING_LENGTH && header[MAP_TITLE_STRING_BEGIN + i] != '\0'; i++)
			title += header[MAP_TITLE_STRING_BEGIN + i];

		// Read map tileset name
		for(uint16_t i = 0; i < MAP_TILESET_STRING_LENGTH && header[MAP_TILESET_STRING_BEGIN + i] != '\0'; i++)
			tileset += header[MAP_TILESET_STRING_BEGIN + i];

		// Read floor config
		for(uint16_t i = 0; i < 256; i++)
			floorType[i] = header[MAP_FLOOR_TYPE_BEGIN + i];

		delete[] header;

		// Read tiles
		for(uint16_t i = 0; i < sizeX; i++)
			for(uint16_t k = 0; k < sizeY; k++)
			{
				file.read((char*) &wall[i][k], 1);
				file.read((char*) &floor[i][k], 1);
				file.read((char*) &item[i][k], 1);
   			}
	}
	catch(const std::exception& e)
	{
		std::cerr << "Error loading map: " << e.what() << '\n';
		return false;
	}
	if(file.is_open())
		file.close();

	return true;
}

bool Map::Save(std::string filename)
{
	std::ofstream file;
	file.open(filename, std::ios::binary | std::ios::out);

	try
	{
		// Convert Values in header into char
		char* header = new char[headerLength];

		// clear header array
		for(uint32_t i = 0; i < headerLength; i++)
			header[i] = 0x00;

		// Convert Map Size X
		header[MAP_SIZE_X_BYTE + 3] = (sizeX & 0x000000ff) >>  0;
		header[MAP_SIZE_X_BYTE + 2] = (sizeX & 0x0000ff00) >>  8;
		header[MAP_SIZE_X_BYTE + 1] = (sizeX & 0x00ff0000) >> 16;
		header[MAP_SIZE_X_BYTE    ] = (sizeX & 0xff000000) >> 24;

		// Convert Map Size Y
		header[MAP_SIZE_Y_BYTE + 3] = (sizeY & 0x000000ff) >>  0;
		header[MAP_SIZE_Y_BYTE + 2] = (sizeY & 0x0000ff00) >>  8;
		header[MAP_SIZE_Y_BYTE + 1] = (sizeY & 0x00ff0000) >> 16;
		header[MAP_SIZE_Y_BYTE    ] = (sizeY & 0xff000000) >> 24;

		// Convert start position X
		header[MAP_PLAYER_START_X_BYTE + 3] = (startX & 0x000000ff) >>  0;
		header[MAP_PLAYER_START_X_BYTE + 2] = (startX & 0x0000ff00) >>  8;
		header[MAP_PLAYER_START_X_BYTE + 1] = (startX & 0x00ff0000) >> 16;
		header[MAP_PLAYER_START_X_BYTE    ] = (startX & 0xff000000) >> 24;

		// Convert start position Y
		header[MAP_PLAYER_START_Y_BYTE + 3] = (startY & 0x000000ff) >>  0;
		header[MAP_PLAYER_START_Y_BYTE + 2] = (startY & 0x0000ff00) >>  8;
		header[MAP_PLAYER_START_Y_BYTE + 1] = (startY & 0x00ff0000) >> 16;
		header[MAP_PLAYER_START_Y_BYTE    ] = (startY & 0xff000000) >> 24;

		header[MAP_MUSIC_BYTE + 3] = (mapMusic & 0x000000ff) >>  0;
		header[MAP_MUSIC_BYTE + 2] = (mapMusic & 0x0000ff00) >>  8;
		header[MAP_MUSIC_BYTE + 1] = (mapMusic & 0x00ff0000) >> 16;
		header[MAP_MUSIC_BYTE    ] = (mapMusic & 0xff000000) >> 24;

		// floor config
		for(uint16_t i = 0; i < 256; i++)
			header[MAP_FLOOR_TYPE_BEGIN + i] = floorType[i];


		// Convert map title
		for(uint16_t i = 0; i < MAP_TITLE_STRING_LENGTH; i++)
		{
			if(i < title.length())
				header[MAP_TITLE_STRING_BEGIN + i] = title.at(i);
			else
				header[MAP_TITLE_STRING_BEGIN + i] = '\0';
		}

		// Convert map tileset name
		for(uint16_t i = 0; i < MAP_TILESET_STRING_LENGTH; i++)
		{
			if(i < tileset.length())
				header[MAP_TILESET_STRING_BEGIN + i] = tileset.at(i);
			else
				header[MAP_TILESET_STRING_BEGIN + i] = '\0';
		}

		// Write file header
		file.write(header, headerLength);
		delete[] header;

		// Write tiles
		for(uint16_t i = 0; i < sizeX; i++)
			for(uint16_t k = 0; k < sizeY; k++)
			{
				file.write((char*) &wall[i][k], 1);
				file.write((char*) &floor[i][k], 1);
				file.write((char*) &item[i][k], 1);
			}
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}

	if(file.is_open())
		file.close();
	
	std::cout << "Map saved." << std::endl;
	return true;
}

std::string Map::GetTitle()
{
	return title;
}

bool Map::SetTitle(std::string title)
{
	if(title.length() <= MAP_TITLE_STRING_LENGTH)
	{
		this->title = title;
		return true;
	}
	else
	{
		return false;
	}
	
}

std::string Map::GetTileset()
{
	return tileset;
}

bool Map::SetTileset(std::string tileset)
{
	if(tileset.length() <= MAP_TILESET_STRING_LENGTH)
	{
		this->tileset = tileset;
		return true;
	}
	else
	{
		return false;
	}
}

uint16_t Map::GetStartX()
{
	return startX;
}

uint16_t Map::GetStartY()
{
	return startY;
}

bool Map::SetStart(uint16_t x, uint16_t y)
{
	if(x < sizeX && y < sizeY)
	{
		startX = x;
		startY = y;
		return true;
	}
	else
		return false;
}

uint8_t Map::GetFloorType(uint8_t index)
{
	return floorType[index];
}

void Map::SetFloorType(uint8_t index, uint8_t value)
{
	floorType[index] = value;
}

uint8_t Map::GetFloorType(uint16_t x, uint16_t y)
{
	return GetFloorType(GetFloor(x, y));
}