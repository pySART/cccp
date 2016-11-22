vpath %.c ../src

#OBJDIR=.
PROGRAMS = ccp 

ccp_SRC = ccp.c main.c
ccp_OBJS = $(ccp_SRC:%.c=%.o)


ifeq ($(OS),Windows_NT)
	TARGET=messaging.dll
	ccp_TARGET=ccp.dll
	EXT=.dll
else
	UNAME_S := $(shell uname -s)
	ifeq ($(UNAME_S),Linux)
		TARGET=messaging.so
		ccp_TARGET=ccp.so
		EXT=.do
	endif
	ifeq ($(UNAME_S),Darwin)
		TARGET=messaging.dylib
		ccp_TARGET=ccp.dylib
		EXT=.dylib
	endif
endif

# TARGET = $(addsuffix $(EXT), messaging)

INCLUDE= -I . -I ../inc
CPPFLAGS += $(INCLUDE)
CFLAGS += -g -fPIC #TODO: conditional: -fprofile-arcs -ftest-coverage --coverage
LDFLAGS= -shared -fPIC #-Wl,-Map=ccp.map


all: $(PROGRAMS)

%.o: %.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $(notdir $@)


ccp: $(ccp_OBJS)
	@echo Linking $(ccp_TARGET)
	$(CC) $(CFLAGS) -o $(ccp_TARGET) $(notdir $(ccp_OBJS)) $(LDFLAGS)

#%.dll:
#	@echo Linking $(ccp_TARGET)
#	$(CC) $(CFLAGS) -o $(ccp_TARGET) $(notdir $(ccp_OBJS)) $(LDFLAGS)

define DLL_template =
 @echo "Und nun? Linking"
 $(1): $$($(1)_OBJS)
# 	$(CC) $(CFLAGS) -o $$($(1)_target) $notdir $$($(1)_OBJS) $(LDFLAGS)
 ALL_OBJS   += $$($(1)_OBJS)
endef

#$(foreach prog,$(PROGRAMS),$(eval $(call DLL_template,$(prog))))

#$(PROGRAMS):
#	$(LINK.o) $^ $(LDLIBS) -o $@

clean:
	rm -vf *o *dll *exe
	#rm -f $(ALL_OBJS) $(PROGRAMS
	
.PHONY: clean all

# Target-specific variables.
#gui.o: CPPFLAGS += -DUSE_NEW_MALLOC=1
#gui.o: gui.h
#        $(COMPILE.c) $(OUTPUT_OPTION) $<
