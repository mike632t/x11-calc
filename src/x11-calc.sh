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
#  28 Feb 24         - Initial version - macmpi / MT
#  29 Feb 24         - Updated  config()  to use a form allowing the  model
#                      number and command line options to be entered at the
#                      same time...
#                    - If no model is defined run setup(), this way if  the
#                      doesn't select a model they will be asked again next
#                      time the launcher runs - MT
#  01 Mar 24         - Allow the model model name to be passed as the first
#                      command-line parameter. Any other parameters will be
#                      passed through to the application - macmpi
#                    - Handle any errors that occour when an invalid option
#                      or parameter is passed on the command line - MT
#  02 Mar 24         - Allow  OPTIONS  paths  expansion  and improve zenity
#                      detection and fallback for error messages - macmpi
#                    - Prefix model names with 'hp' - MT
#

SUCCESS=0
ENOENT=2
EINVAL=22
ENODATA=61
ENOACC=126 # Permission denied (not official)
ENOCMD=127 # Executable file not found (not official)
ENOFNT=192

_models="hp35|hp45|hp70|hp80|hp21|hp22|hp25|hp25c|hp27|hp29c|hp31e|hp32e|hp33e|hp33c|hp34c|hp37e|hp38e|hp38c|hp67|hp10c|hp11c|hp12c|hp15c|hp16c"


_expand_paths() {
   local _args _path
   _args=""
   while [ -n "$1" ]; do
      case "$1" in
         */*)
            _path="$( echo "echo $1" | sh )"
            _args="$_args $_path"
         ;;
         *)
            _args="$_args $1"
         ;;
      esac
      shift
   done
   printf '%s' "$_args"
}

_launch() {
   local _fonts _errmsg _model _f_os_release

   [ -z "$MODEL" ] && exit 0

   _model="${MODEL#hp}"

   case $MODEL in
      hp10c|hp11c|hp12c|hp15c|hp16c)
         # if OPTIONS does not point to a rom file, set expected option to default location
         # no need to check file existence: app will error-out with proper message if missing
         [ -n "${OPTIONS##*.rom*}" ] || [ -z "$OPTIONS" ] && OPTIONS="-r ${XDG_DATA_HOME}/x11-calc/x11-calc-${_model}.rom"
      ;;
   esac
   [ -n "$CMD_OPTS" ] && OPTIONS="$CMD_OPTS" # Allow command line to override options

   "$(dirname "$0")"/x11-calc-$_model $( _expand_paths $OPTIONS ) # Assume script is in the same directory as the executable files

   case $? in
      $SUCCESS)
         ;;
      $EINVAL)
         _errmsg="Invalid operand or parameter:\\n\\n$OPTIONS"
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
         _errmsg="An unhandled error occurred !\\n"
   esac

   if [ -n "$_errmsg" ]; then
      if _has_zenity; then
         zenity --height=100 --width=300 --error --text="$_errmsg"
      else
         echo "$_errmsg"
      fi
   fi
}


_config (){
   local _selection

# Note that the seperator used by different forms is different!

#  _selection=$(zenity --forms --title="x11-calc Setup" \
#     --text="Select model number" \
#     --add-combo="Model:" --combo-values=${_models} \
#     --add-entry="Options:" --ok-label="OK" \
#     --height=96 --width=256 2>/dev/null)

   _selection=$(zenity --forms --title="x11-calc Setup" \
      --text="Select model number" \
      --add-list="Model:" --list-values=${_models} \
      --add-entry="Options:" --ok-label="OK" 2>/dev/null)


   case $? in
      0)
         #echo "$(basename $0): '$_selection'"
         sed -i "s|^MODEL=.*|MODEL=\"${_selection%,|*}\"|" "$_f_conf"
         sed -i "s|^OPTIONS=.*|OPTIONS=\"${_selection#*,|}\"|" "$_f_conf"
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
   if _has_zenity
   then
      _config
   else
      vi "$_f_conf"
   fi

   . "$_f_conf" # Reload modified settings from config file
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
   _f_conf="${XDG_CONFIG_HOME}/x11-calc/x11-calc.conf" # If it does use it
else
   _f_conf="${HOME}/.x11-calc.conf" # Otherwise use a hiddent file in the home folder
fi

command -v zenity >/dev/null 2>&1
has_zenity=$?
_has_zenity() { return "$has_zenity"; }

if ! [ -f "$_f_conf" ]
then
   mkdir -p "$(dirname "${_f_conf}")"
   cat <<-EOF >"$_f_conf"
#
# Select which emulator to run by setting the MODEL to one
# of the following:
#
# $_models
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

. "$_f_conf"

echo "$(basename $0): '$_f_conf'"

eval "
case \$1 in
   --setup)
      _setup
      _launch
      ;;
   --help)
      [ -z \"\$MODEL\" ] && _setup
      CMD_OPTS=\"--help\"
      if _has_zenity
      then
         _launch | zenity --title=\"x11-calc Help\" --text-info --font=\"courier\" --height=320 --width=512
      else
         _launch
      fi
      ;;
   $_models)
      MODEL=\"\$1\"
      shift
      CMD_OPTS=\"\$*\"
      _launch
      ;;
   *)
      [ -z \"\$MODEL\" ] && _setup
      CMD_OPTS=\"\$*\"
      _launch
      ;;
esac
"
