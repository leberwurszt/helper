Helper Quest
by Leberwurszt and KorbiFraiser

compile it just with typing:
	make

The required libraries are:
	libsdl2-dev
	libsdl2-mixer-dev
	libsdl2-image-dev
	libsdl2-ttf-dev
	liblua5.2-dev

The main executable file is enginetest.

Parameters:
-m / --map [mapname]	select map
-e / --edit 		enable edit mode
-r / --framerate	show framerate on screen
-h / --help		help

example: enginetest -m dungeon -e
opens the program with map dungeon und edit mode

Controls:
	WASD: move
	arrow keys: attack
	escape: quit
	I: open/close inventory
	1: previous item in inventory
	2: next item in inventory
	3: use item
	4: take item
	N: noclip on/off (move through walls)
	
Editor controls (edit mode):
	E: edit dynamic (enemies etc.)
	T: edit trigger
	CTRL + T: new trigger
	P: set tile
	CTRL + S: save
	CTRL + SHIFT + S: save as...
	CTRL + O: open
	F1: show player position on console
	F2: transparent walls on/off
	F3: Change map size
	F4: fill whole map
	F6: change tileset
	F7: decrease tile code
	F8: increase tile code
	F9: set tile type to floor
	F10: set tile type to wall
	F11: set tile type to item
