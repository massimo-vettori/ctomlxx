SUFFIXES += .d

CXX    = g++
CFLAGS = -pedantic -std=c++11 -Wall -Wextra -I includes/
DFLAGS = -MT $@ -MMD -MF .build/deps/$*.d

srcs := $(shell find src/ -name *.cpp) # tested only in linux
objs := $(patsubst src/%.cpp, .build/obj/%.o, $(srcs))
deps := $(patsubst src/%.cpp, .build/deps/%.d, $(srcs))

EXECUTABLE_NAME = parser # change this to the actual name of your executable


release: LFLAGS  = -O3 -s
release: CFLAGS += -O3 -s
release: .build/$(EXECUTABLE_NAME)
.PHONY: release

debug: LFLAGS  = -ggdb -g3
debug: CFLAGS += -ggdb -g3
debug: .build/$(EXECUTABLE_NAME)
.PHONY: debug

# You need to have valgrind installed in your machine to run this target
# if not run -> sudo apt install valgrind
leak-check: debug
	@valgrind --track-origins=yes --leak-check=full -s --error-exitcode=1 .build/$(EXECUTABLE_NAME)
.PHONY: leak-check

run:
	@.build/$(EXECUTABLE_NAME)
.PHONY: run


.build/$(EXECUTABLE_NAME): $(objs)
	@echo " [Link]: ..... linking final executable '$@'"
	@$(CXX) $(LFLAGS) -o $@ $^

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
