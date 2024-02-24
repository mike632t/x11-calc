## x11-calc - Another RPN (Reverse Polish) calculator.

Emulators  for  the HP 35, HP 80, HP 45, HP 70, HP 21, HP 22, HP25, HP 25C,
HP 27,  HP 29C,  HP 31E, HP 32E, HP 33E, HP 33C,  HP 34C,  HP 37E,  HP 38C,
HP 38E, HP 67, HP 10C, HP 11C, HP 12C, HP 15C, and HP 16C.

All written in C using X11.

Use  of any language extensions or non standard language features has  been
avoided in order to try to make the code as portable as possible.

The  aim  is to have the same source code compile without  modification  on
Linux, VAX/VMS, and Tru64 Unix.

Unlike  all the other emulators the ROM images for the HP10C, HP11C, HP12C,
HP15C, HP16C are not defined in the the source code and must be loaded from
a separate ROM file.

![HP10](./img/x11-calc-10c.png) ![HP11](./img/x11-calc-11c.png)

More [screenshots](./img/#top)


### Latest News

24 Feb 24
   - By default the application will attempt to use the X11 base fonts. But
     if these are not available it will try to select an suitable alternate
     font instead from a predefined list.

     For all the fonts to be rendered as intended users should ensure  that
     the X11 base fonts are installed (see prerequsites).

16 Feb 24
   - For UNIX based systems the default location used to store the state of
     the machine when it is powered off has changed.
     If the data file already exists in `$HOME` then this will always be used
     by default, but if it does not exist then if `$XDG_DATA_HOME` is defined
     the program will create a sub directory in this location for the  data
     files or in or `$HOME/.local/share/` if `$XDG_DATA_HOME` is not defined or
     does not exist. If `$HOME/.local/share/` does not exist then the program
     will use `$HOME` as before.

01 Nov 23
   - Updated DCL make script.

14 Oct 23
   - Updated build instructions.
   - Uses a simpler display on ARM (excluding Apple).

22 Sep 23
   - Added ability to build using make on MacOS.

### Compiling

To  build the simulator on Linux check that you have all the  prerequisites
installed  then  download the source code from github and unzip it  (a  new
folder  will  be created to automatically).  Then change directory  to  the
new  folder run 'make all' build all the simulators.

e.g:
```
$ wget https://github.com/mike632t/x11-calc/archive/refs/heads/stable.zip
$ unzip x11-calc-stable.zip
$ cd x11-calc-stable
$ make all

$ ./bin/x11-calc-29c
x11-calc-29c: Version 0.10 [Commit ID: 399d546] 01 Nov 23 23:53:00 (Build: 0114)
ROM Size : 4096 words
```
If more than one C compiler is installed or if gcc is not available you can
specify which one to use from the command line.
```
$ make CC=clang VERBOSE=1 all

$ make CC=tcc VERBOSE=1 all
```

On  Tru64 UNIX a specific make file is used, and the execuitable files  are
saved in the same folder as the source.

e.g:
```
$ unzip x11-calc-stable.zip
$ cd x11-calc-stable
$ make -f makefile.tru64 all

$ ./src/x11-calc-29c
x11-calc-29c: Version 0.10 [Commit ID: 399d546] 01 Nov 23 23:53:00 (Build: 0114)
ROM Size : 4096 words
```

On VMS unzip the source code archive, then run the DCL `make` script.

e.g:
```
$ unzip x11-calc-stable.zip
$ set def [.x11-calc-stable.src]
$ @make all

$ mc [-.bin]x11-calc-29c
x11-calc-29c: Version 0.10 [Commit ID: 399d546] 02 Nov 23 23:52:11 (Build: 0114)
ROM Size : 4096 words
```

### Tested

- Debian 12 (Bookworm), clang 14.0.6, x64 + arm64

- Debian 12 (Bookworm), tcc 0.9.27, x64 + arm64

- Debian 12 (Bookworm), gcc 12.2.0, x64 + arm64

- Debian 11 (Bullseye), clang 11.0.1-2, x64

- Debian 11 (Bullseye), gcc 10.2.1, x64

- Debian 11 (Bullseye), tcc 0.9.27, x64

- Debian 10 (Buster), gcc 8.3.0, x64 + arm

- Debian 10 (Buster), clang 7.0.1, x64

- Debian 9 (Stretch), gcc 6.3.0, arm

- Debian 5 (Lenny), gcc 4.2.4, alpha

- Fedora 34, gcc 11.2.1, x64

- Gentoo, gcc 11.2.0, x64

- MacOS 10 (Catalina), clang 12.0.0, x64

- MacOS 13.4.1 (Venture), clang 14.0.3, arm64

- SUSE 15.4, clang 13. 0.1, x64

- SUSE 15.4, gcc 7.5.0, x64

- Ubuntu 20.04, gcc 9.4.0, x64

- Ubuntu 20.04, clang 10.0.0, x64

- Ubuntu 20.04, tcc 0.9.27, x64

- VAX/VMS 5.4-3, VAX C 3.2, VAX (simh)

- OpenVMS 9.2-1, VSI C 7.4-726, x64

- Windows 11 + WSL2, gcc 12.2.0, x64 + arm64


### Prerequisites

The following packages are required to build and/or run the simulator.

- Debian : gcc | clang | tcc, make, libx11-dev, libc6-dev, xfonts-base

- Fedora : gcc, make, libx11-dev, libc6-dev, xorg-x11-xfonts-base or xorg-x11-xfonts-misc

- Gentoo : gcc, make, libx11-dev, libc6-dev, font-misc-misc

- MacOS  : clang, make, [xquartz](https://www.xquartz.org/)

- SUSE   : gcc | clang, make, libX11-devel

- Ubuntu : gcc, make, libx11-dev, libc6-dev, xfonts-base

- Windows 11 + WSL2 : gcc, make, libx11-dev, libc6-dev, xfonts-base


### Keyboard Shortcuts

The following keyboard shortcuts should work on Linux:

'0' - '9', '+'. '-'. '*'. '/' and 'Enter' should do what you expect them to
(when using numeric key pad you need to use numlock as usual).

'f' and where applicable 'g' and 'h' correspond to the shift keys.

'Esc' or 'Backspace' corresponds to 'Clx', 'c' to CHS, 'e' to 'EEX', and on
financial models 'n' and 'i' correspond to 'n' and 'i' if not shifted.

On programmable models 'A' - 'E' correspond to the function keys where they
exist and 'Space' maps to 'SST' if not shifted.

'Ctrl-Z'  Quits,  and  'Ctrl-C' does a reset.  For models  with  continuous
memory 'Ctrl-Z' saves the current register contents, and 'Ctrl-C'  restores
them to the original saved state.


### Loading and saving

For  models with continuous memory the contents of program memory and  data
registers are saved in a hidden file in the users' HOME directory when  the
program  exits  or the calculator is switched off, and restored  from  this
hidden file when the simulator is loaded or reset using 'Ctrl-C'
```
~/.x11-calc-nn.dat
```
When  starting the simulator the name of the data file used to restore  the
saved state can be specified on the command line allowing previously  saved
copies of programs to be loaded automatically when the simulator starts  or
the  simulator is reset using 'Ctrl-C'.  However, any changes will be
saved in the hidden data file.


### Exiting

For  models with a 'sliding' On/Off switch clicking on the switch will turn
the simulator on or off, but if when switching off you hold down the switch
down for two seconds the program will exit.


### Debugging

You  can  start the simulation in trace mode using '-t', or in single  step
mode using '-s', and set a breakpoint using '-b &lt;octal address&gt;'.

'Ctrl-T'  also toggles trace mode when running, 'Ctrl-S' executes the  next
instruction, 'Ctrl-Q' resumes execution, and 'Ctrl-R' displays the contents
of the CPU registers.

When in trace mode a jump to the same instruction produces no output.


### ROM Images

No ROM images are included for the HP10C, HP11C, HP12C, HP15C, and HP16C.

The '-r <filename>' command line option provides the ability to use the ROM
contents held in an separate file.

For the HP10C, HP11C, HP12C, HP15C, and HP16C the ROM comprised of pairs of
hexadecimal values as address:opcode.
```
0000:107
0001:04e
0002:270
0003:238
0004:2ee
0005:13f
```
Other models include the ROM as part of the program, but you can specify an
alternate ROM comprising of pairs of octal values.
```
00000:00255
00001:01420
00002:00451
00003:01456
00004:01746
00005:00472
```
This allows you to use your own ROM images with any of the simulators.


### Known Issues

* Keyboard shortcuts only work on Linux.
* A 24 bit colour display is required.

##### HP 37E

* Fails self test.

##### HP 67

* Cannot read or write to magnetic cards.
* Has continuous memory.

### Raspberry Pi Specific Issues

* The code uses a simplified display on Arm based systems (except Apple) to
avoid the display refresh issues seen on the Raspberry Pi if either FKMS or
KMS overlays are enabled. (Note- Do not disable KMS on the latest Raspberry
Pi OS release).
* The window manager on recent Raspberry Pi OS releases doesn't disable the
ability  to resize or maximize a window if the maximum and  minimum  window
sizes are the same. The application will resize the window automatically it
the  user attempts to change the size but if it is maximized then the title
bar will be hidden (by the window manager) until the window is unmaximized.

### VMS Specific Issues

* Colour palette assumes a black and white display.
* Not all text is visible due to the limited colour palette and the DEC fonts
are missing some characters (for example the Pi symbol).

### Problem Reports

If you find problems or have suggestions relating to these simulators, then
please create a new [issue](https://github.com/mike632t/x11-calc/issues).

Your problem report should contain:

- Architecture (and VM host if applicable);

- Operating System and version;

- Desktop Environment and version;

- Window Manager and version;

- Compiler and version used;

- Commit ID;

- A description of the problem.

Thank you.
