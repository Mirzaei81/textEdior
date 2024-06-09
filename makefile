IDIR :=  ./Include
LIBRARY_FILES:=./lib/

CFLAGS:= -Wall -Wextra -pedantic -std=c99 -g -O3 -I$(IDIR) -mtune=icelake-client

_DEPS:=gapbuffer.h
DEPS:=$(patsubst %,$(IDIR)/%,$(_DEPS))

ODIR=./build
SRCDIR=./src
SRC= $(wildcard $(SRCDIR)/*.c)
_OBJS:= $(addsuffix .o,$(basename $(notdir $(SRC))))
OBJS := $(patsubst %,$(ODIR)/%,$(_OBJS))
EXECUTABLE=foster
ifndef VERBOSE
.SILENT:
endif

$(ODIR)/%.o:$(SRCDIR)/%.c $(DEPS)
	$(CC) -c -fPIC  -o $@  $< $(CFLAGS) -I$(IDIR)

${EXECUTABLE}:$(OBJS)
	$(CC)   -o $@ $^ $(CFLAGS) -lm
#objcopy --only-keep-debug foster foster.debug
#objcopy --strip-debug foster #Uncomment to genrate debugg file

clean:
	rm -f $(ODIR)/*.o *~
	find . -name "*~" -exec rm {} \;
	find . -name "*.o" -exec rm {} \;
