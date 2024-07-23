<a id="top"></a>
## x11-calc - Another RPN (Reverse Polish) calculator.

Simulators for  the HP 35, HP 80, HP 45, HP 70, HP 21, HP 22, HP25, HP 25C,
HP 27,  HP 29C,  HP 31E, HP 32E, HP 33E, HP 33C,  HP 34C,  HP 37E,  HP 38C,
HP 38E, HP 67, HP 10C, HP 11C, HP 12C, HP 15C, and HP 16C.

All written in C using X11.

Use  of any language extensions or non standard language features has  been
avoided in order to try to make the code as portable as possible.

The  aim  is to have the same source code compile without  modification  on
as  many systems and architectures as possible including Linux, VAX/VMS and
Tru64 Unix.

![HP10](./img/x11-calc-10c.png) ![HP11](./img/x11-calc-11c.png)

- [Latest News](#latest)

- [What will it run on](#tested)

- [How to get it](#obtaining)

- [Using it](#using)

- [Known issues](#issues)

- [Acknowledgements](#acknowledgements)

- [Submitting a problem report](#problems)

- [More screenshots](./img/#top)

<a id="latest"></a>
### Latest News <sup>[Back to Top](#top)</sup>

04 May 24

   - Fixed a bug that affected some simulators when compiled on MacOS.
   - Updated this README with more details of the keyboard shortcuts.
   - Added some much needed acknowledgements.

14 Apr 24

   - Allows the size of the window to be adjusted using the `--zoom n` option
     where n can be between 0 and 4.
   - Added an install option to the makefile (by default the installer will
     use `$HOME/.local` if it exists, but it is possible to specify another
     directory by setting the directory `prefix`).
   - Embedded missing firmware.

24 Feb 24
   - By default the application will attempt to use the X11 base fonts. But
     if these are not available it will try to select an suitable alternate
     font instead from a predefined list.

     For all the fonts to be rendered as intended users should ensure  that
     the X11 base fonts are installed (see prerequisites).

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

<a id="tested"></a>
### Tested platforms <sup>[Back to Top](#top)</sup>

The simulator has been successfully compiled and tested on:

   - Alpine 3.20, gcc 13.2.1, x64 + arm64

   - Alpine 3.19, gcc 13.2.1, x64 + arm64

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

   - Fedora 35, gcc 11.3.1, x64

   - Fedora 35, clang 13.0.1, x64

   - Fedora 39, gcc 13.2.1, x64

   - FreeBSD 14.0, clang 16.0.6, x64

   - Gentoo, gcc 11.2.0, x64

   - MacOS 10 (Catalina), clang 12.0.0, x64

   - MacOS 13.4.1 (Ventura), clang 14.0.3, arm64

   - MacOS 14.4.1 (Sonoma), clang 15.0.0, arm46

   - NetBSD 9.2, gcc 7.5.0, x64

   - SUSE 15.4, clang 13. 0.1, x64

   - SUSE 15.4, gcc 7.5.0, x64

   - Ubuntu 20.04, gcc 9.4.0, x64

   - Ubuntu 20.04, clang 10.0.0, x64

   - Ubuntu 20.04, tcc 0.9.27, x64

   - VAX/VMS 5.4-3, VAX C 3.2, VAX (simh)

   - OpenVMS 9.2-1, VSI C 7.4-726, x64

   - Windows 11 + WSL2, gcc 12.2.0, x64 + arm64

<a id="obtaining"></a>
### How to get it <sup>[Back to Top](#top)</sup>

You can either download the source code from GitHub and compile it yourself
or you can use a pre-compiled package.

   - [Building from the source](#building)

   - [Using binary package](#packages)

<a id="using"></a>
### Using the simulator <sup>[Back to Top](#top)</sup>

If you install the simulator on most modern desktops it should create a new
menu entry that will start the launcher script by default. When invoked for
the first time this will prompt you to select the default simulator as well
as any additional command line options.  These selections will be saved and
the selected simulator will become the new default.

When using some desktop environments (like GNOME and KDE) it is possible to
access a sub-menu that will allow you to select a specific model as well as
change the default settings by right clicking on the menu icon.

#### Keyboard Shortcuts

The following keyboard shortcuts should work on Linux:

'0' - '9', '+'. '-'. '*'. '/' and 'Enter' should do what you expect them to
(when using numeric key pad you need to use numlock as usual).

'f' and where applicable 'g' and 'h' correspond to the shift keys.

'Esc' or 'Backspace' corresponds to 'Clx', 'c' to CHS, 'e' to 'EEX', and on
financial models 'n' and 'i' correspond to 'n' and 'i' if not shifted.

'A' - 'E' , 'A' - 'F' , or 'A' - 'B' correspond to program labels  (whether
shifted or not).

'Space' maps to 'SST' (if not shifted.

The following control keys can also be used.

'Ctrl-Z' Exists the simulator. For models  with  continuous memory 'Ctrl-Z'
saves the current memory contents.

'Ctrl-C' Resets the simulator to its initial state.

'Ctrl-T' Toggles tracing of the simulator code execution.

'Ctrl-S' Enable tracing and executes a single instruction.

'Ctrl-Q' Resumes execution (does not disable trace).

#### Loading and saving

For  models with continuous memory the contents of program memory and  data
registers  are saved automatically when the calculator is switched  off  or
the  window is closed.  The current state of the simulator will be saved in
either `$HOME/.local/share/x11-calc/` or in a hidden file in the user's  HOME
directory if `$HOME/.local/` does not exist.

Resetting the simulator using 'Ctrl-C' will reload the saved state.
```
~/.x11-calc-nn.dat
```
When  starting the simulator the name of the data file used to restore  the
saved state can be specified on the command line allowing previously  saved
copies of programs to be loaded automatically when the simulator starts  or
the  simulator is reset using 'Ctrl-C'.  However, any changes will be
saved in the hidden data file.

#### Exiting

For  models with a 'sliding' On/Off switch clicking on the switch will turn
the simulator on or off, but if when switching off you hold down the switch
down for two seconds the program will exit.

#### Window Size

The size of the simulator window can be adjusted from the command line with
the `--zoom ZOOM` option, where the value for ZOOM can be in the range zero
to four (0-4).

#### Debugging

You  can  start the simulation in trace mode using '-t', or in single  step
mode using '-s', and set a break-point using '-b &lt;octal address&gt;'.

'Ctrl-T'  also toggles trace mode when running, 'Ctrl-S' executes the  next
instruction, 'Ctrl-Q' resumes execution, and 'Ctrl-R' displays the contents
of the CPU registers.

When in trace mode a jump to the same instruction produces no output.

#### ROM Images

The '-r <filename>' command line option provides the ability to use the ROM
contents from a separate file.  The contents of the ROM are stored as pairs
values separated by a colon containing the memory address and the opcode.

Anything appearing after a semi colon on each line is ignored.

For the HP10C, HP11C, HP12C, HP15C and HP16C the ROM file contains pairs of
_hexadecimal_ values.
```
0000:107
0001:04e
0002:270
0003:238
0004:2ee
0005:13f
```
For other models the ROM file contains pairs of _octal_ values.
```
00000:00255
00001:01420
00002:00451
00003:01456
00004:01746
00005:00472
```
When loading a ROM from file any gaps between the memory addresses will not
be filled with zeros, and the existing ROM contents will be left unchanged.

ROM files can therefore be used to load alternative version of the firmware
for a particular model or apply a patch to the existing firmware.

<a id="building"></a>
### Building from the source <sup>[Back to Top](#top)</sup>

#### Prerequisites

If you want to compile the simulator from the source code yourself then the
following packages must be installed.

   - Debian : gcc | clang | tcc  make libc6-dev libx11-dev xfonts-base

   - Fedora : gcc | clang  make glibc-devel libX11-devel xorg-x11-fonts-base | xorg-x11-fonts-misc

   - Gentoo : gcc make libc6-dev libx11-dev font-misc-misc

   - MacOS  : clang pmake [xquartz](https://www.xquartz.org/)

   - NetBSD : clang | gcc pmake

   - SUSE   : gcc | clang make libX11-devel

   - Ubuntu : gcc make libc6-dev libx11-dev xfonts-base

   - Windows 11 + WSL2 : gcc make libc6-dev libx11-dev xfonts-base

When all the prerequisites are available, you should be able to [compile](#Compiling) the
program using make if it is in the list of tested platforms.

#### Compiling

To  build the simulator check that you have all the [prerequisites](#Prerequisites) installed
then download the source code from github and unzip it (this will created a
new directory automatically).

e.g:
```
$ wget https://github.com/mike632t/x11-calc/archive/refs/heads/stable.zip
$ unzip x11-calc-stable.zip
```
Then change directory to the new folder.
```
$ cd x11-calc-stable
```
The to compile all the simulators you just need to invoke make.
```
$ make clean; make all
```
You can also compile a single simulator by specifying the model number.
```
$ make hp29c
```
By default the executable files will be created in the `bin` directory.
```
$ ./bin/x11-calc-29c
x11-calc-29c: Version 0.10 [Commit ID: 399d546] 01 Nov 23 23:53:00 (Build: 0114)
ROM Size : 4096 words
```
OR
```
$ bin/x11-calc
```
If more than one C compiler is installed or if gcc is not available you can
specify which one to use from the command line.
```
$ make CC=clang hp11c

$ make CC=tcc
```
##### VMS

On VMS unzip the source code archive, change the default directory and then
run `make.com`.

e.g:
```
$ unzip x11-calc-stable.zip
$ set def [.x11-calc-stable.src]
$ @make all

$ mc [-.bin]x11-calc-29c
x11-calc-29c: Version 0.10 [Commit ID: 399d546] 02 Nov 23 23:52:11 (Build: 0114)
ROM Size : 4096 words
```

#### Installing

On Linux systems after the compilation is complete you can use the makefile
to install the simulators locally.

By default the installer will use `$HOME/.local` if it exists, but it is
possible to specify another directory by setting the directory `prefix`.
```
$ make install

OR

$ make install prefix=/usr
```
The makefile also supports staged installs in a custom directory defined by
DESTDIR.
```
make DESTDIR=/tmp/staging install
```

<a id="packages"></a>
### Using a pre-compiled package <sup>[Back to Top](#top)</sup>

If you don't want to download an compile the sources yourself you can use a
pre-compiled binary package compatible with most distros from [Flathub](https://flathub.org/apps/io.github.mike632t.x11-calc) using Flatpak.

A native binary package is also available on Alpine Linux 3.20 release.\
If x11 is not already installed, add it as standalone (`setup-xorg-base`) or together with a standard desktop (`setup-desktop`).\
Make sure `community` repo is enabled and then install with `apk add x11-calc`.\
To leverage GUI for setup, install `apk add zenity`. Optional program saves may be installed with `apk add x11-calc-prg`.

<a id="issues"></a>
### Known Issues <sup>[Back to Top](#top)</sup>

#### General issues

- A 24 bit colour display is required.
- Keyboard shortcuts are not available on all systems.
- For best results you need to have the X windows core fonts installed.
- Parallel make only works on Linux, NetBSD and FreeBSD.

##### HP 11C + HP 12C + HP 15C + HP 16C

- Keyboard test is successful but these models do not pass the self-test.

##### HP 37E

- Fails self-test.

##### HP 67

- Cannot read or write to magnetic cards.
- Has  continuous memory (which allows saved programs to be loaded from the
command line).

#### Raspberry Pi Specific Issues

- The code uses a simplified display on Arm based systems (except Apple) to
avoid the display refresh issues seen on the Raspberry Pi if either FKMS or
KMS overlays are enabled. (Note- Do not disable KMS on the latest Raspberry
Pi OS release).

#### Wayland specific Issues

- Updating the display under Xwayland is very slow and even though all  the
simulators  work well on a Raspberry Pi3 using X11, on a Raspberry Pi4 with
Xwayland  it cannot redraw the display quickly enough to allow the  display
segments to be drawn with a shaded background.  As a result the display has
been simplified to make it work.
- The simulator window is supposed to be a fixed size. However when running
on Xwayland the window manager does not handle this correctly.

#### VMS Specific Issues

- Colour palette assumes a black and white display.
- Not all text is visible due to the limited colour palette.
- Not all key legends are shown as the font is missing some characters.

<a id="acknowledgements"></a>
### Acknowledgements <sup>[Back to Top](#top)</sup>

There are almost certainly some names I've missed off this list but without
the help and encouragement from several members of the calculator community
it is unlikely that this project would have happened at all or that I would
have managed to get as far as I have.

- `Teenix` for convincing me that it was possible in the first place.

- `Teenix` and `Panamatik` for their excellent simulators from which I was able
to figure out most of what should happen when each instruction is executed.

- `Agarza` for providing the details of the voyager displays.

- `Macmpi` for completely rewriting the makefiles and packaging the simulator
for Flatpak.

- `Macmpi` for packaging and testing the simulators on Alpine Linux

- `Agarza` and `Martin HEPPERLE` for translating the help text.

- `Brouhaha` for taking the time to explain the workings of various models.

- `Thomas KLEMM` for his testing and debugging efforts on MacOS.

- `Mark SHIN` for testing on MacOS.

- `Jonakeys` for getting the simulators working on FreeBSD.

<a id="problems"></a>
### Problem Reports <sup>[Back to Top](#top)</sup>

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
