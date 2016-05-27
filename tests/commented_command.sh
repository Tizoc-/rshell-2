#!/bin/sh
echo "Test case 1: echo Hello World #echo Goodbye"; 
echo "Test case 2: ls #&& echo A";
echo "Test case 3: test -e bin/Random #|| echo 'File does not exist.'";
echo "Test case 4: test -d bin && #test -e bin/Base.o;"
echo "Test case 5: test -d bin/rshell #ls";

echo "\nChanging directory to 'rshell...'"
cd ..
bin/rshell << EOF
echo "Test case 1:"; echo Hello World #echo Goodbye
echo "Test case 2:"; ls #&& echo A
echo "Test case 3:"; test -e bin/Random #|| echo 'File does not exist.'
echo "Test case 4:"; test -d bin #&& echo B
echo "Test case 5:"; test -d bin/rshell #ls
echo exit; exit
EOF

