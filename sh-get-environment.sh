#!/bin/bash
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
#

#
#  join()
#
#  Concatenates  the elements of an array into comma delimited list removing
#  any duplicates.
#
#  It is deliberately verbose to make it more portable.
#
_join() {
   local _delimiter=", "
   local _list=""
   local _text
   for _text in "$@"; do
      case ",$_list," in
         *,"$_text",*) ;;  # skip duplicates
         *)
            [ -n "$_list" ] && _list="$_list$_delimiter"
            _list="$_list$_text"
            ;;
      esac
   done
   echo "$_list"
}

#  Operating System and kernel version
_os=$(uname -s 2>/dev/null)      # System name
_kernel=$(uname -r 2>/dev/null)  # Release
_arch=$(uname -m 2>/dev/null)    # Hardware
_system=""
if [ -f /etc/lsb-release ]; then
   . /etc/lsb-release
   _system="$DISTRIB_DESCRIPTION"
elif [ -f /etc/os-release ]; then
   . /etc/os-release
   _system="$PRETTY_NAME"
elif [ -f /etc/redhat-release ]; then
   _system=$(cat /etc/redhat-release)
else
   _system="$_os $_kernel ($_arch)"
fi
echo "Operating System: $_system"
echo ""
if [ -n "$_kernel" ] && [ -n "$_arch" ]; then
echo "Kernel: $_kernel $_arch"
echo ""
fi

#  Hypervisor
_hypervisor=""
if grep -qi microsoft /proc/version 2>/dev/null; then  # Start by checking for WSL or WSL2
   if uname -r | grep -qi "WSL2"; then
      _hypervisor="wsl2"
   else
      _hypervisor="wsl"
   fi
fi
if [ -z "$_hypervisor" ] && command -v systemd-detect-virt >/dev/null 2>&1; then  # If not found use systemd-detect-virt (if installed)
   v=$(systemd-detect-virt 2>/dev/null || true)
   [ -n "$v" ] && [ "$v" != "none" ] && _hypervisor="$v"
fi
if [ -z "$_hypervisor" ] && command -v virt-what >/dev/null 2>&1; then  # If not found  fallback to virt-what (if installed)
   _hypervisor=$(virt-what 2>/dev/null | tr '\n' ' ' | sed 's/ $//')
fi
if [ -z "$_hypervisor" ]; then # If not found try inspecting DMI/SMBIOS strings
for _file in /sys/class/dmi/id/sys_vendor /sys/class/dmi/id/product_name /sys/class/dmi/id/board_vendor /sys/class/dmi/id/bios_vendor; do
   [ -r "$_file" ] || continue
      _value=$(tr -d '\0' < "$_file" 2>/dev/null || cat "$_file" 2>/dev/null)
      case $_value in
         *Microsoft*|*Hyper-V*|*Virtual\ Machine*) _hypervisor="hyperv"; break ;;
         *VMware*|*VMware,\ Inc.*|*VMware\ Virtual\ Platform*) _hypervisor="vmware"; break ;;
         *QEMU*|*KVM*|*Bochs*|*Red\ Hat*|*RHEV*|*oVirt*) _hypervisor="kvm"; break ;;
         *VirtualBox*|*innotek*|*Oracle*) _hypervisor="virtualbox"; break ;;
         *Xen*|*XenServer*) _hypervisor="xen"; break ;;
         *Parallels*|*Parallels\ Software*) _hypervisor="parallels"; break ;;
      esac
   done
fi
if [ -z "$_hypervisor" ] && grep -q 'hypervisor' /proc/cpuinfo 2>/dev/null; then # If not found try checking the hypervisor CPU flag (can't tell which one it is)
   _hypervisor="unknown"
fi
if [ -n "$_hypervisor" ]; then  # Optional don't display if not defined
   echo "Hypervisor: $_hypervisor"
   echo ""
fi

#  Display Manager
_display_manager=""
_display_manager=$([ -f /etc/X11/default-display-manager ] && cat /etc/X11/default-display-manager 2>/dev/null)
if [ -n "$_display_manager" ]; then
   _display_manager=$(basename "$_display_manager")
fi
if [ -z "$_display_manager" ]; then
   # _display_manager=$(ps -e -o comm= | grep -E 'cdm|gdm|gdm3|kdm|lightdm|sddm|slim|xdm' | sort | uniq)  # Can't use 'grep -E'
   for _process in cdm gdm gdm3 kdm lightdm sddm slim xdm; do
     if pgrep -x "$_process" >/dev/null 2>&1; then
       _display_manager="$_process"
       break
     fi
   done
   if [ -z "$_display_manager" ]; then
      if [ -n "$WAYLAND_DISPLAY" ]; then
         _display_manager="WSLg"
      else
         _display_manager="Unknown"
      fi
   fi
fi
echo "Display Manager: $_display_manager"
echo ""

#  Session Manager
_session_manager=""
_x_session_manager=$(command -v x-session-manager 2>/dev/null)
if [ -n "$_x_session_manager" ]; then  # x-session-manager command exists
   _x_session_manager=$(readlink -f "$_x_session_manager" 2>/dev/null)
   if [ -n "$_x_session_manager" ]; then  # and is valid symlink
      _session_manager=$(basename "$_x_session_manager")
   fi
fi
if [ -z "$_session_manager" ]; then
   # _session_manager=$(ps -eo comm | grep -E 'budgie-session|cinnamon-session|gnome-session|gnome-session-binary|ksmserver|lxsession|mate-session|xfce4-session' | grep -v grep | sort | uniq)  # Can't use grep -E
   for _process in budgie-session cinnamon-session gnome-session gnome-session-binary ksmserver lxsession mate-session xfce4-session x-session-manager; do
     if pgrep -x "$(echo "$_process" | cut -c1-15)" >/dev/null 2>&1; then  # Truncate otherwise process name won't match max 15 characters
       _session_manager="$_process"
       break
     fi
   done
   if [ -z "$_session_manager" ]; then
      _session_manager="Unknown"
   fi
fi
echo "Session Manager: $_session_manager"
echo ""

#  Window Manager
_window_manager=""
#_window_manager=$(ps -e -o comm= | grep -E 'awesome|blackbox|fluxbox|fvwm|gnome-shell|icewm|i3|kwin|marco|metacity|mutter|openbox|sawfish|twm|wmaker|xfwm' | sort | uniq)  # Can't use grep -E
for _process in awesome blackbox fluxbox fvwm gnome-shell icewm i3 kwin marco metacity mutter openbox sawfish twm wmaker xfwm; do
  if pgrep -x "$(echo "$_process" | cut -c1-15)" >/dev/null 2>&1; then  # Truncate otherwise process name won't match max 15 characters
    _window_manager="$_process"
    break
  fi
done
if [ -z "$_window_manager" ]; then
   _window_manager="Unknown"
fi
echo "Window Manager: $_window_manager"
echo ""

# Desktop Environment
_environment="$XDG_CURRENT_DESKTOP"
if [ -z "$_environment" ]; then
   _environment="Unknown"
fi
echo "Desktop Environment: $_environment"
echo ""

# Session Type (Wayland vs X11)
_session_type="$XDG_SESSION_TYPE"
if [ -z "$_session_type" ]; then
  if [ -n "$WAYLAND_DISPLAY" ]; then
    _session_type="wayland"
  elif [ -n "$DISPLAY" ]; then
    _session_type="x11"
  else
    _session_type="Unknown"
  fi
fi
echo "Session Type: $_session_type"
echo ""

# Compiler
for dir in /usr/sfw/bin /usr/ccs/bin /opt/SUNWspro/bin; do # Add additional directories to PATH
    [ -d "$dir" ] && PATH="$dir:$PATH"
done

_matches=()
for _option in cc gcc clang suncc tcc pcc; do
   if command -v "$_option" >/dev/null 2>&1; then
      _command=$(command -v "$_option")
      case "$_option" in
         gcc|clang|pcc)
            _version=$("$_command" --version 2>&1 | sed -n '1p')
            ;;
         tcc)
            _version=$("$_command" -v 2>&1 | sed -n '1p')
            ;;
         cc)
            _version=$("$_command" -v 2>&1 | sed -n '$p')  # Last line
            ;;
         cc|suncc)
            _version=$("$_command" -V 2>&1 | sed -n '1p')
            ;;
      esac
      _version=$(echo $_version | sed -e 's/([^()]*)//g; s/[ \t]*$//; s/version/ /g; s/  */ /g; y/ABCDEFGHIJKLMNOPQRSTUVWXYZ/abcdefghijklmnopqrstuvwxyz/')
      #  [ -n "$_version" ] && _matches[${#_matches[@]}]="${_command} → $_version" # Alternative output showing paths and versions
      [ -n "$_version" ] && _matches[${#_matches[@]}]="$_version"
   fi
done
_list=$(_join ", " "${_matches[@]}")  # Concatenate matches into a list

#  Alternative output showing paths and versions
#  echo "Compilers detected:"
#  for m in "${_matches[@]}"; do
#     echo "  $m"
#  done

if [ -n $__list ]; then
   echo "Compiler: $_list"
else
   echo "Not found"
fi
echo ""

#  Make
_matches=()
for _option in bmake gmake make nmake; do
   if command -v "$_option" >/dev/null 2>&1; then
      _command=$(command -v "$_option")
      case "$_option" in
         bmake|gmake|make|nmake)
            _version=$("$_command" -v 2>&1 | sed -n '1p')
            ;;
      esac
      _version=$(echo $_version | sed -e 's/([^()]*)//g; s/[ \t]*$//; s/version/ /g; s/  */ /g; y/ABCDEFGHIJKLMNOPQRSTUVWXYZ/abcdefghijklmnopqrstuvwxyz/')
      [ -n "$_version" ] && _matches[${#_matches[@]}]="$_version"
   fi
done
_list=$(_join "${_matches[@]}")  # Concatenate matches into a list

if [ -n $__list ]; then
   echo "Make: $_list"
else
   echo "Not found"
fi
#  echo ""

