C_FLAGS := -g -Wall -Wextra -Werror -L ./libs/
CC := g++
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
C_FLAGS += -O0 -g
else
# "Release" build - optimization, and no debug symbols
C_FLAGS += -O3 -s -DNDEBUG
endif

all: stpServer 

debug:
	make "BUILD=debug"

# Files to compile
stpServerOBJ := server.o
stpServerOBJ := $(patsubst %,$(OBJ_PATH)%,$(stpServerOBJ))

# Build .o first
$(OBJ_PATH)%.o: $(SRC_PATH)%.cpp
	@echo [CC] $<
	@$(CC) $(C_FLAGS) -o $@ -c $< $(INC_PATH)                  

# Build .o first
$(OBJ_PATH)%.o: $(SRC_PATH)%.cpp
	@echo [CC] $<
	@$(CC) $(C_FLAGS) -o $@ -c $< $(INC_PATH)                  

# Build final Binary
stpServer:      $(stpServerOBJ)
	@echo [INFO] Creating Binary Executable stpServer
	@$(CC) $(C_FLAGS) -o $@ $^ $(LINKFLAGS)

# Clean all the object files and the binary
clean:   
	@echo "[Cleaning]"
	@$(RM) -rfv $(OBJ_PATH)*
	@$(RM) -rfv stpServer
