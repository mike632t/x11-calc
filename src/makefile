#
#  makefile - RPN (Reverse Polish) calculator simulator.
#
#  Copyright(C) 2019 - MT
#
#  makefile.
#
#  This  program is free software: you can redistribute it and/or modify it
#  under  the terms of the GNU General Public License as published  by  the
#  Free  Software Foundation, either version 3 of the License, or (at  your
#  option) any later version.
#
#  This  program  is distributed in the hope that it will  be  useful,  but
#  WITHOUT   ANY   WARRANTY;   without even   the   implied   warranty   of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General
#  Public License for more details.
#
#  You  should have received a copy of the GNU General Public License along
#  with this program.  If not, see <http://www.gnu.org/licenses/>.
#
#  Note seperator (tab) at the beginning of the line CANNOT be a space..!
#
#  https://stackoverflow.com/questions/50941196/
#
#  01 May 13   0.1   - Initial version - MT
#  02 Jul 13         - Separated  code  and headers for button  and  colour
#                      manipulation into separate files - MT
#  09 Mar 14   0.2   - Separated  code for different calculator models into
#                      a  separate files (instead of using a single  source
#                      file with conditional compilation - MT
#  27 Sep 20   0.3   - Added make clean option - MT
#  08 Aug 21   0.4   - Removed x11-calc-format - MT
#  11 Aug 21   0.5   - Backup created in parent folder - MT
#  12 Sep 21   0.6   - Added '-no-pie' option so that the resulting program
#                      can be executed by double clicking on it - MT
#  23 Sep 21   0.7   - Model number can now be selected just by setting the
#                      MODEL number in this file - MT
#  26 Sep 21  0.8    - Added images to list of files (to backup) - MT
#  30 Sep 21  0.9    - Builds label and switch 'classes' - MT
#   3 Oct 21  0.10   - Fixed issue with backup file name - MT
#   9 Oct 21  0.11   - Moved source code and executable files into separate
#                      folders - MT
#  21 Jan 22         - Moved text messages to a separate file  - MT
#  22 jan 22         - Added a symbol for the current language - MT
#  24 Feb 22         - Clean now deletes ALL object files - MT
#  31 Mar 22         - Added additional libs and flags that are needed when
#                      building the project on NetBSD - MT
#  15 jan 23         - Delete execuitable before re-compiling - MT
#  18 Jan 23         - Allow VERBOSE and DEBUG options to be specified from
#                      the command line - MT
#                    - Conditionally sets operating specific options (don't
#                      need to comment them out when not required) - MT
#  01 May 23         - Fixed ordering of compiler options - MT
#

MODEL	= 21
PROGRAM	= x11-calc
SOURCES = x11-calc.c x11-calc-cpu.c x11-calc-display.c x11-calc-segment.c
SOURCES += x11-calc-button.c x11-calc-switch.c x11-calc-label.c x11-calc-colour.c
SOURCES += x11-keyboard.c x11-calc-messages.c gcc-wait.c
FILES	= *.c *.h LICENSE README.md makefile x11-calc-*.md .gitignore .gitattributes
FILES	+= x11-calc-*.png
OBJECTS	= $(SOURCES:.c=.o)
OUTPUT	= $(PROGRAM).out
LANG	= LANG_$(shell (echo $$LANG | cut -f 1 -d '_'))
UNAME	=  $(shell uname)

LIBS	= -lX11 -lm
FLAGS	= -fcommon -Wall -pedantic -ansi -std=gnu99
FLAGS	+= -Wno-comment -Wno-deprecated-declarations -Wno-builtin-macro-redefined

# Operating system specific settings
ifeq ($(UNAME), NetBSD) # NetBSD..
LIBS	+= -lcompat
FLAGS	+= -I /usr/X11R7/include/ -L /usr/X11R7/lib/ -R /usr/X11R7/lib
endif

# Compiler specific settings
ifeq ($(CXX), g++)
LIBS	+=  -no-pie
endif

ifdef DEBUG
FLAGS	+=  -g
endif

all: clean ../bin/$(PROGRAM)-$(MODEL) $(OBJECTS)
FLAGS	+= -D HP$(MODEL) -D $(LANG)
SOURCES += x11-calc-$(MODEL).c

../bin/$(PROGRAM)-$(MODEL): $(OBJECTS)
ifdef VERBOSE
	@echo
	@echo $(CC) $(OBJECTS) $(LIBS) -o ../bin/$@
	@echo
endif
	@$(CC) $(FLAGS) $(OBJECTS) -o ../bin/$@ $(LIBS)

$(OBJECTS) : $(SOURCES)
	@echo $(PROGRAM)-$(MODEL)
ifdef VERBOSE
	@echo
	@echo $(CC) $(FLAGS) -c $(SOURCES)
endif
	@rm -f ../bin/$(PROGRAM)-$(MODEL)
	@$(CC) $(FLAGS) -c $(SOURCES)

clean:
#	@rm -f $(PROGRAM)-$(MODEL).o
	@rm -f ../bin/$(OBJECTS) # -v
