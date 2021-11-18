$  Verify := 'f$verify(0,0)
$!
$! MAKE.COM
$!
$! Copyright(C) 2013   MT
$!
$! DCL command file to compiles and link X11-CALC
$!
$! This  program is free software: you can redistribute it and/or modify it
$! under  the terms of the GNU General Public License as published  by  the
$! Free  Software Foundation, either version 3 of the License, or (at  your
$! option) any later version.
$!
$! This  program  is distributed in the hope that it will  be  useful,  but
$! WITHOUT   ANY   WARRANTY;   without even   the   implied   warranty   of
$! MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General
$! Public License for more details.
$!
$! You  should have received a copy of the GNU General Public License along
$! with this program.  If not, see <http://www.gnu.org/licenses/>.
$!
$! 23 Jul 13         - Initial version - MT
$! 17 Nov 21         - Define filename as a variable - MT
$!
$  on error then goto Done
$  on control_Y then goto Done
$  _filename := "x11-calc-33"
$  if f$search("''_filename'.exe") .nes. "" then delete '_filename.exe;* /nolog /noconfirm
$  cc /lis x11-calc, '_filename, x11-calc-cpu, x11-calc-segment, x11-calc-display, x11-calc-button, x11-calc-colour, x11-calc-switch, gcc-wait
$  link '_filename, x11-calc, x11-calc-cpu, x11-calc-segment, x11-calc-display, x11-calc-button, x11-calc-colour, x11-calc-switch, gcc-wait, x11-lib.opt/opt
$  del *.obj;* /nolog /noconfim
$Done:
$  exit
