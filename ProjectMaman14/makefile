program: program.o macro.o firstPass.o secondPass.o labels.o
	gcc -ansi -pedantic -Wall -g program.o macro.o firstPass.o secondPass.o labels.o -o program
program.o: program.c
	gcc -c -ansi -Wall -pedantic program.c -o program.o
macro.o: macro.c
	gcc -c -ansi -Wall -pedantic macro.c -o macro.o
firstPass.o: firstPass.c
	gcc -c -ansi -Wall -pedantic firstPass.c -o firstPass.o
secondPass.o: secondPass.c
	gcc -c -ansi -Wall -pedantic secondPass.c -o secondPass.o
labels.o: labels.c
	gcc -c -ansi -Wall -pedantic labels.c -o labels.o

