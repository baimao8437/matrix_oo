EXEC = \
	$(OUT)/test-matrix \
	$(OUT)/test-stopwatch
    
GIT_HOOKS := .git/hooks/applied
OUT ?= .build
.PHONY: all
all: $(GIT_HOOKS) $(OUT) $(EXEC)

$(GIT_HOOKS):
	@scripts/install-git-hooks
	@echo

CC ?= gcc
CFLAGS = -Wall -std=gnu99 -g -O2 -I. -mavx2
LDFLAGS = -lpthread

SRCS_common = tests/test-matrix.c

OBJS := \
	stopwatch.o \
    matrix_naive.o \
    matrix_submatrix.o \
    matrix_sse.o \
    matrix_sse_prefetch.o 

deps := $(OBJS:%.o=%.o.d)
OBJS := $(addprefix $(OUT)/,$(OBJS))
deps := $(addprefix $(OUT)/,$(deps))


METHOD_NUM = $(shell echo $(words $(OBJS)) - 1 | bc)
REPEAT = 100

$(OUT)/%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ -MMD -MF $@.d $<

$(OUT)/test-%: tests/test-%.c $(OBJS) 
	$(CC) $(CFLAGS) -DREPEAT=$(REPEAT) -o $@ $^ $(LDFLAGS)

$(OBJS): | $(OUT)

$(OUT):
	@mkdir -p $@

check: $(EXEC)
	@for test in $^ ; \
	do \
		echo "Execute $$test..." ; $$test && echo "OK!\n" ; \
	done

output.txt: check calculate
	./calculate $(METHOD_NUM) $(REPEAT)

plot: output.txt
	gnuplot scripts/runtime.gp

calculate: calculate.c
	$(CC) $(CFLAGS_common) $^ -o $@ -lm

clean:
	$(RM) $(EXEC) $(OBJS) $(deps) calculate *.txt runtime.png
	@rm -rf $(OUT)

-include $(deps)
