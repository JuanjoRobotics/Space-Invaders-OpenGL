CXX ?= g++
CXXFLAGS = -Iinclude

LDFLAGS = -Llib -lglfw3 -lopengl32 -lgdi32 -luser32 -llibassimp-5d -llibfreetype

SRCS = $(wildcard src/*.cpp src/*.c)

TARGET = game

# MSVC compatibility
ifeq ($(CXX),cl)
	CXXFLAGS = /Iinclude
	LDFLAGS = /link /LIBPATH:lib glfw3.lib opengl32.lib gdi32.lib user32.lib assimp-vc142-mt.lib
	TARGET := $(TARGET).exe
endif

all: $(TARGET) copy-dlls

debug: CXXFLAGS += -DENABLE_DEBUG
debug: clean all

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRCS) $(LDFLAGS)

copy-dlls:
	@copy lib\*.dll . >nul 2>&1

clean:
	@del /Q $(TARGET).exe >nul 2>&1
	@del /Q *.dll >nul 2>&1

.PHONY: all clean copy-dlls