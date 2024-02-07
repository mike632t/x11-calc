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
#  https://www.gnu.org/software/make/manual/html_node/Prerequisite-Types.html
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
#  26 Mar 23         - Set compiler specific flags for gcc - MT
#  01 May 23         - Fixed ordering of compiler options - MT
#  22 Sep 23         - Added build options for MacOS - MT
#   5 Oct 23         - Creates target folder if required - MT
#                    - Lists the execuitable files instead of just the file
#                      names - MT
#   7 Feb 24         - Added new functions to check if a file or  directory
# 							  exists - MT
#

MODEL	= 21
PROGRAM	= x11-calc
SOURCES = x11-calc.c x11-calc-cpu.c x11-calc-display.c x11-calc-segment.c
SOURCES += x11-calc-button.c x11-calc-switch.c x11-calc-label.c x11-calc-colour.c
SOURCES += x11-keyboard.c x11-calc-messages.c gcc-wait.c gcc-exists.c
FILES	= *.c *.h LICENSE README.md makefile x11-calc-*.md .gitignore .gitattributes
FILES	+= x11-calc-*.png
OBJECTS	= $(SOURCES:.c=.o)
OUTPUT	= $(PROGRAM).out
LANG	= LANG_$(shell (echo $$LANG | cut -f 1 -d '_'))
UNAME	=  $(shell uname)
TARGET	=  ../bin

LIBS	= -lX11 -lm
FLAGS	= -fcommon -Wall -pedantic -std=gnu99
FLAGS	+= -Wno-comment -Wno-deprecated-declarations -Wno-builtin-macro-redefined

# Operating system specific settings
ifeq ($(UNAME), NetBSD) # NetBSD..
LIBS	+= -lcompat
FLAGS	+= -I /usr/X11R7/include/ -L /usr/X11R7/lib/ -R /usr/X11R7/lib
endif

ifeq ($(UNAME), Darwin) # MacOS
FLAGS	+= -I /opt/X11/include/X11/ -L /opt/X11/lib/
endif

# Compiler specific settings
ifeq ($(CC), cc)
LIBS	+=  -no-pie
FLAGS	+= -ansi
endif

ifdef DEBUG
FLAGS	+=  -g
endif

all: clean $(TARGET)/$(PROGRAM)-$(MODEL) $(OBJECTS)
FLAGS	+= -D HP$(MODEL) -D $(LANG)
SOURCES += x11-calc-$(MODEL).c

$(TARGET)/$(PROGRAM)-$(MODEL): $(OBJECTS)
ifdef VERBOSE
	@echo
	@echo $(CC) $(OBJECTS) $(LIBS) -o $@
	@echo
endif
ifeq ($(wildcard $(TARGET)/.),)
	@mkdir -p -v $(TARGET)
endif
	@$(CC) $(FLAGS) $(OBJECTS) -o $@ $(LIBS) && (cd $(TARGET) && ls --color $$(echo $@ | sed "s:$(TARGET)/::g") ) # Escape $ using $$...

$(OBJECTS): | $(TARGET)

$(TARGET):
	@mkdir $(TARGET)

$(OBJECTS) : $(SOURCES)
ifdef VERBOSE
	@echo
	@echo "***" $(PROGRAM)-$(MODEL)
	@echo
	@echo $(CC) $(FLAGS) -c $(SOURCES)
endif
	@rm -f $(TARGET)/$(PROGRAM)-$(MODEL)
	@$(CC) $(FLAGS) -c $(SOURCES)

clean:
#	@rm -f $(PROGRAM)-$(MODEL).o
	@rm -f $(TARGET)/$(OBJECTS) # -v
