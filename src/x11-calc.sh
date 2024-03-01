#!/bin/sh
#
#  x11-calc.sh
#
#  Copyright(C) 2024 - macmpi / MT
#
#  Simple launcher to start the default emulator and allow a user to select
#  which one should be used by default (using setup option).
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
#  29 Feb 24         - Updated  config()  to use a form allowing the  model
#                      number and command line options to be entered at the
#                      same time...
#                    - If no model is defined run setup(), this way if  the
#                      doesn't select a model they will be asked again next
#                      time the launcher runs - MT
#

SUCCESS=0
ENOENT=2
EBFONT=59
ENODATA=61
ENOACC=126 # Permission denied (not official)
ENOCMD=127 # Executable file not found (not official)

_launch() {
   local _fonts _errmsg _f_os_release

   [ -z "$MODEL" ] && exit 0

   case $MODEL in
      10c|11c|12c|15c|16c)
         # if OPTIONS does not point to a rom file, set expected option to default location
         # no need to check file existence: app will error-out with proper message if missing
         [ -n "$OPTIONS##*.rom*" ] || [ -z "$OPTIONS" ] && OPTIONS="-r ${XDG_DATA_HOME}/x11-calc-${MODEL}.rom"
      ;;
   esac

   "$(dirname "$0")"/x11-calc-$MODEL $OPTIONS # Assume script is in the same directory as the executable files

   case $? in
      $SUCCESS)
         ;;
      $EBFONT)
         _fonts="<i>xfonts-base</i> or equivalent for this distribution."
         _f_os_release="/etc/os-release"
         grep -sq "freedesktop" "${_f_os_release}" && _f_os_release="/run/host/os-release" # we are in flatpak
         grep -sqE "ubuntu|debian" "${_f_os_release}" && _fonts="<i>xfonts-base</i>"
         grep -sq "fedora" "${_f_os_release}" && _fonts="<i>xorg-x11-fonts-base</i> or <i>xorg-x11-fonts-misc</i>"
         grep -sq "gentoo" "${_f_os_release}" && _fonts="<i>font-misc-misc</i>"
         _errmsg="Please install the following font package:\\n\\n$_fonts"
         ;;
      $ENOENT)
         _errmsg="Missing ROM file !\\n\\nCorrect path in command line, or copy file to defined location:\\n\\n$OPTIONS"
         ;;
      $ENODATA)
         _errmsg="Empty ROM file !\\n"
         ;;
      $ENOACC)
         _errmsg="Access denied.\\n"
         ;;
      $ENOCMD)
         _errmsg="Emulator not found.\\n"
         ;;
      *)
         _errmsg="An unhandled error occoured !\\n"
   esac

   [ -n "$_errmsg" ] && zenity --height=100 --width=300 --error --text="$_errmsg"
}

_config (){
   local _selection
   local _models="35|45|70|80|21|22|25|25c|27|29c|31e|32e|33e|33c|34c|37e|38e|38c|67|10c|11c|12c|15c|16c"

   _selection=$(zenity --forms --title="x11-calc Setup" \
      --text="Select model number" \
      --add-list="Model:" --list-values=${_models} \
      --add-entry="Options:" --ok-label="OK" 2>/dev/null)


   case $? in
      0)
         sed -i "s|^MODEL=.*|MODEL=\"${_selection%,|*}\"|" "$_filename"
         sed -i "s|^OPTIONS=.*|OPTIONS=\"${_selection#*,|}\"|" "$_filename"
         ;;
      1)
         exit 0 # User pressed cancel so just quit - don't attempt to launch the emulator
         ;;
      -1)
         echo "An unexpected error has occurred."
      ;;
   esac

}


_setup() {
   if command -v zenity >/dev/null 2>&1
   then
      _config
   else
      nano "$_filename"
   fi

   . "$_filename" # Reload modified settings from config file
}





: "${XDG_CONFIG_HOME:="$HOME/.config"}" # Define default path for XDG_CONFIG_HOME if not defined
: "${XDG_DATA_HOME:="$HOME/.local/share"}" # Define default path for XDG_DATA_HOME if not defined

if [ -d "$XDG_DATA_HOME" ] # Does XDG_DATA_HOME exist
then
   mkdir -p "${XDG_DATA_HOME}/x11-calc" # If it does create the application directory
fi

if [ -d "$XDG_CONFIG_HOME" ] # Does XDG_CONFIG_HOME exist
then
   mkdir -p "${XDG_CONFIG_HOME}/x11-calc" # If it does create the application directory
   _filename="${XDG_CONFIG_HOME}/x11-calc/x11-calc.conf" # If it does use it
else
   _filename="${HOME}/.x11-calc.conf" # Otherwise use a hiddent file in the home folder
fi

echo "$_filename"

if ! [ -f "$_filename" ]
then
   mkdir -p "$(dirname "${_filename}")"
   cat <<-EOF >"$_filename"
#
# Select which emulator to run by setting the MODEL to one
# of the following:
#
# 35, 80, 45, 70, 21, 22, 25, 25c, 27, 29c, 31e, 32e, 33e, 33c, 34c, 37e,
# 38e, 38c, 67, 10c, 11c, 12c, 15c, or 16c
#
# OPTIONS may contain options as one-liner string to specify:
# preferred non-default save-state file path to be loaded
# (like sample prg presets from /app/share/x11-calc/prg/)
# non-default .rom file path (-r prefix)
# other debug options...
#
# For more complete list of options, run from command-line
# with --help option
# To test OPTIONS line and diagnose errors, run from command-line
# with OPTIONS line

MODEL=""
OPTIONS=""

# To call this setup again, run from command-line
# with --setup option

EOF
fi

. "$_filename"

case $* in
   "--setup")
      _setup
      _launch
      ;;
   "--help")
      OPTIONS="--help"
      if command -v zenity >/dev/null 2>&1
      then
         _launch | zenity --title="x11-calc Help" --text-info --font="courier" --height=320 --width=512
      else
         _launch
      fi
      ;;
   "")
      [ -z "$MODEL" ] && _setup
      _launch
      ;;
   *)
      zenity --height=100 --width=200 --info --text="Invalid option !"
      ;;
esac
