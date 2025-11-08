SRC       = $(shell find src -name "*.c")
INCDIR    = $(shell find src -type d -exec echo -I{} \;)

BUILD_DIR = build
OUT_NATIVE = $(BUILD_DIR)/linux/game
OUT_WEB    = $(BUILD_DIR)/web/index.html

CC_NATIVE      = gcc
CFLAGS_NATIVE  = -Wall -std=c99 $(INCDIR)
LIBS_NATIVE    = -lraylib -lm -lpthread -ldl -lrt -lX11

CC_WEB         = emcc
INCDIR_WEB     = -Ilib
CFLAGS_WEB     = -O2 -Wall -DPLATFORM_WEB -s WASM=1 -s ALLOW_MEMORY_GROWTH=1 $(INCDIR_WEB) $(INCDIR)
LIBS_WEB       = ./lib/libraylib.web.a -s USE_GLFW=3 -s ASSERTIONS=0 -s GL_DEBUG=0 -s NO_EXIT_RUNTIME=1

.PHONY: all clean native web

all: native

native:
	@mkdir -p $(BUILD_DIR)/linux
	$(CC_NATIVE) $(CFLAGS_NATIVE) $(SRC) -o $(OUT_NATIVE) $(LIBS_NATIVE)
	./$(OUT_NATIVE)

web:
	@mkdir -p $(BUILD_DIR)/web
	$(CC_WEB) $(CFLAGS_WEB) $(SRC) $(LIBS_WEB) \
	--preload-file assets/audio@/assets/audio \
	--preload-file assets/textures@/assets/textures \
	--preload-file save \
	--shell-file src/minshell.html \
	-o $(OUT_WEB)

clean:
	rm -rf $(BUILD_DIR)
