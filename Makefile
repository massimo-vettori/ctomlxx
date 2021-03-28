SUFFIXES += .d

CXX    = g++
CFLAGS = -pedantic -std=c++11 -Wall -Wextra -I includes/
DFLAGS = -MT $@ -MMD -MF .build/deps/$*.d

srcs := $(shell find src/ -name *.cpp) # tested only in linux
objs := $(patsubst src/%.cpp, .build/obj/%.o, $(srcs))
deps := $(patsubst src/%.cpp, .build/deps/%.d, $(srcs))

EXECUTABLE_NAME = test
LIBRARY_NAME    = libtoml.a


all: CFLAGS += -fpic
all: .build/lib/$(LIBRARY_NAME)

.build/lib/$(LIBRARY_NAME): $(objs)
	@echo " [Link]: ..... linking static library '$@'"
	@mkdir -p .build/lib/
	@ar crs $@ $^

.build/deps/%.d: src/%.cpp
	@mkdir -p $(dir $@)
	@echo " [Dependency]: creating dependency '$@'"
	@$(CXX) -MM -MT $(patsubst src%.cpp,.build/obj%.o,$<) $< -MF $@

.build/obj/%.o: src/%.cpp
	@mkdir -p $(dir $@)
	@echo " [Compile]: .. compiling target '$@'"
	@$(CXX) $(CFLAGS) $(DFLAGS) -c $< -o $@


clean:MAKEFLAGS += --no-builtin-rules
clean:MAKEFLAGS += --no-builtin-variables
clean:
	@echo " [Clean]: .... Removing object files"
	@$(RM) -r .build/obj
.PHONY: clean

reset:MAKEFLAGS += --no-builtin-rules
reset:MAKEFLAGS += --no-builtin-variables
reset:
	@echo " [Reset]: .... Removing all generated files"
	@$(RM) -r .build/
.PHONY: reset


## Debug ## ## ## ## ## ## ## ## ## ## ## ## ## ## ## ## ## ## ## ## ## ## ##

debug: CFLAGS += -ggdb -g3
debug: LFLAGS  = -ggdb -g3
debug: .build/$(EXECUTABLE_NAME)
.PHONY: debug

.build/obj/test.o: test/test.cpp includes/ctomlxx.h
	@mkdir -p .build/obj
	@$(CXX) -pedantic -ggdb -g3 -std=c++11 -Wall -Wextra -I ../includes/ -c $< -o $@


.build/$(EXECUTABLE_NAME): .build/obj/test.o $(objs)
	@echo " [Link]: ..... linking final executable '$@'"
	@$(CXX) $(LFLAGS) -o $@ $^

leak-check: debug
	@valgrind --track-origins=yes --leak-check=full -s --error-exitcode=1 .build/$(EXECUTABLE_NAME)
.PHONY: leak-check

test: debug
	@./.build/$(EXECUTABLE_NAME)
.PHONY: run

## Debug ## ## ## ## ## ## ## ## ## ## ## ## ## ## ## ## ## ## ## ## ## ## ##
-include $(deps)
