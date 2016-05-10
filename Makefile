rshell : main.o Processes.o Base.o Andand.o Oror.o Command.o
	g++ -W -Wall -Werror -pedantic -ansi main.o Processes.o Base.o Andand.o Oror.o Command.o -o rshell

main.o : main.cpp Processes.cpp Processes.h
	g++ -W -Wall -Werror -pedantic -ansi -c main.cpp

Processes.o : Processes.cpp Processes.h Base.cpp Base.h Andand.cpp Andand.h Oror.cpp Oror.h Command.cpp Command.h
	g++ -W -Wall -Werror -pedantic -ansi -c Processes.cpp
	
Base.o : Base.cpp Base.h
	g++ -W -Wall -Werror -pedantic -ansi -c Base.cpp
	
Andand.o : Base.cpp Base.h Andand.cpp Andand.h
	g++ -W -Wall -Werror -pedantic -ansi -c Andand.cpp
	
Oror.o : Base.cpp Base.h Oror.cpp Oror.h
	g++ -W -Wall -Werror -pedantic -ansi -c Oror.cpp
	
Command.o : Base.cpp Base.h Command.cpp Command.h
	g++ -W -Wall -Werror -pedantic -ansi -c Command.cpp
	
clean : 
		rm  main.o Processes.o Base.o Andand.o Oror.o Command.o