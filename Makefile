SUFFIXES += .d

CXX    = g++
CFLAGS = -pedantic -std=c++11 -Wall -Wextra -I includes/ -fpic
DFLAGS = -MT $@ -MMD -MF .build/deps/$*.d

srcs := $(shell find src/ -name *.cpp) # tested only in linux
objs := $(patsubst src/%.cpp, .build/obj/%.o, $(srcs))
deps := $(patsubst src/%.cpp, .build/deps/%.d, $(srcs))

EXECUTABLE_NAME = parser # change this to the actual name of your executable
LIBRARY_NAME = libtoml.a


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

-include $(deps)

# Makefile created by Massimo Vettori (github - https://github.com/massimo-vettori)
# find it in its source gist: https://gist.github.com/massimo-vettori/adff1afd6b1b933cd77f782186bda038
