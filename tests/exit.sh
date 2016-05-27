#!/bin/sh
echo "Test case 1: ls && test -d tests && exit";
echo "Test case 2: test tests/exit.sh; exit";
echo "Test case 3: exit";
echo "Test case 4: echo B && exit #echo C";
echo "Test case 5: echo exit #exit";
echo "Test case 6: echo D || exit";

echo "\nChanging directory to 'rshell...'"
cd ..
echo "------Testing with 'ls && test -d tests && exit'------";
bin/rshell << EOF
echo "Test case 1:"; ls && test -d tests && exit
EOF
echo "------Testing with 'test tests/exit.sh; exit'------";
bin/rshell << EOF
echo "Test case 2:"; test tests/exit.sh; exit 
EOF
echo "------Testing with 'exit'------";
bin/rshell << EOF
echo "Test case 3:"; exit
EOF
echo "------Testing with 'echo B && exit #echo C'------";
bin/rshell << EOF
echo "Test case 4:"; echo B && exit #echo C
EOF
echo "------Testing with 'echo exit #exit'------";
bin/rshell << EOF
echo "Test case 5:"; echo exit #exit; exit
EOF
echo "------Testing with 'echo D || exit'------";
bin/rshell << EOF
echo "Test case 6:"; echo D || exit
echo Exiting 'exit.sh'; exit
EOF