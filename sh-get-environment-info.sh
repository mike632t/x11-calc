#!/bin/bash

# Operating System
_operating_system=""
if [ -f /etc/os-release ]; then
   . /etc/os-release
   _operating_system="$PRETTY_NAME"
elif [ -f /etc/lsb-release ]; then
   . /etc/lsb-release
   _operating_system="$DISTRIB_DESCRIPTION"
elif [ -f /etc/redhat-release ]; then
   _operating_system=$(cat /etc/redhat-release)
else
   _operating_system="$(uname -s) $(uname -r 2>/dev/null)"
fi
echo "Operating System: $_operating_system"
echo ""

# Hypervisor
_hypervisor=""
if grep -qi microsoft /proc/version 2>/dev/null; then # Start by checking for WSL or WSL2
   if uname -r | grep -qi "WSL2"; then
      _hypervisor="wsl2"
   else
      _hypervisor="wsl"
   fi
fi
if [ -z "$_hypervisor" ] && command -v systemd-detect-virt >/dev/null 2>&1; then # If not found use tyr systemd-detect-virt (if installed)
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

# Display Manager
_display_manager=$([ -f /etc/X11/default-display-manager ] && cat /etc/X11/default-display-manager 2>/dev/null)
if [ -n "$_display_manager" ]; then
   _display_manager=$(basename "$_display_manager")
fi
if [ -z "$_display_manager" ]; then
   _display_manager=$(ps -e -o comm= | grep -E 'cdm|gdm|gdm3|kdm|lightdm|sddm|slim|xdm' | sort | uniq)
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

# Session Manager
_x_session_manager=$(command -v x-session-manager 2>/dev/null)
if [ -n "$_x_session_manager" ]; then  # x-session-manager command exists
   _x_session_manager=$(readlink -f "$_x_session_manager" 2>/dev/null)
   if [ -n "$_x_session_manager" ]; then  # and is valid symlink
      _session_manager=$(basename "$_x_session_manager")
   fi
fi

if [ -z "$_session_manager" ]; then
   _session_manager=$(ps -eo comm | grep -E 'budgie-session|cinnamon-session|gnome-session|gnome-session-binary|ksmserver|lxsession|mate-session|xfce4-session' | grep -v grep | sort | uniq)
   if [ -z "$_session_manager" ]; then
      _session_manager="Unknown"
   fi
fi
echo "Session Manager: $_session_manager"
echo ""

# Window Manager
_window_manager=$(ps -e -o comm= | grep -E 'awesome|blackbox|fluxbox|fvwm|gnome-shell|icewm|i3|kwin|marco|metacity|mutter|openbox|sawfish|twm|wmaker|xfwm' | sort | uniq)
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
_compiler=""
if command -v cc >/dev/null 2>&1; then
   _compiler=$(readlink -f "$(command -v cc)" 2>/dev/null || command -v cc)
   _compiler=$($_compiler -v 2>&1 | grep ' version ' | sed -e 's/([^()]*)//g' | sed -e 's/[ \t]*$$//g' | tr '[:upper:]' '[:lower:]')
else
   _compiler="not found"
fi
echo "Compiler: $_compiler"
echo ""

# Make
_make=""
if command -v make >/dev/null 2>&1; then
   _make=$(readlink -f "$(command -v make)" 2>/dev/null || command -v cc)
   _make=$($_make -v 2>&1 | sed -n '1p' | sed -e 's/([^()]*)//g' | sed -e 's/[ \t]*$$//g' | tr '[:upper:]' '[:lower:]')
else
   _make="Not installed"
fi
echo "Make: $_make"
