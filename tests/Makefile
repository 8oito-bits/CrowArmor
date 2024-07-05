CC := gcc
FLAGS := -static -O2 -I../include

KDIR = /lib/modules/$(shell uname -r)/build

obj-m += tests_inspector.o

# Color codes
GREEN := $(shell tput setaf 2)
RED := $(shell tput setaf 1)
BLUE := $(shell tput setaf 4)
RESET := $(shell tput sgr0)

all: compile

compile:
	@echo "$(RESET)$(GREEN)[*]$(RESET) Compiling Tests for CrowArmor Driver ..."
	@$(CC) $(FLAGS) tests_syscall_hook.c -o tests_syscall_hook.elf
	@$(CC) $(FLAGS) tests_ioctl.c -o tests_ioctl.elf
	@make -C $(KDIR) M=$(PWD) modules
	@echo "$(RESET)$(GREEN)[*]$(RESET) Compilation complete."

clean:
	@echo "$(RESET)$(RED)[*]$(RESET) Deleting Tests drivers compilations ..."
	@make -C $(KDIR) M=$(PWD) clean
	@rm *.elf

install:
	@sudo insmod tests_inspector.ko

uninstall:
	@sudo rmmod tests_inspector.ko
