#!/bin/sh
echo Changing directory to 'rshell...'.
cd ..
bin/rshell << EOF
echo ls; ls
echo echo This is my comment.; echo This is my comment.
echo date; date
echo df -h; df -h
echo ls -f; ls -f
echo cal; cal
echo echo Our command shell works! :); echo Our command shell works! :)
echo exit; exit
EOF
