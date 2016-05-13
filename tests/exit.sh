#!/bin/sh
echo Changing directory to 'rshell'.
cd ..
echo "------Testing with 'ls && exit'------"
bin/rshell << EOF
echo "ls && exit"; ls && exit
EOF
echo "------Testing with 'ls -a || exit'------"
bin/rshell << EOF
echo "ls -a || exit"; ls -a || exit
EOF
echo "------Testing with 'exit mkdir'------"
bin/rshell << EOF
echo "exit mkdir"; exit mkdir
EOF
echo "------Testing with 'exit'------"
bin/rshell << EOF
echo "exit"; exit
EOF