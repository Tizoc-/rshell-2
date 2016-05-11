#!/bin/sh
cd ..
bin/rshell << EOF

ls
echo exit
exit
EOF

