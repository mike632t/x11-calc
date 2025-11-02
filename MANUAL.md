<a id="top"></a>
## x11-calc - User Guide

### Contents

- [Starting](#starting)

- [Command line options](#command_line)

- [Operation](#operation)

<a id="starting"></a>
### Starting the emulator<sup><sup>[Back to Top](#top)</sup></sup>

Installing  the emulators on most modern desktops should create a new  menu
entry that will start the launcher script by default.  When invoked for the
first  time this will prompt you to select the default emulator as well  as
any additional command line options.

These selections will be saved and become the default settings.

When using some desktop environments (like GNOME and KDE) it is possible to
access a sub-menu that will allow you to select a specific model as well as
change the default settings by right clicking on the menu icon.

<a id="command_line"></a>
### Command line options<sup><sup>[Back to Top](#top)</sup></sup>

The  first time the application is launched from the menu the user will  be
prompted  to select a default model and given the opportunity to enter  any
additional  command line options.  These selections will become the default
when the application is started from the menu.

You can also specify command line options if you create your own menu items
or desktop icons, and when starting the program from a command prompt.

The following command line options are available:

```
Usage: x11-calc [OPTION]... [FILE] **

  -b  ADDR                 set break-point (octal)
  -i  OPCODE               set instruction trap (octal)
  -r  FILE                 load an alternate ROM
  -s                       single step
  -t                       trace
  -c, --comma              use a comma instead of a decimal point ***
      --cursor             display cursor
      --no-cursor          hide cursor
      --numlock            use numeric keypad (even if numlock is not on)
      --reset              reset to default **
      --zoom ZOOM          enlarge window size
      --help               display this help and exit
      --version            output version information and exit

**   Only available on models with continuous memory.
***  Only applicable to HP31, HP32E, HP33E, HP33C, HP34C, HP37E, HP38E and HP38C
```

#### [FILE]
When  starting  the emulator the name of the data file used to restore  the
saved state can be specified on the command line, allowing previously saved
copies  of programs to be loaded automatically when the program  starts  or
the emulator is reset using `Ctrl-C`.

#### -b ADDR
Sets a breakpoint at the specified address (octal).  The emulator will halt
when  it encounters the breakpoint and display the instruction allowing the
user to inspect the contents of the processor registers or continue.

Up to 10 breakpoints may be specified.

#### -i OPCODE
Sets  a breakpoint at the specified opcode (octal).  The emulator will halt
when in encounters an instruction in the firmware that matches the opcode.

### -r FILE
Provides the ability to load the firmware from a separate file.

The contents of each ROM is stored as pairs of values containing the memory
address and the opcode separated by a colon.

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

#### -t
Enables the user to trace the execution of the instructions in the firmware
when the emulator starts.

#### -c, --comma
Has the same effect as cutting the jumper on the power board which was used
to  control the appearance of the decimal point and thousands separator  on
the  HP31, HP32E, HP33E, HP33C, HP34C, HP37E, HP38E and HP38C.

Later models allow the user to select the display format from the keyboard.

#### --cursor (default)
Display the mouse cursor

#### --no-cursor
Hides the mouse cursor, useful when using the emulator with a touch screen.

#### --numlock
Forces the keys on the numeric keypad to behave as if numlock is on even if
it isn't (apparently some laptops don't have a numlock key!)

#### --zoom ZOOM
Specifying  a  value for zoom (1, 2, 3 or 4) will enlarge the size  of  the
window up to a maximum of 150%.

#### --help
Display the help text shown above and exits.

#### --version
Displays the version and licence information and exits.


<a id="operation"></a>
### Operation<sup><sup>[Back to Top](#top)</sup></sup>

#### Loading and Saving

Loading and saving is only possible on models with continuous memory.

The  contents of program memory and data registers are saved  automatically
when  the calculator is switched off or the window is closed, either in the
`$HOME/.local/share/x11-calc/` directory or in a hidden file in the  user's
HOME directory if `$HOME/.local/` does not exist.

Where  an on/off slide switch exists switching the calculator off will save
the current state.

You can create a copy the saved emulator state by making a copy of the file
which allow the user to make a backup when ever program was held in  memory
when  the  emulator was switched off.  Different programs can be loaded  by
specifying the name of the file on the command when starting the emulator.

If  the program was compiled with GTK support then a user can load or  save
programs  while the emulator is running by right clicking anywhere  in  the
application window.  This will prompt for the name of an existing data file
in  `run` mode and allow the current emulator state to be saved in when  in
`prgm` mode.

Note - Models with working card readers will be prompt the user to select a
read or write a program card file, instead of restoring the emulator state.

There is no validation when reading a file except that each record needs to
match the expected type (records that don't match the expected type will be
ignored) and the emulator will stop reading the file if the expected number
of records have already been processed.

Trying to read a file with invalid records usually results in an error, but
can  cause some very odd behaviour.  If this happens reset the emulator  to
it's  initial  state  using `Ctrl-C`, or close the window  and  restart the
emulator from the command line using the `--reset` option.

#### Exiting

For  models with a 'sliding' On/Off switch clicking on the switch will turn
the  emulator on or off, but if when switching off you hold down the  mouse
button for two seconds the program will exit.

#### Keyboard Shortcuts

The following keyboard shortcuts should work on Linux:

`0` - `9`, `+`. `-`. `*`. `/` and `Enter` should do what you expect them to
(when using numeric key pad you need to use numlock as usual).

`f` and where applicable `g` and `h` correspond to the shift keys.

`Esc` or `Backspace` corresponds to `Clx`, `c` to `CHS`, `e` to `EEX`, and on
financial models `n` and `i` correspond to `n` and `i` if not shifted.

`A` - `E` , `A` - `F` , or `A` - `B` correspond to program labels  (whether
shifted or not).

`Space` corresponds to `SST` (if not shifted).

The following control keys can also be used.

`Ctrl-C` Resets the emulator to its initial or last saved state.

`Ctrl-R` Prints the register contents.

`Ctrl-S` Enable tracing and executes a single instruction.

`Ctrl-T` Toggles tracing of the emulator code execution.

`Ctrl-Q` Resumes execution (does not disable trace).

`Ctrl-Z` Exits the emulator.

#### Debugging

You  can  start the emulation in trace mode using '-t', or in  single  step
mode using '-s', and set a break-point using '-b &lt;octal address&gt;'.

'Ctrl-T'  also toggles trace mode when running, 'Ctrl-S' executes the  next
instruction, 'Ctrl-Q' resumes execution, and 'Ctrl-R' displays the contents
of the CPU registers.

When in trace mode a jump to the same instruction produces no output.
