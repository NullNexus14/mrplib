# mrplib Makefile -- Windows
CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c11 -Isrc/lib/include
LDFLAGS = -L.

LIB_SRCS = src/lib/mrp_rules.c src/lib/mrp_driver.c src/lib/libmrplib.c
LIB_OBJS = $(LIB_SRCS:.c=.o)
CLI_SRCS = src/cli/mrp-info.c src/cli/mrp-check.c src/cli/mrp-register.c
INST_SRC = src/installer/mrp-installer.c
INST_OBJ = src/installer/mrp-installer.o

.PHONY: all clean lib cli installer test

all: lib cli installer

lib: $(LIB_OBJS)
	$(CC) $(CFLAGS) -fPIC -shared -o libmrplib.dll $(LIB_OBJS)
	ar rcs libmrplib.a $(LIB_OBJS)
	@echo "Built: libmrplib.dll"

src/lib/%.o: src/lib/%.c
	$(CC) $(CFLAGS) -fPIC -DMRPLIB_BUILD -c $< -o $@

cli: lib $(CLI_SRCS)
	$(CC) $(CFLAGS) -o mrp-info src/cli/mrp-info.c -L. -lmrplib -Wl,-rpath,'$$ORIGIN'
	$(CC) $(CFLAGS) -o mrp-check src/cli/mrp-check.c -L. -lmrplib -Wl,-rpath,'$$ORIGIN'
	$(CC) $(CFLAGS) -o mrp-register src/cli/mrp-register.c -L. -lmrplib -Wl,-rpath,'$$ORIGIN'
	@echo "Built: CLI tools"

installer: cli
	@python3 src/installer/embed.py
	$(CC) $(CFLAGS) -static -o mrp-installer.exe $(INST_SRC)
	@echo "Built: mrp-installer.exe"

test: lib cli installer
	$(CC) $(CFLAGS) -o tests/test_basic tests/test_basic.c -L. -lmrplib -Wl,-rpath,.
	LD_LIBRARY_PATH=. ./tests/test_basic
	bash tests/test_full_pipeline.sh

clean:
	rm -f $(LIB_OBJS) $(INST_OBJ)
	rm -f libmrplib.dll libmrplib.a
	rm -f mrp-info mrp-check mrp-register
	rm -f mrp-installer.exe
	rm -f tests/test_basic
