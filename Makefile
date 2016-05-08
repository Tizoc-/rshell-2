a.out : main.o Processes.o Sentinel.o Andand.o Oror.o Command.o
	g++ -W -Wall -Werror -pedantic -ansi main.o Processes.o Sentinel.o Andand.o Oror.o Command.o -o a.out

main.o : main.cpp Processes.cpp Processes.h
	g++ -W -Wall -Werror -pedantic -ansi -c main.cpp
	
Processes.o : Base.h Processes.cpp Processes.h Sentinel.cpp Sentinel.h Command.cpp Command.h Andand.h Andand.cpp Oror.h Oror.cpp
	g++ -W -Wall -Werror -pedantic -ansi -c Processes.o
	
Sentinel.o : Base.h Sentinel.cpp Sentinel.h
	g++ -W -Wall -Werror -pedantic -ansi -c Sentinel.o
	
Andand.o : Base.h Sentinel.cpp Sentinel.h Andand.cpp Andand.h
	g++ -W -Wall -Werror -pedantic -ansi -c Andand.o
	
Oror.o : Base.h Sentinel.cpp Sentinel.h Andand.cpp Andand.h
	g++ -W -Wall -Werror -pedantic -ansi -c Oror.o
	
Command.o : Base.h Command.cpp Command.h
	g++ -W -Wall -Werror -pedantic -ansi -c Command.o