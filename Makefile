BUILD_DIR := build
GENERATED_DIR := data/generated
GENERATED_DIRS := $(GENERATED_DIR) $(GENERATED_DIR)/textures $(GENERATED_DIR)/textures/replacement $(GENERATED_DIR)/textures/thumbnails

CC      := clang
LD      := ld.lld
TARGET  := $(BUILD_DIR)/mod.elf

VENV := .venv

ifeq ($(OS),Windows_NT)
VENV_BIN := $(VENV)/Scripts
else
VENV_BIN := $(VENV)/bin
endif

PYTHON := $(VENV_BIN)/python
PIP := $(VENV_BIN)/pip

LDSCRIPT := mod.ld
CFLAGS   := -target mips -mips2 -mabi=32 -O2 -G0 -mno-abicalls -mno-odd-spreg -mno-check-zero-division \
			-fomit-frame-pointer -ffast-math -fno-unsafe-math-optimizations -fno-builtin-memset \
			-Wall -Wextra -Wno-incompatible-library-redeclaration -Wno-unused-parameter -Wno-unknown-pragmas -Wno-unused-variable \
			-Wno-missing-braces -Wno-unsupported-floating-point-opt -Werror=section
CPPFLAGS := -nostdinc -D_LANGUAGE_C -DMIPS -DF3DEX_GBI_2 -DF3DEX_GBI_PL -DGBI_DOWHILE -I include -I include/dummy_headers \
			-I mm-decomp/include -I mm-decomp/src -I mm-decomp/extracted/n64-us -I mm-decomp/expected/build/n64-us -I mm-decomp/include/libc \
			-I data/generated -I src -DDEBUG
LDFLAGS  := -nostdlib -T $(LDSCRIPT) -Map $(BUILD_DIR)/mod.map --unresolved-symbols=ignore-all --emit-relocs -e 0 --no-nmagic

rwildcard=$(wildcard $1$2) $(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2))

C_SRCS := $(call rwildcard,src/,*.c)
C_OBJS := $(addprefix $(BUILD_DIR)/, $(C_SRCS:.c=.o))
C_DEPS := $(addprefix $(BUILD_DIR)/, $(C_SRCS:.c=.d))

BUILD_DIRS := $(addprefix $(BUILD_DIR)/,  $(sort $(dir $(C_SRCS))))

$(TARGET): $(C_OBJS) $(LDSCRIPT) | $(BUILD_DIR)
	$(LD) $(C_OBJS) $(LDFLAGS) -o $@

$(BUILD_DIR) $(BUILD_DIRS) $(GENERATED_DIRS):
ifeq ($(OS),Windows_NT)
	mkdir $(subst /,\,$@)
else
	mkdir -p $@
endif

$(C_OBJS): $(BUILD_DIR)/%.o : %.c | $(BUILD_DIR) $(BUILD_DIRS) data
	$(CC) $(CFLAGS) $(CPPFLAGS) $< -MMD -MF $(@:.o=.d) -c -o $@

$(VENV_BIN)/activate: requirements.txt
	python3 -m venv $(VENV)
	$(PIP) install -r requirements.txt
	touch $(VENV_BIN)/activate

$(GENERATED_DIR)/map_data.c: $(GENERATED_DIR) data/extract_data.py data/map.blend
ifeq ($(OS),Windows_NT)
	blender.exe ./data/map.blend --background --python data/extract_data.py 1> nul
else
	blender ./data/map.blend --background --python data/extract_data.py 1> /dev/null
endif

$(GENERATED_DIR)/overlay_data.c: $(GENERATED_DIRS) data/generate_overlays.py data/extract_overlays_data.py data/map.blend $(VENV_BIN)/activate
	$(PYTHON) ./data/generate_overlays.py

data: $(GENERATED_DIR)/map_data.c $(GENERATED_DIR)/overlay_data.c

clean:
	rm -rf $(BUILD_DIR)

-include $(C_DEPS)

.PHONY: clean data
