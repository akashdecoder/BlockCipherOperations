Cryptogrhapic Algorithms in C++.

Commit1: Added the AES Key schedule algorithm.

create shared library(for linux only)

$ g++ -c -Wall -Werror -fpic AES/aes.cpp

$ g++ -shared -o libaes.so aes.o

$ g++ -L/$pwd -Wall -o driver driver.c -laes

[$pwd = /home/username/Desktop/Ug_Research/BlockCipherOperations] --> directory structure of my system.