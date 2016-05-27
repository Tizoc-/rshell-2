#!/bin/sh
echo "Test case 1: (echo A && echo B) || (echo C && echo D)";
echo "Test case 2: (echo A || echo B) && (echo C || echo D)";
echo "Test case 3: (echo A && ls) && (echo Hello || echo world)";
echo "Test case 4: (test bin/rshell || echo Doesn't Exist) && (test -d src && echo E)";
echo "Test case 5: (test binnnn && echo Doesn't Exist) || (echo D && echo E)";
echo "Test case 6: (test -d Randomfolder || exit) && (echo G && echo H)";
echo "Test case 7: (test -d tests || exit) && (echo Y && echo Z)";

echo "\nChanging directory to 'rshell...'"
cd ..
bin/rshell << EOF
echo "Test case 1:"; (echo A && echo B) || (echo C && echo D)
echo "Test case 2:"; (echo A || echo B) && (echo C || echo D)
echo "Test case 3:"; (echo A && ls) && (echo Hello || echo world)
echo "Test case 4:"; (test bin/rshell || echo Doesn't Exist) && (test -d src && echo E)
echo "Test case 5:"; (test binnnn && echo Doesn't Exist) || (echo D && echo E)
echo "Test case 6:"; (test -d Randomfolder || exit) && (echo G && echo H)
EOF
bin/rshell << EOF
echo "Test case 7:"; (test -d tests || exit) && (echo Y && echo Z)
echo "exit"; exit
EOF