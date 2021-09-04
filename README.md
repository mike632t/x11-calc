## x11-calc - Another RPN (Reverse Polish) calculator.

![HP21](./images/x11-calc-21.png) ![HP32](./images/x11-calc-32.png) ![HP33](./images/x11-calc-33.png)

Written in C using X11. Work in progress, currently just a 'nice' GUI,  with
animated  buttons.  I've created the data structures for the  CPU,  register
and  display  'objects'  but the  actual  implementation  of  the  processor
simulation is still work in progress.

The  intention is for the code to be reasonably portable.  Currently it will
compile on Linux running on an x64, arm, or i386 processor, VMS running on a
VAX or Alpha processor, and Tru64 Unix running on an Alpha processor.

## To Do

* Sort out colour mapping to allow program to run on display with less  than
  24 -bit colour - may get round to this be one day...

* Finish processor simulation.
