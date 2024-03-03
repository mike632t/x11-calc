#! /bin/sh
#
#  make
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
#  28 Feb 24         - Initial version - MT
#  29 Feb 24         - If it exists copy ./src/x11-calc.sh to ./bin - MT
#
make -s -f makefile.$(uname | tr '[:upper:]' '[:lower:]') "$@"
[ -f ./src/x11-calc.sh ] && cp -v ./src/x11-calc.sh ./bin/
