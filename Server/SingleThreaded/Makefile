C_FLAGS := -g -Wall -Wextra -Werror -L ./libs/
CC := g++ -std=c++20
RM := rm
LINKFLAGS := -lpthread -lspdlog

.PHONY: $(TARGET)
.PHONY: clean

VPATH:= ./src/ ./obj/ ./include/

# Path for .c , .h and .o Files 
SRC_PATH := ./src/
OBJ_PATH := ./obj/
INC_PATH := -I ./include

ifeq ($(BUILD),debug)   
# "Debug" build - no optimization, and debugging symbols
C_FLAGS += -O0 -g -DDEBUG
else
# "Release" build - optimization, and no debug symbols
C_FLAGS += -O3 -s -DNDEBUG
endif

all: stServer tprServer tpServer ppServer edServer epollServer

debug:
	make "BUILD=debug"

# Files to compile
stServerOBJ := stServer.o configParser.o config.o
stServerOBJ := $(patsubst %,$(OBJ_PATH)%,$(stServerOBJ))

tprServerOBJ := tprServer.o configParser.o config.o
tprServerOBJ := $(patsubst %,$(OBJ_PATH)%,$(tprServerOBJ))

tpServerOBJ := tpServer.o configParser.o config.o
tpServerOBJ := $(patsubst %,$(OBJ_PATH)%,$(tpServerOBJ))

ppServerOBJ := ppServer.o configParser.o config.o
ppServerOBJ := $(patsubst %,$(OBJ_PATH)%,$(ppServerOBJ))

edServerOBJ := edServer.o configParser.o config.o
edServerOBJ := $(patsubst %,$(OBJ_PATH)%,$(edServerOBJ))

epollServerOBJ := epollServer.o configParser.o config.o
epollServerOBJ := $(patsubst %,$(OBJ_PATH)%,$(epollServerOBJ))

# Build .o first
$(OBJ_PATH)%.o: $(SRC_PATH)%.cpp
	@echo [CC] $<
	@$(CC) $(C_FLAGS) -o $@ -c $< $(INC_PATH)                  

# Build .o first
$(OBJ_PATH)%.o: $(SRC_PATH)%.cpp
	@echo [CC] $<
	@$(CC) $(C_FLAGS) -o $@ -c $< $(INC_PATH)                  

# Build stServer Binary
stServer:      $(stServerOBJ)
	@echo [INFO] Creating Binary Executable stServer
	@$(CC) $(C_FLAGS) -o $@ $^ $(LINKFLAGS)

# Build tprServer Binary
tprServer:      $(tprServerOBJ)
	@echo [INFO] Creating Binary Executable tprServer
	@$(CC) $(C_FLAGS) -o $@ $^ $(LINKFLAGS)

# Build tpServer Binary
tpServer:      $(tpServerOBJ)
	@echo [INFO] Creating Binary Executable tpServer
	@$(CC) $(C_FLAGS) -o $@ $^ $(LINKFLAGS)

# Build ppServer Binary
ppServer:      $(ppServerOBJ)
	@echo [INFO] Creating Binary Executable ppServer
	@$(CC) $(C_FLAGS) -o $@ $^ $(LINKFLAGS)

# Build edServer Binary
edServer:      $(edServerOBJ)
	@echo [INFO] Creating Binary Executable edServer
	@$(CC) $(C_FLAGS) -o $@ $^ $(LINKFLAGS)

# Build epollServer Binary
epollServer:      $(epollServerOBJ)
	@echo [INFO] Creating Binary Executable epollServer
	@$(CC) $(C_FLAGS) -o $@ $^ $(LINKFLAGS)

# Clean all the object files and the binary
clean:   
	@echo "[Cleaning]"
	@$(RM) -rfv $(OBJ_PATH)*
	@$(RM) -rfv stServer
	@$(RM) -rfv tprServer
	@$(RM) -rfv tpServer
	@$(RM) -rfv ppServer
	@$(RM) -rfv edServer
	@$(RM) -rfv epollServer
