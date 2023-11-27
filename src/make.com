$  _verify := 'f$verify(0,0)
$!
$! Copyright(C) 2013   MT
$!
$! DCL command file to compile and link X11-CALC
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
$! 20 Jan 22         - Make specified model or all models - MT
$! 21 Jan 22         - Moved text messages to a separate file  - MT
$! 22 Dec 22         - The model number defined on the command line must be
$!                     enclosed in quotes - MT
$! 01 Nov 23         - Executable files created in [-.BIN] - MT
$!                   - Create [-.BIN] folder if it doesn't exist - MT
$!
$  _message_status = f$environment("MESSAGE")
$  on error then goto _done
$  on control_Y then goto _done
$  _list = "10,35,45,70,80,21,22,25,25c,27,29c,31e,32e,33e,33c,34c,37e,38c,67,10c,11c,12c,15c,16c"
$  _count = 0
$  if "''P1'" .eqs "" then goto _check_folder
$  if "''P1'" .eqs "ALL" then goto _check_folder
$  if f$extract(0, 2, "''P1'") .nes. "HP" then goto _invalid
$  _model = f$edit(f$extract(2, f$length("''P1'") - 2,"''P1'"),"lowercase")
$  if f$locate(_model, _list) .eqs. f$length(_list) then goto _invalid
$  _list = _model
$_check_folder:
$  if f$search("[-]bin.dir") .eqs. "" then create/dir [-.bin]
$_next:
$  _model = f$element(_count, ",", _list)
$  if _model .eqs. "," then goto _done
$  write sys$output "x11-calc-''_model'"
$  if f$search("x11-calc''_model'.exe") .nes. "" then delete "x11-calc''_model'.exe;*" /nolog /noconfirm
$  if f$search("*.obj") .nes. "" then delete *.obj;* /nolog /noconfim
$  cc /define="HP''_model'" x11-calc-'_model, x11-calc, x11-calc-cpu, x11-calc-segment, x11-calc-display, x11-calc-button, x11-calc-colour, x11-calc-switch, x11-calc-label, x11-calc-messages, gcc-wait
$  link /exec=[-.bin]x11-calc-'_model x11-calc-'_model, x11-calc, x11-calc-cpu, x11-calc-segment, x11-calc-display, x11-calc-button, x11-calc-colour, x11-calc-switch, x11-calc-label, x11-calc-messages, gcc-wait, x11-lib.opt/opt
$  if f$search("*.obj") .nes. "" then delete *.obj;* /nolog /noconfim
$  _count = _count + 1
$  goto _next
$_invalid:
$  write sys$output "Invalid model number!"
$_done:
$  set message 'Message_Status'
$! if _verify .eq. 0 then set verify
