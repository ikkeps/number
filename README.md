Description
===========

Hello world - like Linux kernel driver that allows to create devices similar to /dev/zero but with arbitrary symbol emmitted.

Compiling
=========

    make all
  
Usage
=====

As superuser execute:

    insmod number.ko
  
After that you can create devices invoking

    ./make_number zeroes 48
  
That will make */dev/zeroes* that emmit zeroes ("0" symbol).
