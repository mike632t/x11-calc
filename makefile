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
#  Note separator (tab) at the beginning of the line CANNOT be a space..!
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
#  24 Oct 23         - Selects files using a wildcard, avoids any errors if
#                      no files exist when using tar - MT
#                    - Backs up backup files! (*.h.[0-9], *.h.[0-9]) - MT
#                    - Clean removes execuitable files - MT
#                    - Checks that the bin directory exists before deleting
#                      files - MT
#  01 Nov 23         - Tidied up tabs in comments - MT
#  16 Feb 24         - Removed HP10 - MT
#  25 Feb 24         - Backup all makefiles - MT
#  26 Feb 24         - Defined common options in make comand - MT
#                    - Renamed makefiles to make it harder to use the wrong
#                      makefile on different operating systems - MT
#  02 Mar 24         - Added scripts to FILES - MT
#  04 Mar 24         - Added launcher build - macmpi
#  09 Mar 24         - Individual  calc recipes are  dynamically  generated
#                      and desktop file has built models shortcuts - macmpi
#  10 Mar 24         - Moved new desktop file to TARGET directory - MT
#                    - Create TARGET directory if it doesn't exist - MT
#                    - Fixed error in directory path when TARGET is defined
#                      on the command line - MT
#                    - Simplified directory creation - MT
#  13 Mar 24         - Create  install  recipe to install in given drectory
#                      add DESTDIR, and rename TARGET as BIN - macmpi
#  16 Mar 24         - Updated list of files (used to backup project) - MT
#                      Improve GNU conformance with prefix - macmpi
#  17 Mar 24         - Fail early if source and destination directories are
#                      the same - MT
#  18 Mar 24         - Test that $(prefix) exists before installing - MT
#  20 Mar 24         - Rename launcher into x11-calc - macmpi
#                    - Updated files - MT
#  22 Mar 24         - Simplify model targets auto-generation (portability)
#                      and use same naming as in oyher makefiles - macmpi
#                    - Clean out unwanted files before taking a backup, and
#                      remove symbolic links too - MT
#  23 Mar 24         - Do not delete execuitables - MT
#                    - Do not automatically delete object files - MT
#                    - Removed make.sh from list of files - MT
#                    - Do not clean up files before backing up - MT
#                    - Make clean deletes execuitables (leaving other files
#                      in the same folder untouched) - MT
#                    - Use .DEFAULT target + improve x11-calc deps - macmpi
#                    - Feed x11-calc _models with makefile  MODELS - macmpi
#  24 Mar 24         - Updated FILES - MT
#  28 Mar 24         - Leverage src/makefile.generic - macmpi
#  30 Mar 24         - Now common makefile for Linux/NetBSD/Darwin - macmpi
#  01 Apr 24         - Prepare osf1 inclusion into common makefile - macmpi
#                    - Tested on Tru64 UNIX and fixed up minor issues - MT
#                    - Modified backup to work on Tru64 Linux - MT
#                    - Consolidate Linux/NetBSD/Darwin and OSF1 - macmpi
#  02 Apr 24         - Add differentiated desktop files installs - macmpi
#  03 Apr 24         - Allow more custom desktop files installs - macmpi
#  03 Apr 24         - Fixed possible issue with use of date - MT
#                    - Attempts  to detect the default desktop  environment
#                      based on the existing directory structure - MT
#  04 Apr 24         - Aggregate everything in unique top makefile - macmpi
#                    - Fixed some issues on Tru64, running make install did
#                      not use the default desktop settings, but attempring
#                      to detect the default desktop in the catch all  case
#                      seems to work - MT
#                    - Made DESKTOP case insensitive - MT
#                    - Tidied up internal vairable names comments and error
#                      messages - MT
#  05 Apr 24         - Various tweaks - macmpi
#  06 Apr 24         - Shortened  bash command line length and target names
#                      for Tru64 UNIX compatibility - MT
#                    - Only attempts to set the 'prefix' based on the local
#                      environment if neither 'prefix' or 'DESTDIR' are set
#                      by the user - MT
#  09 Apr 24         - Optimized install script - MT
#  24 Apr 24         - Fix  parallel  make with .NOTPARALLEL accross models
#                      in  top  makefile  to prevent concurrent compiles of
#                      same  common  files.  Make  is  still  parallelizing
#                      submakes calls which do the real work. - macmpi
#  29 Apr 24         - Improve  parallel  make  performance  -  macmpi
#  12 May 24         - Use a separate makefile for MacOS - MT
#

PROGRAM		=  x11-calc

BIN		= bin
SRC		= src
PRG		= prg
ROM		= rom
IMG		= img

# Files to be backed up (and the current date).

_files		= `ls makefile makefile.*.[0-9] $(SRC)/makefile $(SRC)/makefile.all $(SRC)/makefile.linux $(SRC)/makefile.darwin $(SRC)/makefile.bsd $(SRC)/makefile.osf1 $(SRC)/makefile.*.[0-9] 2>/dev/null || true`
_source		= `ls $(SRC)/*.c $(SRC)/*.c.[0-9] $(SRC)/*.h $(SRC)/*.h.[0-9] $(SRC)/*.in $(SRC)/*.in.[0-9] 2>/dev/null || true`
_data		= `ls $(ROM)/$(PROGRAM)*.rom $(ROM)/$(PROGRAM)*.rom.[0-9] $(PRG)/$(PROGRAM)*.dat $(PRG)/$(PROGRAM)*.dat.[0-9] 2>/dev/null || true`
_images		= `ls $(SRC)/*.ico $(SRC)/*.ico.[0-9] $(SRC)/*.png $(SRC)/*.png.[0-9] $(SRC)/*.svg $(SRC)/*.svg.[0-9] $(IMG)/*.png $(IMG)/*.png.[0-9] 2>/dev/null || true`
_other		= `ls $(SRC)/make.com  $(SRC)/make.com.[0-9] *.md *.md.[0-9] $(SRC)/*.md $(SRC)/*.md.[0-9] $(IMG)/*.md $(IMG)/*.md.[0-9] .gitignore .gitattributes 2>/dev/null || true`

_date		= `date +'%Y%m%d%H%M'`

# Calculator models available in the emulator.

_classic	= hp35 hp45 hp70 hp80
_woodstock	= hp21 hp22 hp25 hp25c hp27 hp29c
_topcat		= hp67
_spice		= hp31e hp32e hp33e hp33c hp34c hp37e hp38e hp38c
_voyager	= hp10c hp11c hp12c hp15c hp16c

MODELS		= $(_classic) $(_woodstock) $(_topcat) $(_spice) $(_voyager)

# The prefix will only be modified by the installer it it is NOT set on the
# command  line.  If no prefix is defined on the command line then the most
# approprite  value will be chosen based on the current environment or  the
# selected DESKTOP.

DESKTOP		=
prefix		=
DESTDIR		=

MENU		= hp35 hp21 hp25c hp29c hp31e hp32e hp33c hp34c hp10c hp11c hp12c hp15c hp16c

.PHONY: backup clean install

all: $(MODELS) $(PROGRAM)

classic: $(_classic) $(PROGRAM)

woodstock: $(_woodstock) $(PROGRAM)

topcat: $(_topcat) $(PROGRAM)

spice: $(_spice) $(PROGRAM)

voyager: $(_voyager) $(PROGRAM)

# Base per-model compile target:
$(MODELS): common
	@_model="`echo "$@" | sed 's/hp//'`"; \
	cd $(SRC); \
	$(MAKE) -s MODEL=$$_model all

common:
	@cd $(SRC); $(MAKE) -s common

$(PROGRAM): $(BIN)/$(PROGRAM)

$(BIN)/$(PROGRAM): $(SRC)/$(PROGRAM).in
	@mkdir -p $(BIN)
	@_mdls=`echo "$(MODELS)" | tr ' ' '|'` ; \
		sed "s/^_models=\".*/_models=\"$$_mdls\"/" $(SRC)/$(PROGRAM).in > $@
	@chmod +x $@
	@ls $@ | sed "s:$(BIN)/::g"

clean:
	@rm -f $(SRC)/*.o $(SRC)/*.o_*
	@[ -d "$(BIN)" ] && rm -rf $(BIN) || true

install:
# Attempt to 'install' the application and data files to the correct target
# directories based on 'DESTDIR' and 'prefix'. Alternativly you can use the
# 'DESKTOP' vairable to specify the name of the target desktop to select an
# approprite 'prefix' automatically.  However as the script will attempt to
# detect what 'prefix' to use based on the local directory structure unless
# you are targetng another system or installing the application system wide
# it isn't necessary to specify either - 'make install' should just work...
#
# Note that Tru64 requires both DESTDIR and prefix to be explicitly defined
# when invoking make.
#
	@_unset() { if [ -z "$(DESTDIR)$(prefix)" ]; then return 0; else return 1; fi;}; \
	_desktop="`echo "$(DESKTOP)" | tr '[:lower:]' '[:upper:]' `"; \
	if [ -z "$$_desktop" ]; then \
		if [ -d "$$HOME/.local" ]; then \
			_desktop="FREEDESKTOP"; \
		elif [ -d "$$HOME/.dt" ]; then \
			_desktop="CDE"; \
		else \
			_desktop="NONE"; \
		fi; \
	fi; \
	case $$_desktop in \
		FREEDESKTOP | GNOME | MATE | KDE | LXDE | LXQT | XFCE) \
			_unset && prefix="$$HOME/.local" || true; \
			$(MAKE) -s DESKTOP=$$_desktop DESTDIR=$(DESTDIR) prefix=$$prefix do_env; \
			;; \
		CDE) \
			_unset && prefix="$$HOME/.dt" || true; \
			$(MAKE) -s DESTDIR=$(DESTDIR) prefix=$$prefix do_cde; \
			;; \
		NONE) \
			_unset && prefix="$$HOME" || true; \
			$(MAKE) -s DESTDIR=$(DESTDIR) prefix=$$prefix do_none; \
			;; \
		*) \
			echo "'DESKTOP=$(DESKTOP)' was not recognised: Try 'make install' instead."; \
			;; \
	esac;

do_copy: $(BIN)
# No  matter what desktop environment we are using (even if it is NONE)  we
# need to copy the executable files to the destination folder.
#
	@[ -n "$(DESTDIR)" ] || [ -d "$(prefix)" ] || \
		{ echo "Please ensure $(prefix) exists (or set DESTDIR for staged install)." >&2; exit 1; }
	@mkdir -p "$(DESTDIR)$(prefix)"
	@cp -R $(BIN) "$(DESTDIR)$(prefix)"/ # Fail early if source and destination directories are the same

do_none: do_copy
# Copy binaries (and saved programs ?) to $(BIN) if no desktop specified.
#
# Copying saved programs to the same folder and the application really only
# makes sense if the application looks in it's current folder for a file it
# can't load otherwise - which is not yet implemented.
#
# This target rule is a placeholder for now.
#
#
	@[ -n "$${VERBOSE+x}" ] && echo "No desktop option selected" || true
	@[ -n "$${VERBOSE+x}" ] && echo "Installing in $(DESTDIR)$(prefix)" || true
#
#	@cp -R $(PRG)/* "$(DESTDIR)$(prefix)/$(BIN)/"

do_cde: do_copy
# Desktop integration files install for CDE - just a place holder for now.
#
# ToDo - setup desktop and icons (in xpm format 16×16 (x11-calc.t.pm) 32×32
# (x11-calc.m.pm) 48×48 (x11-calc.l.pm)) in either /etc/dt/appconfig/icons/,
# /usr/dt/appconfig/icons/ or $HOME/.dt/icons
#
	@[ -n "$${VERBOSE+x}" ] && echo "Selected CDE desktop environment" || true
	@[ -n "$${VERBOSE+x}" ] && echo "Installing in $(DESTDIR)$(prefix)" || true
#
# Add program files
	@cp -R $(PRG)/* "$(DESTDIR)/$(prefix)/$(BIN)/"

do_env: do_copy $(SRC)/$(PROGRAM).desktop.in $(SRC)/$(PROGRAM).svg
# Desktop integration files install for all freedesktop compatible desktops
# with workaround for bsd `sed -i` syntax
#
	@[ -n "$${VERBOSE+x}" ] && echo "Selected $(DESKTOP) environment" || true
	@[ -n "$${VERBOSE+x}" ] && echo "Installing in $(DESTDIR)$(prefix)" || true
#
# Copy desktop files
	@mkdir -p "$(DESTDIR)$(prefix)"/share/applications;
	@_dest_file="$(DESTDIR)$(prefix)/share/applications/$(PROGRAM).desktop"; \
	_tmp_file="`mktemp`"; \
	cp $(SRC)/$(PROGRAM).desktop.in "$$_dest_file"; \
	for _item in $(MENU); do \
		printf "\n[Desktop Action $$_item]\nName=• $$_item\nExec=$(PROGRAM) $$_item\n" >> "$$_dest_file"; \
		sed "s/^Actions=.*/&;$$_item/" "$$_dest_file" > "$$_tmp_file" ; mv "$$_tmp_file" "$$_dest_file"; \
	done; \
	chmod 644 "$$_dest_file"; \
	rm -f "$$_tmp_file"
# Copy program files
	@mkdir -p "$(DESTDIR)$(prefix)"/share/$(PROGRAM)/
	@cp -R $(PRG) "$(DESTDIR)$(prefix)"/share/$(PROGRAM)/
	@chmod -R 644 "$(DESTDIR)$(prefix)"/share/$(PROGRAM)/$(PRG)/*
# Copy icon files
	@mkdir -p "$(DESTDIR)$(prefix)"/share/icons/hicolor/scalable/apps
	@cp $(SRC)/$(PROGRAM).svg "$(DESTDIR)$(prefix)"/share/icons/hicolor/scalable/apps/$(PROGRAM).svg
	@chmod 644 "$(DESTDIR)$(prefix)"/share/icons/hicolor/scalable/apps/$(PROGRAM).svg

backup:
# Backup known files to a tar archive (with nultiple workarounds to accomodate
# tar limitations and maximum line length on Tru64 UNIX)
	@tar -cf ..\/$(PROGRAM)-$(_date).tar $(_files)
	@tar -rf ..\/$(PROGRAM)-$(_date).tar $(_source)
	@tar -rf ..\/$(PROGRAM)-$(_date).tar $(_data)
	@tar -rf ..\/$(PROGRAM)-$(_date).tar $(_images)
	@tar -rf ..\/$(PROGRAM)-$(_date).tar $(_other)
	@echo "$(PROGRAM)-$(_date).tar"
