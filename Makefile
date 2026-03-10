CC = gcc
CFLAGS = -Wall -Wextra -O2 -static

# The output names
BIN_NAME = kamikaze
CPIO_NAME = kamikaze.cpio
ROOTFS_DIR = rootfs

.PHONY: all cpio clean

# default into binary
all: $(BIN_NAME)

# binary build go brrr
$(BIN_NAME): ./src/main.c ./src/config.h ./src/utils.c ./src/utils.h
	$(CC) $(CFLAGS) -o $(BIN_NAME) ./src/main.c ./src/utils.c

cpio: $(BIN_NAME)
	@echo "=> Building rootfs structure..."
	rm -rf $(ROOTFS_DIR)
	# technically kamikaze can create /dev if not exists,
	# but let's play safe here
	mkdir -p $(ROOTFS_DIR)/dev
	# Copy the binary as 'init' so the kernel executes it automatically
	cp $(BIN_NAME) $(ROOTFS_DIR)/init
	@echo "=> Packing $(CPIO_NAME)..."
	cd $(ROOTFS_DIR) && find . -print0 | cpio --null -o -H newc > ../$(CPIO_NAME) 2>/dev/null
	@echo "=> Done! $(CPIO_NAME) is ready."

clean:
	@echo "=> Cleaning up..."
	rm -rf $(BIN_NAME) $(CPIO_NAME) $(ROOTFS_DIR)
