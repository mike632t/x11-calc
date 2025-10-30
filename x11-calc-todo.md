
## To Do List

### Planned changes

- [ ] Add a graphical printer display to the HP10

- [ ] Use a fixed window, display and key sizes (don't use scale with for CLASSIC or  KISS)

- [ ] Allow user to resize the window at runtime (scroll wheel?).

- [ ] Formatted program listings (using a separate python script)

- [ ] Reorganize x11-calc and simplify the switch handling

- [ ] Add HP65

- [ ] Draw switches showing position (and not just labels)

- [ ] Select default zoom based on screen resolution?

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

- [X] Allow user to specify window position.

  [https://tronche.com/gui/x/xlib/window/XMoveResizeWindow.html]::

- [X] Add card reader to HP67

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
