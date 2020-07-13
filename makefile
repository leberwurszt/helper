BINARY_NAME = helper

COMPILER = g++
COMPILER_FLAGS =
COMPILER_FLAGS += -std=c++17
COMPILER_FLAGS += -Werror

HEADER_DIR = ./cpp
SOURCE_DIR = ./cpp

LIBS =
LIBS += -lSDL2
LIBS += -lSDL2_image
LIBS += -lSDL2_ttf
LIBS += -lSDL2_mixer

helper_SOURCE =
helper_SOURCE += $(SOURCE_DIR)/main.cpp
helper_SOURCE += $(SOURCE_DIR)/helperengine/helperengine.cpp
helper_SOURCE += $(SOURCE_DIR)/helperengine/helperengine_load.cpp
helper_SOURCE += $(SOURCE_DIR)/helperengine/helperengine_draw.cpp
helper_SOURCE += $(SOURCE_DIR)/helperengine/helperengine_runtime.cpp
helper_SOURCE += $(SOURCE_DIR)/helperengine/helperengine_controls.cpp
helper_SOURCE += $(SOURCE_DIR)/helperengine/helperengine_sound.cpp
helper_SOURCE += $(SOURCE_DIR)/helperengine/helperengine_save.cpp
helper_SOURCE += $(SOURCE_DIR)/helperengine/helperengine_edit.cpp
helper_SOURCE += $(SOURCE_DIR)/map/map.cpp
helper_SOURCE += $(SOURCE_DIR)/dynamic/dynamic.cpp
helper_SOURCE += $(SOURCE_DIR)/dynamic/player.cpp
helper_SOURCE += $(SOURCE_DIR)/dynamic/npc.cpp
helper_SOURCE += $(SOURCE_DIR)/dynamic/spurdo.cpp
helper_SOURCE += $(SOURCE_DIR)/dynamic/lever.cpp
helper_SOURCE += $(SOURCE_DIR)/dynamic/door.cpp
helper_SOURCE += $(SOURCE_DIR)/dynamic/pressure_plate.cpp
helper_SOURCE += $(SOURCE_DIR)/dynamic/fireball.cpp
helper_SOURCE += $(SOURCE_DIR)/dynamic/sentry_gun.cpp
helper_SOURCE += $(SOURCE_DIR)/dynamic/rolling_stone.cpp
helper_SOURCE += $(SOURCE_DIR)/dynamic/vampire.cpp
helper_SOURCE += $(SOURCE_DIR)/dynamic/gondola.cpp
helper_SOURCE += $(SOURCE_DIR)/dynamic/spider.cpp
helper_SOURCE += $(SOURCE_DIR)/trigger.cpp
helper_SOURCE += $(SOURCE_DIR)/sound_command.cpp
helper_SOURCE += $(SOURCE_DIR)/message_command.cpp
helper_SOURCE += $(SOURCE_DIR)/area.cpp
helper_SOURCE += $(SOURCE_DIR)/event.cpp
helper_SOURCE += $(SOURCE_DIR)/functions.cpp
helper_SOURCE += $(SOURCE_DIR)/textbox.cpp
helper_SOURCE += $(SOURCE_DIR)/trigger.cpp

helper_HEADERS =
helper_HEADERS += $(HEADER_DIR)/helperengine/helperengine.h
helper_HEADERS += $(HEADER_DIR)/item.h
helper_HEADERS += $(HEADER_DIR)/map/map.h
helper_HEADERS += $(HEADER_DIR)/map/mapcontainer.h
helper_HEADERS += $(HEADER_DIR)/dynamic/dynamic_sounds.h
helper_HEADERS += $(HEADER_DIR)/dynamic/dynamic_default.h
helper_HEADERS += $(HEADER_DIR)/dynamic/alldynamics.h
helper_HEADERS += $(HEADER_DIR)/dynamic/dynamic.h
helper_HEADERS += $(HEADER_DIR)/dynamic/player.h
helper_HEADERS += $(HEADER_DIR)/dynamic/npc.h
helper_HEADERS += $(HEADER_DIR)/dynamic/spurdo.h
helper_HEADERS += $(HEADER_DIR)/dynamic/lever.h
helper_HEADERS += $(HEADER_DIR)/dynamic/door.h
helper_HEADERS += $(HEADER_DIR)/dynamic/pressure_plate.h
helper_HEADERS += $(HEADER_DIR)/dynamic/fireball.h
helper_HEADERS += $(HEADER_DIR)/dynamic/sentry_gun.h
helper_HEADERS += $(HEADER_DIR)/dynamic/rolling_stone.h
helper_HEADERS += $(HEADER_DIR)/dynamic/vampire.h
helper_HEADERS += $(HEADER_DIR)/dynamic/gondola.h
helper_HEADERS += $(HEADER_DIR)/dynamic/spider.h
helper_HEADERS += $(HEADER_DIR)/sound_command.h
helper_HEADERS += $(HEADER_DIR)/message_command.h
helper_HEADERS += $(HEADER_DIR)/area.h
helper_HEADERS += $(HEADER_DIR)/event.h
helper_HEADERS += $(HEADER_DIR)/functions.h
helper_HEADERS += $(HEADER_DIR)/trigger.h
helper_HEADERS += $(HEADER_DIR)/textbox.h

helper_OBJS =
helper_OBJS += $(SOURCE_DIR)/main.o
helper_OBJS += $(SOURCE_DIR)/trigger.o
helper_OBJS += $(SOURCE_DIR)/map/map.o
helper_OBJS += $(SOURCE_DIR)/sound_command.o
helper_OBJS += $(SOURCE_DIR)/message_command.o
helper_OBJS += $(SOURCE_DIR)/area.o
helper_OBJS += $(SOURCE_DIR)/event.o
helper_OBJS += $(SOURCE_DIR)/functions.o
helper_OBJS += $(SOURCE_DIR)/textbox.o
helper_OBJS += $(SOURCE_DIR)/helperengine/helperengine.o
helper_OBJS += $(SOURCE_DIR)/helperengine/helperengine_load.o
helper_OBJS += $(SOURCE_DIR)/helperengine/helperengine_draw.o
helper_OBJS += $(SOURCE_DIR)/helperengine/helperengine_runtime.o
helper_OBJS += $(SOURCE_DIR)/helperengine/helperengine_controls.o
helper_OBJS += $(SOURCE_DIR)/helperengine/helperengine_sound.o
helper_OBJS += $(SOURCE_DIR)/helperengine/helperengine_save.o
helper_OBJS += $(SOURCE_DIR)/helperengine/helperengine_edit.o
helper_OBJS += $(SOURCE_DIR)/dynamic/dynamic.o
helper_OBJS += $(SOURCE_DIR)/dynamic/player.o
helper_OBJS += $(SOURCE_DIR)/dynamic/npc.o
helper_OBJS += $(SOURCE_DIR)/dynamic/spurdo.o
helper_OBJS += $(SOURCE_DIR)/dynamic/lever.o
helper_OBJS += $(SOURCE_DIR)/dynamic/door.o
helper_OBJS += $(SOURCE_DIR)/dynamic/pressure_plate.o
helper_OBJS += $(SOURCE_DIR)/dynamic/fireball.o
helper_OBJS += $(SOURCE_DIR)/dynamic/sentry_gun.o
helper_OBJS += $(SOURCE_DIR)/dynamic/rolling_stone.o
helper_OBJS += $(SOURCE_DIR)/dynamic/vampire.o
helper_OBJS += $(SOURCE_DIR)/dynamic/gondola.o
helper_OBJS += $(SOURCE_DIR)/dynamic/spider.o


define newline


endef



%.o: %.cpp
	@echo $<
	@$(COMPILER) -c -o $@ $< $(COMPILER_FLAGS)


all: $(helper_OBJS)
	@$(COMPILER) -o $(BINARY_NAME) $(helper_OBJS) $(LIBS)
	@echo Done.



#	$(foreach F,$(helper_SOURCE),$(newline)echo "${F}" >> $@;)
#	@for s in $(helper_SOURCE); do $(COMPILER) -c $$s $(COMPILER_FLAGS); echo $$s; done


clean:
	@rm $(foreach F,$(helper_OBJS),$F)
	@rm -r $(BINARY_NAME)
