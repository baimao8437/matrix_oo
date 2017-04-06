EXEC = \
    naive \
    submatrix \
    sse \
    sse_prefetch

EXEC := $(addprefix tests/test-matrix_,$(EXEC))

GIT_HOOKS := .git/hooks/applied
.PHONY: all
all: $(GIT_HOOKS) $(EXEC)

$(GIT_HOOKS):
	@scripts/install-git-hooks
	@echo

CC ?= gcc
CFLAGS = -Wall -std=gnu99 -g -O2 -I. -mavx2
LDFLAGS = -lpthread

SRCS_common = tests/test-matrix.c

METHOD_NUM = $(words $(EXEC))
REPEAT = 1000

tests/test-%: %.c
	$(CC) $(CFLAGS) -o $@ $(SRCS_common) $< stopwatch.c

check: $(EXEC)
	@for test in $^ ; \
	do \
		echo "Execute $$test..." ; $$test && echo "OK!\n" ; \
	done

cache-test: $(EXEC)
	@>time.txt
	@for method in $(EXEC); \
	do \
		printf "%s"$$method" "; \
		perf stat --repeat $(REPEAT) \
		-e cache-misses,cache-references ./$$method; \
		printf "\n"; \
	done >> time.txt

output.txt: cache-test calculate
	./calculate $(METHOD_NUM) $(REPEAT)

plot: output.txt
	gnuplot scripts/runtime.gp

calculate: calculate.c
	$(CC) $(CFLAGS_common) $^ -o $@ -lm

clean:
	$(RM) $(EXEC) calculate *.txt runtime.png
