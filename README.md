<a id="top"></a>

[![latest packaged version(s)](https://repology.org/badge/latest-versions/x11-calc.svg)](https://repology.org/project/x11-calc/versions)

## x11-calc - Another RPN (Reverse Polish) calculator

Emulators  for  the HP 35, HP 80, HP 45, HP 70, HP 55, HP 21, HP 22, HP 25,
HP 25C,  HP 27,  HP 29C, HP 31E, HP 32E, HP 33E,  HP 33C,  HP 34C,  HP 37E,
HP 38C, HP 38E, HP10, HP 67, HP 10C, HP 11C, HP 12C, HP 15C, and HP 16C.

All written in C using X11.

Use  of any language extensions or non standard language features has  been
avoided in order to try to make the code as portable as possible.

The  aim  is to have the same source code compile without  modification  on
as  many  systems  and architectures as possible including  Linux,  NetBSD,
MacOS, VMS, Solaris and Tru64 Unix.

 ![HP67](./img/x11-calc-67.png) ![HP25](./img/x11-calc-25.png) ![HP34C](./img/x11-calc-34c.png)
 ![HP10](./img/x11-calc-10c.png) ![HP11](./img/x11-calc-11c.png)

### Contents <sup><sup>[Back to Top](#top)</sup></sup>

- [Changes](#changes)

- [Building/Installing](#obtaining)

- [Tested Systems](#tested)

- [User Guide](./GUIDE.md#top)

- [Known issues](./GUIDE.md#issues)

- [Acknowledgements](#acknowledgements)

- [Submitting a problem report](#problems)

- [More screenshots](./img/#top)

<a id="changes"></a>
### Changes <sup><sup>[Back to Top](#top)</sup></sup>

Details of the latest/important changes.

18 Oct 25

   - Added support for the HP67 card reader (requires GTK).

14 Sep 25

   - Added the HP55 complete with timer.

05 Sep 25

   - Fixed issues that prevented the file selection dialog from being shown
     when using Flatpak.

24 Aug 25

   - HP37E now passes self test.
   - Loading  or saving the current state can now be done by right clicking
     on  the application window.  In `run` mode the user will be prompted  to
     load the state from a previously saved `.dat` (data) file, while in `prgm`
     mode the user will be prompted to save the current state.

09 Aug 25

   - Improved display updates (using double buffering).
   - Minor changes to fix compilation issues on VAX and DEC Windows.

08 Jul 25

   - Finally managed to get the HP10 working!!
     Note that there is no graphical display for the printer, the output is
     to `stdout` on the comsole.  This allows the output from the printer  to
     be redirected to a file if required.

07 Jul 25

   - Fixed regression bug that affected HP12C.

30 Jun 25

   - Enable the use of the `-c`, or `--comma` command line option to modify
     the state of the hardware link that controls decimal point format.

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

13 Jun 13

   - Initial version can draw the application window using X11!

<a id="obtaining"></a>
### Building/Installing <sup><sup>[Back to Top](#top)</sup></sup>

You can either download the source code from GitHub and compile it yourself
or you can use a pre-compiled package.

   - [Building from the source](#building)

   - [Using binary package](#packages)

<a id="building"></a>
### Building from the source <sup><sup>[Back to Top](#top)</sup></sup>

#### Prerequisites

If you want to compile the emulators from the source code yourself then the
following packages must be installed.

   - Alpine   : gcc | clang | tcc  make libc6-dev libx11-dev xorg-x11-fonts-base [gtk+3.0-dev]

   - Debian   : gcc | clang | tcc  make libc6-dev libx11-dev xfonts-base [libgtk-3-dev | libgtk2.0-dev]

   - Fedora   : gcc | clang  make glibc-devel libX11-devel xorg-x11-fonts-base | xorg-x11-fonts-misc [gtk3-devel]

   - Gentoo   : gcc make libc6-dev libx11-dev font-misc-misc [x11-libs/gtk+]

   - MacOS    : [Xcode](https://developer.apple.com/xcode/) [Xquartz](https://www.xquartz.org/)

   - NetBSD   : clang | gcc pmake

   - SUSE     : gcc | clang make libX11-devel

   - Solaris 10 : SUNWgcc SUNWgccruntime SUNWtoo SUNWhea SUNWxwhl SUNWgmake

   - Ubuntu : gcc | clang make libc6-dev libx11-dev xfonts-base [libgtk-3-dev]

   - Windows 11 + WSL2 : gcc make libc6-dev libx11-dev xfonts-base

When all the prerequisites are available, you should be able to [compile](#Compiling) the
program using make (or gmake).

Note - GTK is an optional dependency and is only required to allow users to
load or save programs.  This will not stop the program from running by this
functionality  will be unavailable if the GTK libraries are  not  available
when the program is compiled.

#### Compiling

To  build the emulators check that you have all the [prerequisites](#Prerequisites) installed
then download the source code from github and unzip it (this will created a
new directory automatically).

##### Linux/NetBSD

```
$ wget https://github.com/mike632t/x11-calc/archive/refs/heads/stable.zip
$ unzip stable.zip
```
Then change directory to the new folder.
```
$ cd x11-calc-stable
```
To compile all the emulators you just need to invoke make.
```
$ make clean; make all
```
You can also compile an emulator by specifying the model number.
```
$ make hp67
```
By default the executable files will be created in the `bin` directory.
```
$ bin/x11-calc-67
x11-calc-67: Version 0.19.0198 [Commit Id : 751ab27] gcc version 8.3.0 19 Oct 25 20:51:51
ROM Size: 8192 words
Loading '/home/system/.local/share/x11-calc/x11-calc-67.dat'
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

##### Tru64 UNIX

To compile all the emulators you just need to invoke make.
```
$ make clean; make all
```
You can also compile a single emulator by specifying the model number.
```
$ make hp29c
```
By default the executable files will be created in the `bin` directory.
```
$ bin/x11-calc-29c
```

##### Solaris 10

You  need to be able to use the GNU tools so you have need to include their
location in your PATH.
```
$ PATH=$PATH:/usr/sfw/bin/
```
Download the source code, unzip it, and change to the new directory.
```
$ wget https://github.com/mike632t/x11-calc/archive/refs/heads/stable.zip
$ unzip stable.zip
$ cd x11-calc-stable
```
Since `gcc` isn't the default compiler you need to specify it when invoking
`gmake`.
```
$ gmake CC=gcc clean hp21
```
```
$ gmake CC=gcc clean all
```
By default the executable files will be created in the `bin` directory.
```
$ ./bin/x11-calc-33c
x11-calc-33c: Version 0.19.0198 [Commit ID: 992cf145] gcc version 3.4.3 19 Oct 25 20:32:48
ROM Size: 4096 words
```

##### VMS

On VMS unzip the source code archive, change the default directory and then
run `make.com`.
```
$ unzip stable.zip
$ set def [.x11-calc-stable.src]
$ @make all

$ mc [-.bin]x11-calc-29c
x11-calc-29c: Version 0.10 [Commit ID: 399d546] 02 Nov 23 23:52:11 (Build: 0114)
ROM Size : 4096 words
```

#### Installing

On Linux systems after the compilation is complete you can use the makefile
to install the emulators locally.

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
### Using a pre-compiled package <sup><sup>[Back to Top](#top)</sup></sup>

If  you  don't  want  to compile the application yourself  you  can  use  a
pre-compiled flatpak [package](https://flathub.org/apps/io.github.mike632t.x11-calc).

A native binary package is also available on Alpine Linux 3.20 release.

If x11 is not already installed, add it as standalone (`setup-xorg-base`) or together with a standard desktop (`setup-desktop`).
Make sure `community` repo is enabled and then install with `apk add x11-calc`.
To leverage GUI for setup, install `apk add zenity`. Optional program saves may be installed with `apk add x11-calc-prg`.

<a id="tested"></a>
### Tested Systems<sup><sup>[Back to Top](#top)</sup></sup>

The emulators have been successfully compiled and tested on:

   - Alpine 3.20, gcc 13.2.1, x64 + arm64 **

   - Alpine 3.19, gcc 13.2.1, x64 + arm64 ***

   - Debian 12 (Bookworm), clang 14.0.6, x64 + arm64

   - Debian 12 (Bookworm), tcc 0.9.27, x64 + arm64

   - Debian 12 (Bookworm), gcc 12.2.0, x64 + arm64

   - Debian 11 (Bullseye), clang 11.0.1-2, x64 ***

   - Debian 11 (Bullseye), gcc 10.2.1, x64 ***

   - Debian 10 (Buster), gcc 8.3.0, arm **

   - Debian 10 (Buster), gcc 8.3.0, x64

   - Debian 10 (Buster), clang 7.0.1, x64

   - Debian 10 (Buster), tcc 0.9.27, x64

   - Debian 9 (Stretch), gcc 6.3.0, arm ***

   - Debian 5 (Lenny), gcc 4.2.4, alpha

   - Debian 5 (Lenny), gcc 4.3.2, i686

   - Fedora 35, gcc 11.3.1, x64

   - Fedora 35, clang 13.0.1, x64

   - Fedora 39, gcc 13.2.1, x64

   - Fedora 39, clang 17.0.6, x64

   - Fedora 41, gcc 14.3.1, x64

   - Fedora 41, clang 19.1.7, x64

   - Fedora 42, gcc 15.2.1, x64

   - Fedora 42, clang 20.1.8, x64

   - FreeBSD 14.0, clang 16.0.6, x64 **

   - Gentoo, gcc 11.2.0, x64 **

   - MacOS 10.1.15 (Catalina), clang 12.0.0, x64 **

   - MacOS 12.7.6 (Monterey), clang 14.0.0, x64

   - MacOS 13.4.1 (Ventura), clang 14.0.3, arm64 **

   - MacOS 14.4.1 (Sonoma), clang 15.0.0, arm46 **

   - Minix 3.4.0, clang 3.6, x86

   - NetBSD 9.2, gcc 7.5.0, x86

   - NetBSD 9.2, clang 16.0.6, x86

   - SunOS 5.10, gcc 3.4.3, x86

   - SUSE 15.4, clang 13. 0.1, x64

   - SUSE 15.4, gcc 7.5.0, x64

   - Tru64 5.1, cc 6.5-011, alpha

   - Ubuntu 20.04, gcc 9.4.0, x64 + aarch64

   - Ubuntu 20.04, clang 10.0.0, x64 + aarch64

   - Ubuntu 20.04, tcc 0.9.27, x64 + aarch64

   - Ubuntu 22.04, gcc 11.4.0, x64

   - Ubuntu 24.04, gcc 13.3.0, x64

   - VMS 5.4-3, VAX C 3.2, VAX (simh)

   - VMS 7.3-2, C 6.5-001, alpha

   - VMS 9.2-1, C 7.4-726, x64 ***

   - Windows 11 + WSL2, gcc 12.2.0, x64 + arm64 **

** Not yet retested with this build.

*** Previously tested but no plans to retest.

**** Compiles with warnings

<a id="acknowledgements"></a>
### Acknowledgements <sup><sup>[Back to Top](#top)</sup></sup>

There are almost certainly some names I've missed off this list but without
the help and encouragement from several members of the calculator community
it is unlikely that this project would have happened at all or that I would
have managed to get as far as I have.

- `Teenix` for convincing me that it was possible in the first place.

- `Teenix`  and `Panamatik` for their excellent emulators from which I  was
   able to figure out most of what should happen when each  instruction  is
   is executed.

- `Agarza` for providing the details of the voyager displays.

- `Macmpi` for completely rewriting the make files and maintaining backward
   compatibility with Tru64 UNIX while doing so!

- `Macmpi` for packaging the emulators for Flatpak and setting up the build
   so new releases are built automatically.

- `Macmpi` for packaging and testing the emulators on Alpine Linux.

- `Agarza` and `Martin HEPPERLE` for helping to translate the help text.

- `Brouhaha` for taking the time to explain the workings of various models.

- `Thomas KLEMM` for his testing and debugging efforts on MacOS.

- `Mark SHIN` for testing on MacOS.

- `Jonakeys` for getting the emulators working on FreeBSD.
Virginia Giuffre
- `Quozl` for helping with double buffering display changes.

- `Kjellc` for adding support for the HP67 card reader.

- 'Vttoth` for allowing me to include some of his example programs.

- `Kjellc` for translating the help text (into Swedish).

<a id="problems"></a>
### Problem Reports <sup><sup>[Back to Top](#top)</sup></sup>

If  you encounter an issue or have suggestions for future changes to  these
emulators, then please create a new [issue](https://github.com/mike632t/x11-calc/issues).

Your problem report should contain:

- Computer model (or hypervisor);

- Processor architecture;

- Operating System and version;

- Desktop Environment and version;

- Window Manager and version;

- If you are using a pre-built binary (Alpine / Flatpak);

- Compiler and version used (if applicable);

- Commit ID;

- A description of the problem.

Thank you.
