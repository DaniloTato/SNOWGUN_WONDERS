# Compiler (override on command line or auto-detect)
ifeq ($(OS),Windows_NT)
    CXX := g++
    APP_NAME := snowgun.exe
    BINDIR := dist/windows
    OBJDIR := dist/obj

    # Update SFML_ROOT to your new SFML 64-bit installation path
    SFML_ROOT := C:/SFML-2.6.0

    INCLUDES = \
        -I$(SFML_ROOT)/include \
        -I./include \
        -I./include/ui \
        -I./scripts/include \
        -I./blueprints/include \
        -I./scene_builders/include

    LIBS = \
        -L$(SFML_ROOT)/lib \
        -lsfml-graphics \
        -lsfml-window \
        -lsfml-system \
        -lsfml-audio

    CXXFLAGS = -std=c++17 -Wall -Wextra -Wno-unused-parameter -O2 $(INCLUDES)
    LDFLAGS = $(LIBS)
else
    CXX ?= clang++
    APP_NAME := snowgun
    BINDIR := dist/macos
    LIBDIR := $(BINDIR)/lib
    OBJDIR := dist/obj

    INCLUDES = \
        -I./include \
        -I./include/ui \
        -I./scripts/include \
        -I./blueprints/include \
        -I./scene_builders/include

    export PKG_CONFIG_PATH := /opt/homebrew/lib/pkgconfig

    SFML_CFLAGS := $(shell pkg-config --cflags sfml-graphics sfml-window sfml-system sfml-audio)
    SFML_LIBS   := $(shell pkg-config --libs sfml-graphics sfml-window sfml-system sfml-audio)

    JSON_CFLAGS := $(shell pkg-config --cflags nlohmann_json 2>/dev/null)

    CXXFLAGS = -std=c++17 -Wall -Wextra -Wno-unused-parameter -O2 $(INCLUDES) $(SFML_CFLAGS) $(JSON_CFLAGS)
    LDFLAGS  = $(SFML_LIBS) -Wl,-rpath,@executable_path/lib
endif

SRC := $(shell find src scripts/src blueprints/src scene_builders/src -name '*.cpp')
OBJ := $(SRC:%.cpp=$(OBJDIR)/%.o)

all: $(BINDIR)/$(APP_NAME)

$(BINDIR)/$(APP_NAME): $(OBJ)
	mkdir -p $(BINDIR)
	$(CXX) $(OBJ) -o $@ $(LDFLAGS)

$(OBJDIR)/%.o: %.cpp
	mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

copy-assets:
	mkdir -p $(BINDIR)/assets
	mkdir -p $(BINDIR)/config
	cp -r assets/* $(BINDIR)/assets/
	cp -r config/* $(BINDIR)/config/

clean:
	rm -rf dist release *.zip

run: all copy-assets
ifeq ($(OS),Windows_NT)
	$(BINDIR)/$(APP_NAME)
else
	./$(BINDIR)/$(APP_NAME)
endif

.PHONY: all clean run