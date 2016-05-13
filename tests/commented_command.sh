#!/bin/sh
echo Changing directory to 'rshell...'.
cd ..
bin/rshell << EOF
echo "ls #currently listing files in directory"; ls #currently listing files in directory
echo "#runningCommandsNow"; #runningCommandsNow
echo "echo #Hello World!"; echo # Hello World!
echo exit; exit
EOF

