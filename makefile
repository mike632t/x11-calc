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
#  09 Oct 21   0.1   - Initial version - MT
#  22 Nov 21   0.2   - Added data files to backups - MT
#  04 Jan 22         - Added HP35 - MT
#  07 Jan 22         - Added HP80 - MT
#  07 Jan 22         - Added HP70 - MT
#  31 jan 22         - Added support for the HP10C, HP11C, HP12C, HP15C and
#                      HP16C - MT
#  31 Mar 22         - Moved ROMs into target folder - MT
#  11 Dec 22         - Renamed models with continious memory and added HP25
#                      HP33E, and HP38E - MT
#  23 Dec 22         - Changed the order in which simulators are built - MT
#  06 Jun 23         - Added HP41C - MT
#  24 Oct 23         - Selects files using a wildcard, avoids any errors if
#                      no files exist when using tar - MT
#                    - Backs up backup files! (*.h.[0-9], *.h.[0-9]) - MT
#                    - Clean removes execuitable files - MT
#                    - Checks that the bin directory exists before deleting
#                      files - MT
#  01 Nov 23         - Tidied up tabs in comments - MT
#

PROGRAM	=  x11-calc

FILES		=  $(wildcard ./src/*.c) $(wildcard ./src/*.c.[0-9]) $(wildcard ./src/*.h) $(wildcard ./src/*.h.[0-9]) ./src/makefile
FILES		+= $(wildcard ./bin/x11-calc-*) $(wildcard *.md) LICENSE makefile
FILES		+= $(wildcard ./rom/*) $(wildcard ./prg/*.dat) $(wildcard ./img/x11-calc-*.png)
FILES		+= .gitignore .gitattributes

MAKE		=  make

all: clean hp35 hp45 hp70 hp80 hp10 hp21 hp22 hp25 hp25c hp27 hp29c hp67 hp31e hp32e hp33e hp33c hp34c hp37e hp38e hp38c hp41c hp10c hp11c hp12c hp15c hp16c

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

hp41c:
	@$(MAKE) --no-print-directory -C ./src MODEL=41c

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
	@test -d ./bin && find ./bin -type f  ! -name "*.*" -delete || true

backup:
	@echo "$(PROGRAM)-`date +'%Y%m%d%H%M'`.tar.gz"; tar -czpf ..\/$(PROGRAM)-`date +'%Y%m%d%H%M'`.tar.gz $(FILES)
