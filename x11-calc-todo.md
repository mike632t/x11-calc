
## To Do List

### Planned changes

- [x] Fix segmentation fault in display code (HP12C).

- [x] Update build instruction for Tru64 UNIX.

- [x] Update screenshots.

- [x] Implement display jumper setting in SPICE series.

- [x] Update help text.

- [x] Fix launcher help text display.

- [x] Update README (document new options).

- [x] Tidy up data structure definitions.

- [ ] Use variables for all error messages, and add missing translations.

  [https://superuser.com/questions/392439/lang-and-language-environment-variable-in-debian-based-systems]::

- [ ] Fix window resizing.
      Using window hints to resize the window when doesn't work every where environments

  [https://tronche.com/gui/x/xlib/window/XMoveResizeWindow.html]::

- [ ] Allow user to resize the window at runtime (scroll wheel?).

- [ ] Add logic for a  separate 'PIK chip' and move printer code.

- [ ] Finish debugging HP10 instruction set.

- [ ] Add new button styles for HP67 and HP1xC.

- [ ] Implement  different styles of digit (to allow for different  decimal
      point positions, and number of segments etc).

- [ ] Better segregation of common code (switch, label and keyboard).

- [ ] Translate README.

- [ ] Move VMS make script to top level directory.

- [ ] Implement the display decoder as a separate 'chip'.

- [ ] Expand tilde (~) in filenames.

  [https://stackoverflow.com/questions/40128376/how-to-expand-tilde-in-path]::

- [ ] Add HP19C.

- [ ] Implement 16 segment digit display.

- [ ] Add display decoder for HP41C

- [ ] Add support for HP41C?

### Rejected changes

- [ ] Allow any _programmable_ model to load a saved program.
      (Not possible without modifying the firmware)

