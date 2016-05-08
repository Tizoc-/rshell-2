a.out : main.o Base.o Processes.o Andand.o Oror.o Command.o
	g++ -W -Wall -Werror -pedantic -ansi main.o Processes.o Andand.o Oror.o Command.o -o a.out

main.o : main.cpp Processes.cpp Processes.h
	g++ -W -Wall -Werror -pedantic -ansi -c main.cpp

Base.o : Base.cpp Base.h
	g++ -W -Wall -Werror -pedantic -ansi -c Base.cpp

Processes.o : Base.cpp Base.h Processes.cpp Processes.h Command.cpp Command.h Andand.h Andand.cpp Oror.h Oror.cpp
	g++ -W -Wall -Werror -pedantic -ansi -c Processes.cpp
	
Andand.o : Base.cpp Base.h Andand.cpp Andand.h
	g++ -W -Wall -Werror -pedantic -ansi -c Andand.cpp
	
Oror.o : Base.cpp Base.h Oror.cpp Oror.h
	g++ -W -Wall -Werror -pedantic -ansi -c Oror.cpp
	
Command.o : Base.cpp Base.h Command.cpp Command.h
	g++ -W -Wall -Werror -pedantic -ansi -c Command.cpp