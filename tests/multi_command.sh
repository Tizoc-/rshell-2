#!/bin/sh
echo Changing directory to 'rshell'.
cd ..
bin/rshell << EOF
echo "ls -a echo Hello World!;"; ls -a; echo "Hello World!;" ##
echo "ls -a; echo hello && mkdir myfolder || echo world;"; ls -a; echo hello && mkdir myfolder || echo world;
echo "df -h || cal"; df -h || cal
echo "cal && df -h"; cal && df -h
echo exit; exit
EOF
