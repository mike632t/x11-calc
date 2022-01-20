$  _verify := 'f$verify(0,0)
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
$! 20 Jan 22         - Can now build one or all the simulators - MT
$!
$  _message_status = f$environment("MESSAGE")
$  on error then goto _done
$  on control_Y then goto _done
$  _list = "35,80,45,70,21,22,25,27,29,31,32,33,34,37,38,67"
$  _count = 0
$  if "''P1'" .eqs "" then goto _next
$  if "''P1'" .eqs "ALL" then goto _next
$  if f$extract(0, 2, "''P1'") .nes. "HP" then goto _invalid
$  if f$locate(f$extract(2, 2, "''P1'"), _list) .eqs. f$length(_list) then goto _invalid
$  _list = f$extract(2, 2, "''P1'")
$_next:
$  _model = f$element(_count, ",", _list)
$  if _model .eqs. "," then goto _done
$  write sys$output "x11-calc-''_model'"
$  if f$search("x11-calc''_model'.exe") .nes. "" then delete "x11-calc''_model'.exe;*" /nolog /noconfirm
$  if f$search("*.obj") .nes. "" then delete *.obj;* /nolog /noconfim
$  cc /define=HP'_model x11-calc-'_model, x11-calc, x11-calc-cpu, x11-calc-segment, x11-calc-display, x11-calc-button, x11-calc-colour, x11-calc-switch, gcc-wait
$  link x11-calc-'_model, x11-calc, x11-calc-cpu, x11-calc-segment, x11-calc-display, x11-calc-button, x11-calc-colour, x11-calc-switch, gcc-wait, x11-lib.opt/opt
$  if f$search("*.obj") .nes. "" then delete *.obj;* /nolog /noconfim
$  _count = _count + 1
$  goto _next
$_invalid:
$  write sys$output "Invalid model number!"
$_done:
$  set message 'Message_Status'
$  if _verify .eq. 0 then set verify
