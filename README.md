Cryptogrhapic Algorithms in C++.

Commit1: Added the AES Key schedule algorithm.
create shared library(for linux only)
$ gcc -c -Wall -Werror -fpic AES/aes.cpp

$ gcc -shared -o libaes.so aes.o

$ gcc -L/$pwd -Wall -o driver driver.c -laes

[$pwd = /home/username/Desktop/Ug_Research/BlockCipherOperations]