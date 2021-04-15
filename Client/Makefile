C_FLAGS := -g -Wall -Wextra -Werror
CC := g++
RM := rm
LINKFLAGS := -lpthread

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

all: client 

debug:
	make "BUILD=debug"

# Files to compile
clientOBJ := client.o
clientOBJ := $(patsubst %,$(OBJ_PATH)%,$(clientOBJ))

# Build .o first
$(OBJ_PATH)%.o: $(SRC_PATH)%.cpp
	@echo [CC] $<
	@$(CC) $(C_FLAGS) -o $@ -c $< $(INC_PATH)                  

# Build .o first
$(OBJ_PATH)%.o: $(SRC_PATH)%.cpp
	@echo [CC] $<
	@$(CC) $(C_FLAGS) -o $@ -c $< $(INC_PATH)                  

# Build final Binary
client:      $(clientOBJ)
	@echo [INFO] Creating Binary Executable client
	@$(CC) $(C_FLAGS) -o $@ $^ $(LINKFLAGS)

# Clean all the object files and the binary
clean:   
	@echo "[Cleaning]"
	@$(RM) -rfv $(OBJ_PATH)*
	@$(RM) -rfv client
