#!/bin/sh
#
#  sh-get-environment.sh
#
#  Copyright(C) 2025 - MT
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
#  18 Oct 25  0.1.0001  - Initial version - MT
#  09 Nov 25            - Attempts to detect the session manager - MT
#  12 Nov 25            - Added the default compiler - MT
#  20 Nov 25            - Scan for multiple compilers - MT
#  21 Nov 25            - Improved portability (tested on Solaris 10) - MT
#  22 Nov 25            - Added kernel version and architecture - MT
#                       - Fixed issues in join  - MT
#                       - Ignore 'not installed' error messages - MT
#  23 Nov 25            - Display architecture in brackets - MT
#  24 Nov 25            - Modified for Tru64 UNIX - MT
#  25 Nov 25            - Fixed issues on Solaris and Minix - MT
#  27 Nov 25            - Match whole words when searching process names to
#                         avoid spurious results - MT
#  30 Nov 25            - Specifically checks for the software version when
#                         running on Darwin - MT
#  01 Dec 25            - Checks the software version command exists - MT
#  02 Dec 25            - Fixed MacOS version detection - MT
#

#
#  join()
#
#  Concatenates  the elements of an array into comma delimited list removing
#  any duplicates.
#
#  It is deliberately verbose to make it more portable.
#
#  21 Nov 25  0.1.0001  - Initial version - MT
#                       - Now handles multi-character delimiters - MT
#
#_join() {
#   local _delimiter=", "  # Includes space
#   local _list=""
#   local _text
#   for _text in "$@"; do
#      case "$_delimiter$_list$_delimiter" in
#         *"$_delimiter$_text$_delimiter"*) ;;  # Skip duplicates entries, comment out this line to allow duplicates
#         *)
#            [ -n "$_list" ] && _list="$_list$_delimiter"
#            _list="$_list$_text"
#            ;;
#      esac
#   done
#   echo "$_list"
#}

#
#  resolve()
#
#  Searches for a file resolving symbolic links using readlink if available
#  otherwise it will fall back to parsing the output of 'ls -l' to maximize
#  portability. Can handle absolute and relative links safely returning the
#  fully resolved path.
#
#  24 Nov 25  0.1.0001  - Initial version - MT
#

resolve() {
   _path="$1"
   while [ -L "$_path" ]; do
      if command -v readlink >/dev/null 2>&1; then
         _target=`readlink "$_path" 2>/dev/null`
      else
         # Tru64 fallback: parse ls -l output
         _target=`ls -l "$_path" 2>/dev/null | awk '{for(i=1;i<=NF;i++){if($i=="->"){print $(i+1);exit}}}'`
      fi

      # Handle relative symlink targets
      case "$_target" in
         /*) _path="$_target" ;;
         *)  _dir=`dirname "$_path"`
            _path="$_dir/$_target"
            ;;
      esac
   done
   echo "$_path"
}

#
#  lower()
#
#  Converts a string to lowercase.
#
#  25 Nov 25  0.1.0001  - Initial version - MT
#

lower() {
   echo "$*" | sed -e 'y/ABCDEFGHIJKLMNOPQRSTUVWXYZ/abcdefghijklmnopqrstuvwxyz/'
}

#  Operating System and kernel version
_os=`uname -s 2>/dev/null`      # System name
_kernel=`uname -r 2>/dev/null`  # Release
_arch=`uname -m 2>/dev/null`    # Hardware
_system=""

if [ -f /etc/lsb-release ]; then
   . /etc/lsb-release
   _system="$DISTRIB_DESCRIPTION"
elif [ -f /etc/os-release ]; then
   . /etc/os-release
   _system="$PRETTY_NAME"
elif [ -f /etc/redhat-release ]; then
   _system=`cat /etc/redhat-release`
elif [ "`lower $_os`" = "darwin" ] && command -v sw_vers 2>&1 >/dev/null; then  # Only exists on Mac OS
      _system="`sw_vers -productName 2>/dev/null` `sw_vers -productVersion 2>/dev/null`"
else
   _system="$_os $_kernel"
fi
echo "Operating System: $_system"
echo ""

if [ -n "$_kernel" ] && [ -n "$_arch" ]; then
   echo "Kernel: `lower $_kernel` ($_arch)``"
   echo ""
fi

#  Hypervisor
_hypervisor=""
if grep -i microsoft /proc/version 2>/dev/null >/dev/null; then  # Check for WSL/WSL2
   if uname -r | grep -i "WSL2" >/dev/null 2>&1; then
      _hypervisor="wsl2"
   else
      _hypervisor="wsl"
   fi
fi
if [ -z "$_hypervisor" ]; then
   if type systemd-detect-virt >/dev/null 2>&1; then  # Try systemd-detect-virt
      v=`systemd-detect-virt 2>/dev/null`
      if [ -n "$v" ] && [ "$v" != "none" ]; then
         _hypervisor="$v"
      fi
   fi
fi
if [ -z "$_hypervisor" ]; then
   if type virt-what >/dev/null 2>&1; then  # Try virt-what
      _hypervisor=`virt-what 2>/dev/null | tr '\n' ' ' | sed 's/ $//'`
   fi
fi
if [ -z "$_hypervisor" ]; then
   for _file in /sys/class/dmi/id/sys_vendor \
             /sys/class/dmi/id/product_name \
             /sys/class/dmi/id/board_vendor \
             /sys/class/dmi/id/bios_vendor
   do
      if [ -r "$_file" ]; then  # Inspect DMI/SMBIOS strings (Linux-only)
         _value=`cat "$_file" 2>/dev/null | tr -d '\000'`
         case "$_value" in
         *Microsoft*|*Hyper-V*|*Virtual\ Machine*)
            _hypervisor="hyperv"
            break ;;
         *VMware*|*VMware,\ Inc.*|*VMware\ Virtual\ Platform*)
            _hypervisor="vmware"
            break ;;
         *QEMU*|*KVM*|*Bochs*|*Red\ Hat*|*RHEV*|*oVirt*)
            _hypervisor="kvm"
            break ;;
         *VirtualBox*|*innotek*|*Oracle*)
            _hypervisor="virtualbox"
            break ;;
         *Xen*|*XenServer*)
            _hypervisor="xen"
            break ;;
         *Parallels*|*Parallels\ Software*)
            _hypervisor="parallels"
            break ;;
         esac
      fi
   done
fi
if [ -z "$_hypervisor" ]; then
   if grep hypervisor /proc/cpuinfo >/dev/null 2>&1; then  # CPU flag check (Linux-only)
      _hypervisor="unknown"
   fi
fi
if [ -n "$_hypervisor" ]; then
   echo "Hypervisor: `lower $_hypervisor`"
   echo ""
fi

#  Display Manager
_display_manager=""
if [ -f /etc/X11/default-display-manager ]; then
   _display_manager=`cat /etc/X11/default-display-manager 2>/dev/null`  # Check default-display-manager file (Debian/Ubuntu)
   if [ -n "$_display_manager" ]; then  # Strip path if found
      _display_manager=`basename "$_display_manager"`
   fi
fi

if [ -z "$_display_manager" ]; then
   for _process in cdm dtlogin gdm gdm3 kdm lightdm sddm slim xdm  # Check running processes
   do
      if ps -e 2>/dev/null | grep -w "$_process " | grep -v grep >/dev/null 2>&1; then  # Exclude the grep itself
         _display_manager="$_process"
         break
      fi
   done

   if [ -z "$_display_manager" ]; then
      if [ -n "$WAYLAND_DISPLAY" ]; then
         _display_manager="WSLg"
      else
         _display_manager="unknown"
      fi
   fi
fi
echo "Display Manager: `lower $_display_manager`"
echo ""

#  Session Manager
_session_manager=""
for _process in budgie-session cinnamon-session dxsession dtsession \
   gnome-session gnome-session-binary ksmserver lxsession mate-session \
   xfce4-session x-session-manager Xsession xsession
do
   if ps -e 2>/dev/null | grep -w "$_process" | grep -v grep >/dev/null 2>&1; then
      _session_manager="$_process"
      break
   fi
done
if [ -z "$_session_manager" ]; then  # Check running processes
   _x_session_manager=`command -v x-session-manager 2>/dev/null`  # Check if x-session-manager exists
   if [ -n "$_x_session_manager" ]; then
      _resolved=`resolve "$_x_session_manager"`
      _session_manager=`basename "$_resolved"`
   fi
   if [ -z "$_session_manager" ]; then
      _session_manager="unknown"
   fi
fi
echo "Session Manager: `lower $_session_manager`"
echo ""

# Window Manager
_window_manager=""
for _process in awesome blackbox fluxbox fvwm gnome-shell icewm i3 kwin marco \
   metacity mutter mwm openbox sawfish twm wmaker xfwm; do
      if ps -e 2>/dev/null | grep -w "$_process" | grep -v grep >/dev/null 2>&1; then
         _window_manager="$_process"
         break
      fi
      if ps -A 2>/dev/null | grep -w "$_process" | grep -v grep >/dev/null 2>&1; then
         _window_manager="$_process"
         break
      fi
done
if [ -z "$_window_manager" ]; then
   _window_manager="unknown"
fi
echo "Window Manager: `lower $_window_manager`"
echo ""

# Desktop Environment
_environment="$XDG_CURRENT_DESKTOP"
if [ -z "$_environment" ]; then
   if [ "$_session_manager" = "gnome-session" ]; then
      _environment="gnome"
   elif [ "$_display_manager" = "xdm" ] && [ "$_window_manager" = "mwm" ]; then
      _environment="motif"
   elif [ "$_display_manager" = "dtlogin" ] && [ "$_window_manager" = "twm" ]; then
      _environment="cde"
   else
      _environment="unknown"
   fi
fi
echo "Desktop Environment: `lower $_environment`"
echo ""


# Compiler
for dir in /usr/sfw/bin /usr/ccs/bin /opt/SUNWspro/bin; do  # Add additional directories to PATH
    [ -d "$dir" ] && PATH="$dir:$PATH"  && export PATH
done
_matches=""
for _option in cc gcc clang suncc tcc pcc; do
   if command -v "$_option" 2>&1 >/dev/null; then
      _command=`command -v "$_option"`
      case "$_option" in
      cc)
         _version=`"$_command" -V 2>/dev/null | sed -n '1p'`
         if [ -z "$_version" ]; then
            _version=`"$_command" -v 2>&1 | grep -w version | tail -1`
         fi
         ;;
      gcc|clang|pcc)
         _version=`"$_command" --version 2>/dev/null | sed -n '1p'`
         ;;
      tcc)
         _version=`"$_command" -v 2>/dev/null | sed -n '1p'`
         ;;
      suncc)
         _version=`"$_command" -V 2>/dev/null | sed -n '1p'`
         ;;
      esac
      _version=`echo $_version | grep -v "not installed" | sed -e 's/([^()]*)//g; s/[ \t]*$//; s/ version / /g; s/^\([^0-9]*[0-9][^ ]*\) .*/\1/; s/  */ /g' | sed -n '1p'`
      if [ -n "$_version" ]; then
         case ",$_matches," in
            *,"$_version",*)  # Already found don't add a duplicate
               ;;
            *)
               if [ -n "$_matches" ]; then
                  _matches="$_matches, $_version"
               else
                  _matches="$_version"
               fi
               ;;
         esac
      else  # Binary exists but version is blank
         if [ -n "$_matches" ]; then
            _matches="$_matches, $_option"
         else
            _matches="$_option"
         fi
      fi
   fi
done
if [ -n "$_matches" ]; then
   echo "Compiler: `lower $_matches`"
else
   echo "Compiler: none"
fi
echo ""

_matches=""
for _option in bmake gmake make nmake; do
   if command -v "$_option" 2>&1 >/dev/null; then
      _command=`command -v "$_option"`
      case "$_option" in
      bmake|gmake|make|nmake)
         _version=`"$_command" -v 2>/dev/null | sed -n '1p'`
         ;;
      esac
      _version=`echo $_version | grep -v "not installed" | sed -e 's/([^()]*)//g; s/[ \t]*$//; s/ version / /g; s/ on .*//; s/  */ /g; y/ABCDEFGHIJKLMNOPQRSTUVWXYZ/abcdefghijklmnopqrstuvwxyz/' | sed -n '1p'`
      if [ -n "$_version" ]; then
         case ",$_matches," in
            *,"$_version",*)  # Already found don't add a duplicate
               ;;
            *)
               if [ -n "$_matches" ]; then
                  _matches="$_matches, $_version"
               else
                  _matches="$_version"
               fi
               ;;
         esac
      else  # Binary exists but version is blank
         if [ -n "$_matches" ]; then
            _matches="$_matches, $_option"
         else
            _matches="$_option"
         fi
      fi
   fi
done
if [ -n "$_matches" ]; then
   echo "Make: `lower $_matches`"
else
   echo "Make: none"
fi
#echo ""

