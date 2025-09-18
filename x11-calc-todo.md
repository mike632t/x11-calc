
## To Do List

### Planned changes

- [ ] Formatted program listings (using a separate python script)

- [ ] Add a graphical printer display to the HP10

- [ ] Reorganize x11-calc and simplify the switch handling

- [ ] Add HP65

- [ ] Add proper card reader to HP67

- [ ] Draw switches showing position (and not just labels)

- [ ] Support for very high resolution screens (select zoom based on screen
resolution if none specified?)

- [ ] Fix window resizing.  Using window hints to resize the window doesn't
work in every environment

  [https://tronche.com/gui/x/xlib/window/XMoveResizeWindow.html]::

- [ ] Allow user to resize the window at runtime (scroll wheel?).

- [ ] Better segregation of common code (switch, label and keyboard)

- [ ] Translate README - Volunteers needed!

- [ ] Move VMS make script to top level directory

- [ ] Expand tilde (~) in filenames

  [https://stackoverflow.com/questions/40128376/how-to-expand-tilde-in-path]::

- [ ] Add HP19C

- [ ] Implement 16 segment digit display

- [ ] Add display decoder for HP41C

- [ ] Add HP41C

### Completed changes

- [X] Added HP55 with timer mode

- [X] Optional menu (load/save) if GTK is installed

- [x] Optionally allow numeric keypad to be used if NumLock is off (`--numlock`)

- [x] Added HP10

- [x] Fix segmentation fault in display code (HP12C)

- [x] Update build instruction for Tru64 UNIX

- [x] Update screenshots

- [x] Implement  different styles of digit (to allow for different  decimal
point positions, and number of segments etc)

- [x] Implement display jumper setting in SPICE series

- [x] Update help text

- [x] Fix launcher help text display

- [x] Update README (document new options)

- [x] Tidy up data structure definitions

- [X] Use variables for all error messages, and add missing translations

  [https://superuser.com/questions/392439/lang-and-language-environment-variable-in-debian-based-systems]::

### Rejected changes

- [ ] Allow any _programmable_ model to load a saved program - Not possible without modifying the firmware

- [ ] Add logic for a  separate 'PIK chip' and move printer code - Not worth the effort to separate chip functions (yet)

- [ ] Implement the display decoder as a separate 'chip' - Not worth the effort to separate chip functions (yet)
