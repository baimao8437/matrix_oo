EXEC = \
    naive \

EXEC := $(addprefix tests/test-matrix_,$(EXEC))

GIT_HOOKS := .git/hooks/applied
.PHONY: all
all: $(GIT_HOOKS) $(EXEC)

$(GIT_HOOKS):
	@scripts/install-git-hooks
	@echo

CC ?= gcc
CFLAGS = -Wall -std=gnu99 -g -O2 -I.
LDFLAGS = -lpthread

SRCS_common = tests/test-matrix.c

REPEAT = 10

tests/test-%: %.c
	$(CC) $(CFLAGS) -o $@ $(SRCS_common) $< stopwatch.c

cache-test: $(EXEC)
	@for method in $(EXEC); \
	do \
		printf "%s"$$method"\n";\
		perf stat --repeat $(REPEAT) \
		-e cache-misses,cache-references ./$$method; \
		printf "\n"; \
	done 

check: $(EXEC)
	@for test in $^ ; \
	do \
		echo "Execute $$test..." ; $$test && echo "OK!\n" ; \
	done

clean:
	$(RM) $(EXEC)
