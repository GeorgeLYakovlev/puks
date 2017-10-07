CC = g++
LD = g++
AR = ar
#CFLAGS = -std=c++11 -Wall `pkg-config --cflags gtk+-3.0`
CFLAGS = -std=c++11 -Wall `pkg-config --cflags gtkmm-3.0` -DDEBUG -g
LFLAGS = -std=c++11
#LIBS = -lpthread `pkg-config --libs gtk+-3.0`
LIBS = -lpthread `pkg-config --libs gtkmm-3.0`
ARFLAGS = rvs
LIB_DIRS = .

MODULES   := app hardware ui utils
SRC_DIR   := $(addprefix src/,$(MODULES))
BUILD_DIR := $(addprefix build/,$(MODULES))

DEPS := $(foreach sdir, $(SRC_DIR), $(wildcard $(sdir)/*.h))
DEPS += src/version.h
SRC := $(foreach sdir, $(SRC_DIR), $(wildcard $(sdir)/*.cc))
OBJ := $(patsubst src/%.cc, build/%.o, $(SRC))

INCLUDES := -I src
#LDIRS := $(addprefix -L ../,$(LIB_DIRS))

vpath %.cc $(SRC_DIR)

define make-goal
$1/%.o: %.cc
	$(CC) $(CFLAGS) $(INCLUDES) -c $$< -o $$@
endef

.PHONY: all checkdirs debug clean

debug: CFLAGS += -DDEBUG -g
debug: checkdirs puks

all: checkdirs puks

checkdirs: $(BUILD_DIR)

$(BUILD_DIR):
	@mkdir -p $@

puks: $(OBJ)
	@echo DEPS: $(DEPS)
	@echo OBJ: $(OBJ)
	$(LD) $(LDIRS) $(LFFLAGS) -o $@ $^ $(LIBS)

clean:
	rm -rf $(BUILD_DIR)

$(foreach bdir,$(BUILD_DIR),$(eval $(call make-goal,$(bdir))))

