

all:
	gcc -g -Wall elfparser.c -o elfparser
	gcc -g -Wall test.c -o test
	gcc -g -Wall tracer.c -o tracer
	gcc -g -Wall tracer2.c -o tracer2
	gcc -g -Wall test2.c -o test2
	gcc -g -Wall code_inject.c -o code_inject 
	gcc -g -Wall -nostdlib payload.c -o payload

clean:
	rm elfparser test tracer tracer2 test2 code_inject payload 
