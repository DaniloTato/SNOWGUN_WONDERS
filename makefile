CXX ?= g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Wno-unused-parameter -g
LDFLAGS  =

APP_NAME := snowgun
VERSION  := 0.1.0
ZIP_NAME := $(APP_NAME)-macos-$(VERSION).zip

UNAME_S := $(shell uname -s)

INCLUDES = \
	-I./include \
	-I./include/ui \
	-I./scripts/include \
	-I./blueprints/include \
	-I./scene_builders/include

export PKG_CONFIG_PATH := /opt/homebrew/opt/openal-soft/lib/pkgconfig:$(PKG_CONFIG_PATH)

SFML_CFLAGS := $(shell pkg-config --cflags sfml-graphics sfml-window sfml-system sfml-audio)
SFML_LIBS   := $(shell pkg-config --libs   sfml-graphics sfml-window sfml-system sfml-audio)

ifeq ($(UNAME_S),Darwin)
	SFML_LIBS += -framework OpenAL
endif

JSON_CFLAGS := $(shell pkg-config --cflags nlohmann_json 2>/dev/null)

CXXFLAGS += $(INCLUDES) $(SFML_CFLAGS) $(JSON_CFLAGS)
LDFLAGS  += $(SFML_LIBS)

SRC := $(shell find src scripts/src blueprints/src scene_builders/src -name '*.cpp')
OBJ := $(SRC:%.cpp=dist/obj/%.o)

DEV_BIN  := $(APP_NAME)
DIST_BIN := dist/$(APP_NAME)

all: $(DEV_BIN)

$(DEV_BIN): $(OBJ)
	$(CXX) $(OBJ) -o $@ $(LDFLAGS)

run: all
	./$(DEV_BIN)

clean:
	rm -rf dist $(DEV_BIN)

dist: $(DIST_BIN)

$(DIST_BIN): $(OBJ)
	mkdir -p dist
	$(CXX) $(OBJ) -o $@ $(LDFLAGS)

dist-run: dist
	cd dist && ./$(APP_NAME)

dist-clean:
	rm -rf dist

dist/obj/%.o: %.cpp
	mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

dist-package: dist
	cp -R assets dist/
	cp -R config dist/
	mkdir -p dist/lib
	cp /opt/homebrew/opt/sfml@2.6/lib/libsfml-{graphics,window,system,audio}.2.6.dylib dist/lib/

zip: dist-package
	@echo "Creating release zip..."
	cd dist && zip -r ../$(ZIP_NAME) \
		$(APP_NAME) \
		assets \
		config \
		lib
	@echo "Created $(ZIP_NAME)"