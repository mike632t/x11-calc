#
#  makefile - RPN (Reverse Polish) calculator simulator.
#
#  Copyright(C) 2019 - MT
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
#	Note separator (tab) at the beginning of the line CANNOT be a space..!
#
#	09 Oct 21   0.1   - Initial version - MT
#  22 Nov 21   0.2   - Added data files to backups - MT
#  04 Jan 22			- Added HP model 35 - MT
#  07 Jan 22         - Added HP model 80 - MT
#  07 Jan 22         - Added HP model 70 - MT
#  31 jan 22         - Added support for the HP10C, HP11C, HP12C, HP15C and
#                      HP16C (all are currently work in progress) - MT
#	31 Mar 22			- Moved ROMs into target folder - MT
#  11 Dec 22			- Renamed models with continious memory and added hp25
#                      hp33e, and hp38e - MT
#  23 Dec 22			- Changed the order in which simulators are built - MT
#

PROGRAM	=  x11-calc
FILES		=  ./src/*.c ./src/*.h ./prg/*.dat ./src/makefile
FILES		+= ./bin/*.rom
FILES		+= ./bin/x11-calc-*
FILES		+= *.md LICENSE makefile .gitignore .gitattributes
FILES		+= ./img/x11-calc-*.png
MAKE		=  make

all: clean hp35 hp45 hp70 hp80 hp10 hp21 hp22 hp25 hp25c hp27 hp29c hp67 hp31e hp32e hp33e hp33c hp34c hp37e hp38e hp38c hp10c hp11c hp12c hp15c hp16c

classic: clean hp35 hp80 hp45 hp70

woodstock: clean hp21 hp22 hp25 hp25c hp27 hp29c

spice: clean hp31e hp32e hp33e hp33c hp34c hp37e hp38e hp38c

voyager: clean hp10c hp11c hp12c hp15c hp16c


hp35:
	@$(MAKE) --no-print-directory -C ./src MODEL=35

hp80:
	@$(MAKE) --no-print-directory -C ./src MODEL=80

hp45:
	@$(MAKE) --no-print-directory -C ./src MODEL=45

hp70:
	@$(MAKE) --no-print-directory -C ./src MODEL=70

hp10:
	@$(MAKE) --no-print-directory -C ./src MODEL=10

hp21:
	@$(MAKE) --no-print-directory -C ./src MODEL=21

hp22:
	@$(MAKE) --no-print-directory -C ./src MODEL=22

hp25:
	@$(MAKE) --no-print-directory -C ./src MODEL=25

hp25c:
	@$(MAKE) --no-print-directory -C ./src MODEL=25c

hp27:
	@$(MAKE) --no-print-directory -C ./src MODEL=27

hp29c:
	@$(MAKE) --no-print-directory -C ./src MODEL=29c

hp67:
	@$(MAKE) --no-print-directory -C ./src MODEL=67

hp31e:
	@$(MAKE) --no-print-directory -C ./src MODEL=31e

hp32e:
	@$(MAKE) --no-print-directory -C ./src MODEL=32e

hp33e:
	@$(MAKE) --no-print-directory -C ./src MODEL=33e

hp33c:
	@$(MAKE) --no-print-directory -C ./src MODEL=33c

hp34c:
	@$(MAKE) --no-print-directory -C ./src MODEL=34c

hp37e:
	@$(MAKE) --no-print-directory -C ./src MODEL=37e

hp38e:
	@$(MAKE) --no-print-directory -C ./src MODEL=38e

hp38c:
	@$(MAKE) --no-print-directory -C ./src MODEL=38c

hp10c:
	@$(MAKE) --no-print-directory -C ./src MODEL=10c

hp11c:
	@$(MAKE) --no-print-directory -C ./src MODEL=11c

hp12c:
	@$(MAKE) --no-print-directory -C ./src MODEL=12c

hp15c:
	@$(MAKE) --no-print-directory -C ./src MODEL=15c

hp16c:
	@$(MAKE) --no-print-directory -C ./src MODEL=16c

clean:
	@rm  -f ./src/*.o

backup:
	@echo "$(PROGRAM)-`date +'%Y%m%d%H%M'`.tar.gz"; tar -czpf ..\/$(PROGRAM)-`date +'%Y%m%d%H%M'`.tar.gz $(FILES)

