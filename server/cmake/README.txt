Then to buiuld server create folder

DeepTacToe/server/build without doxygen, change path to it and run

cmake ..
make -j4

DeepTacToe/server/build with doxygen, change path to it and run

cmake -DDEFINE_DOXYGEN_ENABLE=ON ..
make -j4

To create package run "make package" after "make -j4"

If doxygen was enabled run "make doxygen". This will produce
DeepTacToe/server/build/doxygen-html/index.html

At any given time it is ok to remove all files from
rm -rf DeepTacToe/server/build/*

And repeate from top of this file again
