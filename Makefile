CARGO=cargo
CFLAGS=--release
LIB_PATH=$(shell pwd)/smartHome/plugins
LIBS=$(wildcard plugins/*)

ifeq ($(OS),Windows_NT)
    OS_detected := Windows
else
    OS_detected := $(shell uname -s)
endif

ifeq ($(OS_detected), Linux)
	LIBEXT=".so"
endif
ifeq ($(OS_detected), Darwin)
	LIBEXT=".dylib"
endif

all: $(LIBS) smartHome

run: $(LIBS) smartHome
	@cd smartHome && $(CARGO) run $(CFLAGS)

smartHome: FORCE
	@cd smartHome && \
	$(CARGO) build $(CFLAGS)

$(LIBS): FORCE
	@mkdir -p $(LIB_PATH)
	@cd $@ && find . -type f -name "*.so" -exec rm -f {} \;
	@cd $@ && $(CARGO) build $(CFLAGS) && find ./target -type f -name "*.so" -exec cp {} $(LIB_PATH) \;

FORCE:
