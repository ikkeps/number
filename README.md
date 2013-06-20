number
===========

Simple hello-world-like Linux kernel driver that allows to create devices similar to /dev/zero but with arbitrary byte emmitted.

Building
--------

    make all
  
Usage
-----

As superuser execute:

    insmod number.ko
  
After that you can create devices invoking

    ./make_number.sh zeroes 48
  
That will make */dev/zeroes* that emmit zeroes ("0" symbol).

TODO
----

Add `mmap` support (looks non-trivial for me now).
