# Variables created for CrowArmor
KDIR = /lib/modules/$(shell uname -r)/build
DRIVER_PATH = $(PWD)/sources/crowarmor/crowarmor.ko

# Default variables for Makefile in Linux
obj-m += sources/crowarmor/crowarmor.o
sources/crowarmor/crowarmor-objs = sources/drm/drm_virtual_device.o \
							   sources/chrdev/chrdev.o \
							   sources/inspector/inspector.o \
							   sources/control_registers/cr4.o \
							   sources/control_registers/cr0.o \
							   sources/crowarmor/module.o \
							   sources/hook_syscall/hook.o \
							   sources/hook_syscall/syscall.o \
							   sources/crowarmor/crow.o \
							   sources/kpobres/kallsyms_lookup.o \

ccflags-y += -D_KERNEL -Wimplicit-fallthrough=0 \
	-Wimplicit-fallthrough=0 -I${PWD} -I${PWD}/include -I${PWD}/sources \

# Color codes
GREEN := $(shell tput setaf 2)
RED := $(shell tput setaf 1)
BLUE := $(shell tput setaf 4)
RESET := $(shell tput sgr0)

.PHONY: all clean infos help

all:
	@echo "$(RESET)$(GREEN)[*]$(RESET) Compiling the CrowArmor Driver ..."
	@make -C $(KDIR) M=$(PWD) modules
	@echo "$(RESET)$(GREEN)[*]$(RESET) Compilation complete."

clean:
	@echo "$(RESET)$(RED)[*]$(RESET) Deleting drivers compilations ..."
	@make -C $(KDIR) M=$(PWD) clean
	@echo "$(RESET)$(GREEN)[*]$(RESET) Deleting complete."


infos: check_driver
	@echo "$(RESET)$(BLUE)[*]$(RESET) Kernel version: $(shell uname -r)"
	@echo "$(RESET)$(BLUE)[*]$(RESET) CrowArmor Driver version: $(shell modinfo -F version $(DRIVER_PATH))"
	@echo "$(RESET)$(BLUE)[*]$(RESET) CrowArmor Driver srcversion: $(shell modinfo -F srcversion $(DRIVER_PATH))"
	@echo "$(RESET)$(BLUE)[*]$(RESET) CrowArmor Driver author: $(shell modinfo -F author $(DRIVER_PATH))"
	@echo "$(RESET)$(BLUE)[*]$(RESET) CrowArmor Driver license: $(shell modinfo -F license $(DRIVER_PATH))"

check_driver:
	@if [ ! -e $(DRIVER_PATH) ]; then \
		echo "$(RESET)$(RED)[!]$(RESET) CrowArmor Driver not found: $(DRIVER_PATH)"; \
		exit 1; \
	fi

help:
	@echo "$(RESET)$(GREEN)Available targets:$(RESET)"
	@echo "  $(RESET)$(GREEN)make all$(RESET)           : Compile the CrowArmor Driver"
	@echo "  $(RESET)$(GREEN)make clean$(RESET)         : Delete driver compilation"
	@echo "  $(RESET)$(GREEN)make infos$(RESET)         : Display CrowArmor Driver information"
	@echo "  $(RESET)$(GREEN)make help$(RESET)          : Display this help message"
	@echo "  $(RESET)$(GREEN)make install$(RESET)       : Install Driver in machine"
	@echo "  $(RESET)$(GREEN)make uninstall$(RESET)     : Uninstall Driver from machine"
	@echo "  $(RESET)$(GREEN)make tests_compile$(RESET) : Make Tests Driver"
	@echo "  $(RESET)$(GREEN)make tests_clean$(RESET)   : Delete Tests Driver"

install: check_driver
	@echo "$(RESET)$(BLUE)[*]$(RESET) Installing drivers ..."
	@sudo insmod $(DRIVER_PATH)
	@echo "$(RESET)$(BLUE)[*]$(RESET) Drivers Installed Successfully ..."

uninstall: check_driver
	@echo "$(RESET)$(BLUE)[*]$(RESET) Uninstalling driver ..."
	@sudo rmmod crowarmor
	@echo "$(RESET)$(BLUE)[*]$(RESET) Driver Uninstalled Successfully ..."

tests_compile: 
	@make -C tests compile

tests_clean:
	@make -C tests clean