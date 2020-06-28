compiler=g++
cppdir=cpp
hdir=cpp
lib=-lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer

t=$(cppdir)/helperengine/helperengine.cpp $(cppdir)/helperengine/helperengine_load.cpp $(cppdir)/helperengine/helperengine_draw.cpp $(cppdir)/helperengine/helperengine_runtime.cpp $(cppdir)/helperengine/helperengine_controls.cpp $(cppdir)/helperengine/helperengine_sound.cpp $(cppdir)/helperengine/helperengine_save.cpp $(cppdir)/helperengine/helperengine_edit.cpp
u=$(cppdir)/map/map.cpp
v=$(cppdir)/dynamic/dynamic.cpp $(cppdir)/dynamic/player.cpp $(cppdir)/dynamic/npc.cpp $(cppdir)/dynamic/spurdo.cpp $(cppdir)/dynamic/lever.cpp $(cppdir)/dynamic/door.cpp $(cppdir)/dynamic/pressure_plate.cpp $(cppdir)/dynamic/fireball.cpp $(cppdir)/dynamic/sentry_gun.cpp $(cppdir)/dynamic/rolling_stone.cpp $(cppdir)/dynamic/vampire.cpp $(cppdir)/dynamic/gondola.cpp $(cppdir)/dynamic/spider.cpp
w=$(cppdir)/trigger.cpp
x=$(cppdir)/sound_command.cpp $(cppdir)/message_command.cpp $(cppdir)/area.cpp $(cppdir)/event.cpp $(cppdir)/functions.cpp $(cppdir)/textbox.cpp $(cppdir)/config_table.cpp

source=$t $u $v $w $x

header=$(hdir)/helperengine/helperengine.h $(hdir)/item.h $(hdir)/map/map.h $(hdir)/map/mapcontainer.h $(hdir)/dynamic/dynamic_sounds.h $(hdir)/dynamic/dynamic_default.h $(hdir)/dynamic/alldynamics.h $(hdir)/dynamic/dynamic.h $(hdir)/dynamic/player.h $(hdir)/dynamic/npc.h $(hdir)/dynamic/spurdo.h $(hdir)/dynamic/lever.h $(hdir)/dynamic/door.h $(hdir)/dynamic/pressure_plate.h $(hdir)/dynamic/fireball.h $(hdir)/dynamic/sentry_gun.h $(hdir)/dynamic/rolling_stone.h $(hdir)/dynamic/vampire.h $(hdir)/dynamic/gondola.h $(hdir)/dynamic/spider.h $(hdir)/sound_command.h $(hdir)/message_command.h $(hdir)/area.h $(hdir)/event.h $(hdir)/functions.h $(hdir)/trigger.h $(hdir)/textbox.h $(hdir)/config_table.h

object=map.o dynamic.o player.o npc.o helperengine.o helperengine_load.o trigger.o spurdo.o lever.o door.o pressure_plate.o fireball.o sentry_gun.o rolling_stone.o vampire.o gondola.o spider.o helperengine_draw.o helperengine_runtime.o helperengine_controls.o helperengine_sound.o sound_command.o message_command.o area.o event.o functions.o helperengine_save.o helperengine_edit.o textbox.o config_table.o

program=helper

all :  $(source) $(header) $(cppdir)/main.cpp
	$(compiler) -c $(cppdir)/main.cpp $(source) -std=c++17
	$(compiler) -o $(program) main.o $(object) $(lib)
	rm $(object) main.o


