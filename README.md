## x11-calc - Another RPN (Reverse Polish) calculator.

![HP21](./images/x11-calc-21.png) 

Written in C using X11.  

The use of any language extensions or non standard features has been avoided
in order to try to make the code as portable as possible.  Currently it will
compile using Linux running on x64, arm, or i386 processors.  

Earlier versions of the code would also compile on VMS running on either VAX
or Alpha processors, and Tru64 Unix running on Alpha.  This is not currently
possible but this capability will be restored in future releases.  

(Removing the keyboard handling routines should allow the current code to be
compiled on VMS).

Currently the only calculator that is simulated is an HP21 but the aim is to
add support for additional models when possible.

![HP32](./images/x11-calc-32.png) ![HP33](./images/x11-calc-33.png)


## To Do

* Sort out colour mapping to allow program to run on display with less  than
  24 -bit colour - may get round to this be one day...

* Finish processor simulation.
