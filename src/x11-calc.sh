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
#                    - Updated to work on Tru64 UNIX - MT
#  03 Mar 24         - Changed to use a drop down selection rather then the
#                      original list box - MT
#                    - Changed  default back to a list box, due to an issue
#                      with the way zenity displays dropdown dialogs - MT
#                    - Model selection dialog box  style may be selected in
#                      the configuration file - MT
#                    - Tidied up default configuration file, and added  the
#                      script name to all console messages, enabled the use
#                      of 'nano' if it is installed - MT
#                    - Reformatted error messages so they will work on both
#                      the console and message box - MT
#                    - Error messages always show on console - MT
#

SUCCESS=0
ENOENT=2
EINVAL=22
ENODATA=61
ENOACC=126 # Permission denied (not official)
ENOCMD=127 # Executable file not found (not official)
ENOFNT=192 # No font (not official)

_classic="hp35|hp45|hp70|hp80"
_woodstock="hp21|hp22|hp25|hp25c|hp27|hp29c"
_topcat="hp67"
_spice="hp31e|hp32e|hp33e|hp33c|hp34c|hp37e|hp38e|hp38c"
_voyager="hp10c|hp11c|hp12c|hp15c|hp16c"
_models="$_classic|$_woodstock|$_topcat|$_spice|$_voyager"


_expand_paths() {

   _args=""
   while [ -n "$1" ]; do
      case "$1" in
         */*)
            _path="` echo "echo $1" | sh `"
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

   if [ -z "$MODEL" ]; then exit 0; fi

   _model="`echo $MODEL | sed 's/^hp//'`"

eval "
   case \$MODEL in
      $_voyager)
         # if OPTIONS does not point to a rom file, set expected option to default location
         # no need to check file existence: app will error-out with proper message if missing
         [ -n \"\${OPTIONS##*.rom*}\" ] || [ -z \"\$OPTIONS\" ] && OPTIONS=\"-r \${XDG_DATA_HOME}/x11-calc/x11-calc-\${_model}.rom\"
      ;;
   esac
"

   [ -n "$CMD_OPTS" ] && OPTIONS="$CMD_OPTS" # Allow command line to override options

   _model="/x11-calc-$_model"
   echo "`basename $0`: Executing '`dirname "$0"`"$_model `_expand_paths $OPTIONS`"'."

   if [ -f "`dirname "$0"`"$_model ]; then
      "`dirname "$0"`"$_model `_expand_paths $OPTIONS` # Assume script is in the same directory as the executable files
   else
      `exit $ENOCMD`
   fi

   case $? in
      $SUCCESS)
         ;;
      $EINVAL)
         _errmsg="Invalid operand or parameter '$OPTIONS'."
         ;;
      $ENOFNT)
         _fonts="'xfonts-base' or equivalent"
         _f_os_release="/etc/os-release"
         grep -sq "freedesktop" "${_f_os_release}" && _f_os_release="/run/host/os-release" # we are in flatpak
         grep -sqE "ubuntu|debian" "${_f_os_release}" && _fonts="'xfonts-base'"
         grep -sq "fedora" "${_f_os_release}" && _fonts="'xorg-x11-fonts-base' or 'xorg-x11-fonts-misc'"
         grep -sq "gentoo" "${_f_os_release}" && _fonts="'font-misc-misc'"
         _errmsg="Please install missing fonts in $_fonts."
         ;;
      $ENOENT)
         _errmsg="Missing ROM file!"
         ;;
      $ENODATA)
         _errmsg="Empty ROM file!"
         ;;
      $ENOACC)
         _errmsg="Access denied."
         ;;
      $ENOCMD)
         _errmsg="Emulator not found."
         ;;
      *)
         _errmsg="An unhandled error occurred!"
   esac

   if [ -n "$_errmsg" ]; then
      if _has_zenity; then
         zenity --height=100 --width=300 --error --text="$_errmsg"
      fi
      echo "`basename $0`: $_errmsg"
   fi
}


_config (){

   if [ "$DROPDOWN" ]
   then
      _selection=`zenity --forms --title="x11-calc Setup" \
         --text="Select model number" \
         --add-combo="Model:" --combo-values=${_models} \
         --add-entry="Options:" --ok-label="OK" \
         --height=96 --width=256 2>/dev/null`
   else
      _selection=`zenity --forms --title="x11-calc Setup" \
         --text="Select model number" \
         --add-list="Model:" --list-values=${_models} \
         --add-entry="Options:" --ok-label="OK" \
         --width=256 2>/dev/null`
   fi

   case $? in
      0)
         if [ "$DROPDOWN" ] # The seperator used by different forms is different!
         then
            sed -i "s|^MODEL=.*|MODEL=\"${_selection%|*}\"|" "$_f_conf"
            sed -i "s|^OPTIONS=.*|OPTIONS=\"${_selection#*|}\"|" "$_f_conf"
         else
            sed -i "s|^MODEL=.*|MODEL=\"${_selection%,|*}\"|" "$_f_conf"
            sed -i "s|^OPTIONS=.*|OPTIONS=\"${_selection#*,|}\"|" "$_f_conf"
         fi
         ;;
      1)
         exit 0 # User pressed cancel so just quit - don't attempt to launch the emulator
         ;;
      *)
         echo "`basename $0`: An unexpected error has occurred."
      ;;
   esac

}


_setup() {
   if _has_zenity
   then
      _config
   else
      if command -v nano >/dev/null 2>&1
      then
         nano "$_f_conf"
      else
         vi "$_f_conf"
      fi
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

if [ ! -f "$_f_conf" ]
then
   if ! _has_zenity; then echo "\n`basename $0`: Did you know that installing 'zenity' will give you a graphical setup interface.\n"; fi
   mkdir -p "`dirname "${_f_conf}"`"
   cat <<-EOF >"$_f_conf"
#
# Select which emulator to run by setting the MODEL to one
# of the following:
#
# classic: $_classic
# woodstock: $_woodstock
# topcat: $_topcat
# spice: $_spice
# voyager: $_voyager
#

MODEL=""
OPTIONS=""

#
# To see a list of possible options define the default model above
# and use:
#
# '$0 --help'
#

EOF
fi

echo "`basename $0`: Using '$_f_conf'"

. "$_f_conf"

if [ -z "$MODEL" ]; then
   echo "`basename $0`: Model not defined - configure using '$0 --setup'"
fi

eval "
case \$1 in
   --setup)
      _setup
      _launch
      ;;
   --help)
      if [ -z \"\$MODEL\" ]; then _setup; fi
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
      if [ -z \"\$MODEL\" ]; then _setup; fi
      CMD_OPTS=\"\$*\"
      _launch
      ;;
esac
"

