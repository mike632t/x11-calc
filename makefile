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
#	Note seperator (tab) at the beginning of the line CANNOT be a space..!
#
#	01 May 13   0.1	- Initial version - MT
#  02 Jul 13         - Seperated code and headers for button and colour
#                      manipulation into separate files - MT
#  09 Mar 14   0.2   - Seperated  code for different calculator models into
#                      a  seperate files (instead of using a single  source
#                      file with conditional compilation - MT
#  27 Sep 20   0.3   - Added make clean option - MT
#  08 Aug 21   0.4   - Removed x11-calc-format - MT
#  11 Aug 21	0.5	- Backup created in parent folder - MT
#

PROGRAM 	= x11-calc
SOURCES 	= x11-calc.c x11-calc-21.c x11-calc-cpu.c x11-calc-display.c x11-calc-segment.c x11-calc-button.c x11-calc-colour.c gcc-wait.c
FILES		= *.c *.h LICENSE README.md makefile x11-calc-instruction-set.md .gitignore .gitattributes
OBJECTS	= $(SOURCES:.c=.o)
OUTPUT	= $(PROGRAM).out
LIBS		= -lX11 -lm
INCLUDE	= .
FLAGS		= -Wall -Wno-comment -pedantic -ansi
#FLAGS		+= -g # For debugging
CC			= gcc

$(PROGRAM): $(OBJECTS)
	@$(CC) $(FLAGS) $(OBJECTS) -o $@ $(LIBS)

$(OBJECTS) : $(SOURCES)
	@$(CC) $(FLAGS) -c $(SOURCES) -I $(INCLUDE)

all: $(PROGRAM) $(OBJECTS)

clean:
	@rm -f $(OBJECTS)
#	@rm -f $(PROGRAM)

backup:
	@echo "$(PROGRAM)-`date +'%Y%m%d%H%M'`.tar.gz"; tar -czpf ..\/$(PROGRAM)-`date +'%Y%m%d%H%M'`.tar.gz $(FILES)

