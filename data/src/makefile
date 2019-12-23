TARGET1	=	server.out
TARGET2	=	client.out

BUILD_DIR ?= ../build

SRC_DIRS1 ?= ./server ./common
SRC_DIRS2 ?= ./client ./common
########################################################################################
##
########################################################################################
SRCS1 := $(shell find $(SRC_DIRS1) -name *.cpp)
OBJS1 := $(SRCS1:%=$(BUILD_DIR)/%.o)
DEPS1 := $(OBJS1:.o=.d)

SRCS2 := $(shell find $(SRC_DIRS2) -name *.cpp)
OBJS2 := $(SRCS2:%=$(BUILD_DIR)/%.o)
DEPS2 := $(OBJS2:.o=.d)
########################################################################################
##
########################################################################################
INC_DIRS1 := $(shell find $(SRC_DIRS1) -type d)
INC_FLAGS1 := $(addprefix -I,$(INC_DIRS1))
CPPFLAGS1 ?= $(INC_FLAGS1) -MMD -MP

INC_DIRS2 := $(shell find $(SRC_DIRS2) -type d)
INC_FLAGS2 := $(addprefix -I,$(INC_DIRS2))
CPPFLAGS2 ?= $(INC_FLAGS2) -MMD -MP
########################################################################################
##
########################################################################################
all: $(BUILD_DIR)/$(TARGET1) $(BUILD_DIR)/$(TARGET2)

LDFLAGS = -lSDL2 -lSDL2_image -lcwiid -lbluetooth -lSDL2_mixer -lSDL2_ttf -Wl,-rpath,/usr/local/lib

$(BUILD_DIR)/$(TARGET1): $(OBJS1)
	$(CXX) $(OBJS1) -o $@ $(LDFLAGS)

$(BUILD_DIR)/$(TARGET2): $(OBJS2)
	$(CXX) $(OBJS2) -o $@ $(LDFLAGS)
########################################################################################
##
########################################################################################
CXXFLAGS1 = -g
CXXFLAGS2 = -g -std=c++11

$(BUILD_DIR)/%.cpp.o: %.cpp
	$(MKDIR_P1) $(dir $@)
	$(CXX) $(CPPFLAGS1)$(CPPFLAGS2) $(CXXFLAGS1) -c $< -o $@

$(BUILD_DIR)/%.cpp.o: %.cpp
	$(MKDIR_P2) $(dir $@)
	$(CXX) $(CPPFLAGS2) $(CXXFLAGS2) -c $< -o $@
########################################################################################
##
########################################################################################
-include $(DEPS1)
MKDIR_P1 ?= mkdir -p

-include $(DEPS2)
MKDIR_P2 ?= mkdir -p









