a.out : main.o Processes.o Andand.o Oror.o Command.o
	g++ -W -Wall -Werror -pedantic -ansi main.o Processes.o Andand.o Oror.o Command.o -o a.out

main.o : main.cpp Processes.cpp Processes.h
	g++ -W -Wall -Werror -pedantic -ansi -c main.cpp
	
Processes.o : Base.h Processes.cpp Processes.h Command.cpp Command.h Andand.h Andand.cpp Oror.h Oror.cpp
	g++ -W -Wall -Werror -pedantic -ansi -c Processes.cpp
	
Andand.o : Base.h Andand.cpp Andand.h
	g++ -W -Wall -Werror -pedantic -ansi -c Andand.cpp
	
Oror.o : Base.h Oror.cpp Oror.h
	g++ -W -Wall -Werror -pedantic -ansi -c Oror.cpp
	
Command.o : Base.h Command.cpp Command.h
	g++ -W -Wall -Werror -pedantic -ansi -c Command.cpp