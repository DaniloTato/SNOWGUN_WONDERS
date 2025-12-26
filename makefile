CXX ?= clang++
CXXFLAGS = -std=c++17 -Wall -Wextra -Wno-unused-parameter -O2

APP_NAME := snowgun
VERSION  := 0.1.0-alpha
ZIP_NAME := $(APP_NAME)-macos-$(VERSION).zip

UNAME_S := $(shell uname -s)

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
SFML_LIBS   := $(shell pkg-config --libs   sfml-graphics sfml-window sfml-system sfml-audio)

JSON_CFLAGS := $(shell pkg-config --cflags nlohmann_json 2>/dev/null)

CXXFLAGS += $(INCLUDES) $(SFML_CFLAGS) $(JSON_CFLAGS)

LDFLAGS  += $(SFML_LIBS) -Wl,-rpath,@executable_path/lib

SRC := $(shell find src scripts/src blueprints/src scene_builders/src -name '*.cpp')
OBJ := $(SRC:%.cpp=$(OBJDIR)/%.o)

all: $(BINDIR)/$(APP_NAME)

$(BINDIR)/$(APP_NAME): $(OBJ)
	mkdir -p $(BINDIR)
	$(CXX) $(OBJ) -o $@ $(LDFLAGS)

$(OBJDIR)/%.o: %.cpp
	mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

bundle-libs:
	mkdir -p $(LIBDIR)
	cp /opt/homebrew/opt/sfml@2.6/lib/libsfml-*.2.6.dylib $(LIBDIR)
	cp /opt/homebrew/opt/openal-soft/lib/libopenal*.dylib $(LIBDIR)

fix-libs:
	@for lib in $(LIBDIR)/*.dylib; do \
		install_name_tool -id @rpath/$$(basename $$lib) $$lib; \
	done

fix-bin:
	@otool -L $(BINDIR)/$(APP_NAME) | grep /opt/homebrew | awk '{print $$1}' | while read dep; do \
		install_name_tool -change $$dep @rpath/$$(basename $$dep) $(BINDIR)/$(APP_NAME); \
	done

package: all bundle-libs fix-libs fix-bin
	cp -R assets $(BINDIR)/
	cp -R config $(BINDIR)/

sign:
	codesign --force --deep --sign - $(BINDIR)/$(APP_NAME)
	codesign --force --sign - $(LIBDIR)/*.dylib
	@echo "Signed $(LIBDIR) libraries and $(APP_NAME) binary"

zip: package sign
	cd dist && zip -r ../$(ZIP_NAME) macos
	@echo "Created $(ZIP_NAME)"

run: all
	./$(BINDIR)/$(APP_NAME)

clean:
	rm -rf dist $(ZIP_NAME)

.PHONY: all clean run zip package bundle-libs fix-libs fix-bin