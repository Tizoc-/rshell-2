#!/bin/sh
echo "Test case 1: test -e bin/Command.o"; 
echo "Test case 2: test -f bin/Command.o"; 
echo "Test case 3: test -d bin/Command.o"; 
echo "Test case 4: test src/Base.cpp"; 
echo "Test case 5: test -d bin"; 
echo "Test case 6: test -f bin"; 
echo "Test case 7: test -e bin/Random_File || echo 'Path does not exist'";
echo "Test case 8: test -e bin/rshell && echo 'Path exists!'";
echo "Test case 9: test -g bin/rshell";
echo "Test case 10: [ -e tests/precedence_test.sh ] || echo path exists";

echo "\nChanging directory to 'rshell...'"
cd ..
bin/rshell << EOF
echo "Test case 1:"; test -e bin/Command.o 
echo "Test case 2:"; test -f bin/Command.o 
echo "Test case 3:"; test -d bin/Command.o 
echo "Test case 4:"; test src/Base.cpp
echo "Test case 5:"; test -d bin
echo "Test case 6:"; test -f bin
echo "Test case 7:"; test -e bin/Random_File || echo 'Path does not exist'"
echo "Test case 8:"; test -e bin/rshell && echo 'Path exists'"
echo "Test case 9:"; test -g bin/rshell
echo "Test case 10:"; [ -e tests/test_test.sh ] || echo path exists
echo "exit"; exit
EOF