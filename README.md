# x11-calc - Another RPN (Reverse Polish) calculator.

Written in C using X11. Work in progress, currently just a 'nice' GUI. 

The  intention is for the code to be reasonably portable.  Currently it will
compiles  and run on Linux using x64, arm, and i386 processors and VMS using
both VAX and Alpha processors.

# To Do

* Fix make file to use '-no-pie' on versions of Linux with a current version
  of gcc.
* Rewite the display code in a more structured form (like the button and LED
  segment code).
* Add code for named registers to each calculator model.
* Implement instruction parser methods for each calculator model.

* Everything else!

# Later
* Use a decimal number library.
* Sort out colour mapping to allow program to run on display with less  than
  24 -bit colour - may get round to this be one day...
