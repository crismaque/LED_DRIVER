USER_APP = bin/user_app
KERNEL_MODULE = kernel_module

SRC_DIR = src
TEMP_DIR = temp
KERNEL_DIR = /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)

all: dirs module user

dirs:
        @mkdir -p bin
        @mkdir -p temp

module:
                make -C $(KERNEL_DIR) M=$(PWD)/$(SRC_DIR) modules
                mv $(SRC_DIR)/$(KERNEL_MODULE).ko bin/
                find $(SRC_DIR) -maxdepth 1 -type f \
                        ! -name 'kernel_module.c' \
                        ! -name 'user_app.c' \
                        ! -name '*.h' \
                        ! -name '*.ko' \
                        ! -name 'Makefile' \
                        -exec mv -t $(TEMP_DIR)/ {} +
user:
                gcc -Isrc -o $(USER_APP) $(SRC_DIR)/user_app.c

link:
                ln -sf bin/user_app user_app

clean:
                make -C $(KERNEL_DIR) M=$(PWD)/$(SRC_DIR) clean
                rm -rf bin/*
                rm -rf temp/*
                rm user_app

install: module
                @sudo rmmod $(KERNEL_MODULE) 2>/dev/null || true
                @sudo dmesg -C
                @sudo insmod bin/$(KERNEL_MODULE).ko && dmesg | tail -6
                @sudo sleep 1
                @sudo mknod -m 666 /dev/led_gpio c $$(dmesg | grep 'registrado con numero mayor' | tail -1 |grep -oE '[0-9]+') 0 || true
                @sudo chmod 666 /dev/led_gpio

uninstall:
                sudo rmmod $(KERNEL_MODULE)

.PHONY: all module user clean install uninstall
