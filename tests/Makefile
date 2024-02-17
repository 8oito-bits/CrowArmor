CC := gcc
FLAGS := -static -O2 

# Color codes
GREEN := $(shell tput setaf 2)
RED := $(shell tput setaf 1)
BLUE := $(shell tput setaf 4)
RESET := $(shell tput sgr0)

all: compile

compile:
	@echo "$(RESET)$(GREEN)[*]$(RESET) Compiling Tests for CrowArmor Driver ..."
	@$(CC) $(FLAGS) tests_syscall_hook.c -o tests_syscall_hook
	@echo "$(RESET)$(GREEN)[*]$(RESET) Compilation complete."

clean:
	@echo "$(RESET)$(RED)[*]$(RESET) Deleting Tests drivers compilations ..."
	@rm tests_syscall_hook

	