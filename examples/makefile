
all: .build/test

.build/test: .build/obj/test.o ../.build/lib/libtoml.a
	@g++ $< -o$@ -L../.build/lib/ -ltoml 

.build/obj/test.o: test.cpp ../includes/ctomlxx.h
	@mkdir -p .build/obj/
	@g++ -c -O3 -pedantic -Wall -std=c++11 $< -o$@  -I../includes/ 



run: all
	@.build/test

leak-check: .build/test
	@valgrind --track-origins=yes --leak-check=full --show-leak-kinds=all -s --error-exitcode=1 .build/test

clean:
	@rm -r .build/obj/

reset:
	@rm -r .build/
