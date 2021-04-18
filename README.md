<<<<<<< HEAD
# Project Details

Cryptogrhapic Algorithms in C++.

#### AES Encryption and Deryption Presentation

https://www.slideshare.net/AkashRanjandas1/advanced-encryption-standard-ug-reseacrh


#### Link: how to work with shared libraries in linux

https://www.geeksforgeeks.org/working-with-shared-libraries-set-2/


## Testing: 

https://drive.google.com/file/d/1AysOsildKzyqfBbAtOv7OGtbhDPr6Cz5/view?usp=sharing
=======
# Project Details

1. Cryptogrhapic Algorithms in C++.
2. Shared Libraries in Linux.
3. Bits manipulation using "bitset" stl

## [AES Encryption and Deryption Presentation](https://www.slideshare.net/AkashRanjandas1/advanced-encryption-standard-ug-reseacrh)


## [Link: how to work with shared libraries in linux](https://www.geeksforgeeks.org/working-with-shared-libraries-set-2/)


## [Block Cipher Modes Of Operation Presentation](https://www.slideshare.net/AkashRanjandas1/block-cipher-modes-of-operations)

## To dynamically link and load the shared library

$ g++ -shared -fPIC -o libaes.so AES/aes.cpp

$ export LD_LIBRARY_PATH=/home/******/Desktop/Ug_Research/BlockCipherOperations:$LD_LIBRARY_PATH

$ sudo mv libaes.so /usr/local/lib

$ cd /

$ cd /usr/local/lib

$ sudo ldconfig

$ g++ security_client.cpp -o security_client -laes

$ ./security_client enc aes-128-cbc -in filename -out enc_filename

$ ./security_client enc aes-128-cbc -in enc_filename null null

****** --> some file name in my system


## Using ldconfig

Once a shared library is created, copy the shared library to directory in which you want the library to reside (for example /usr/local/lib or /usr/lib). Now, run ldconfig command in this directory.

#### What does ldconfig do?

Linker name for shared library is a symbolic link to the fully qualified soname which in turn is a symbolic link to the real name. Well, this command does exactly the same.

When you run an ELF executable, by default the loader is run first. The loader itself is a shared object file /lib/ld-linux.so.X where ‘X’ is a version number. This loader in turn finds and loads all the shared libraries on which our program depends.

All the directories that are searched by the loader in order to find the libraries is stored in /etc/ld.so.conf. Searching all the directories specified in /etc/ld.so.conf file can be time consuming so every time ldconfig command is run, it sets up the required symbolic links and then creates a cache in file /etc/ld.so.cache where all the information required for executable is written. Reading information from cache is very less time consuming. The catch here is that ldconfig command needs to be run every-time a shared library is added or removed. So on start-up the program uses /etc/ld.so.cache to load the libraries it requires.
>>>>>>> 908d147137e3d74f1073f874be7757426f0e7062
