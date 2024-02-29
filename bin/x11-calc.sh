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

SUCCESS=0
ENOENT=2
EBFONT=59
ENODATA=61
ENOACC=126 # Permission denied (not official)
ENOCMD=127 # Executable file not found (not official)

DEFAULT=25c

_launch() {
local _fonts _errno _errmsg

[ -z "$MODEL" ] && exit #

case $MODEL in
   10c|11c|12c|15c|16c)
      # if FLAGS does not point to a rom file, set expected option to default location
      # no need to check file existence: app will error-out with proper message if missing
      [ -n "$FLAGS##*.rom*" ] || [ -z "$FLAGS" ] && FLAGS="-r ${XDG_DATA_HOME}/x11-calc-${MODEL}.rom"
   ;;
esac

"$(dirname "$0")"/x11-calc-$MODEL $FLAGS # Assume script is in the same directory as the executable files
_errno=$?
case $_errno in
   $SUCCESS)
      ;;
   $EBFONT)
      _fonts="<i>xfonts-base</i> or equivalent for this distribution."
      if [ -f /run/host/os-release ] # Check os-release exists
      then
         grep -qE "ubuntu|debian" /run/host/os-release && _fonts="<i>xfonts-base</i>"
         grep -qE "fedora" /run/host/os-release && _fonts="<i>xorg-x11-fonts-base</i> or <i>xorg-x11-fonts-misc</i>"
         grep -qE "gentoo" /run/host/os-release && _fonts="<i>font-misc-misc</i>"
      fi
      _errmsg="Please install the following font package:\n\n$_fonts"
      ;;
   $ENOENT)
      _errmsg="Missing ROM file !\nCorrect path in command line, or copy file to defined location:\n\n$FLAGS"
      ;;
   $ENODATA)
      _errmsg="Empty ROM file !\n"
      ;;
   $ENOACC)
      _errmsg="Access denied.\n"
      ;;
   $ENOCMD)
      _errmsg="Command not found.\n"
      ;;
   *)
      _errmsg="An unhandled error occoured !\n"
esac

if [ -n "$_errmsg" ]
then
   zenity --height=100 --width=300 --info --text="$_errmsg"
fi


}

_config (){
   local _selection _options
   local models="35 80 45 70 21 22 25 25c 27 29c 31e 32e 33e 33c 34c 37e 38e 38c 67 10c 11c 12c 15c 16c"

   _selection=$(zenity --list --title="Calculator selection" \
      --text="Choose preferred calculator model:" --column="HP model" $models \
      --ok-label="OK" --height=300 --width=225 2>/dev/null)

   # _options=$(zenity --entry --title="Expert settings" \
   #    --text="Command line options:" --entry-text="$_options" \
   #    --ok-label="Set" --height=100 --width=300 2>/dev/null)

   #_options=""

   # [ -z "$_selection" ] && _selection=$DEFAULT
   [ -n "$_selection" ] && sed -i 's/^MODEL=.*/MODEL="'"$_selection"'"/' "$_filename" # Dont update the selection if none was made (user pressed cancel)
   # sed -i 's/^FLAGS=.*/FLAGS=\"'"$_options"'\"/' "$_filename"
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

## Main

_filename=""

: "${XDG_CONFIG_HOME:="$HOME/.config"}" # Define default path for XDG_CONFIG_HOME if not defined
: "${XDG_DATA_HOME:="$HOME/.local/share"}" # Define default path for XDG_DATA_HOME if not defined

if ! [ -d "$XDG_DATA_HOME" ] # Does XDG_DATA_HOME exist
then
   XDG_DATA_HOME="$HOME" # If not use $HOME
else
   XDG_DATA_HOME="${XDG_DATA_HOME}/x11-calc"
   mkdir -p "$XDG_DATA_HOME"
fi

if [ -d "$XDG_CONFIG_HOME" ] # Does XDG_CONFIG_HOME exist
then
   _filename="${XDG_CONFIG_HOME}/x11-calc/x11-calc.conf" # If it does use it
else
   _filename="${HOME}/.x11-calc.conf"
fi

#echo $XDG_DATA_HOME
#echo $XDG_CONFIG_HOME
echo $_filename

if ! [ -f "$_filename" ]
then
   mkdir -p $(dirname "${_filename}")
   cat <<-EOF >"$_filename"
#
# Select which emulator to run by setting the MODEL to one
# of the following:
#
# 35, 80, 45, 70, 21, 22, 25, 25c, 27, 29c, 31e, 32e, 33e, 33c, 34c, 37e,
# 38e, 38c, 67, 10c, 11c, 12c, 15c, or 16c
#
# FLAGS may contain options as one-liner string to specify:
# preferred non-default save-state file path to be loaded
# (like sample prg presets from /app/share/x11-calc/prg/)
# non-default .rom file path (-r prefix)
# other debug options...
#
# For more complete list of options, run from command-line
# with --help option
# To test FLAGS line and diagnose errors, run from command-line
# with FLAGS line

MODEL="$DEFAULT"
FLAGS=""

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
      FLAGS="--help"
      #echo "$(_launch)" | zenity --height=320 --width=512 --text-info --font="courier"
      zenity  --height=320 --width=512  --info --text "<span font=\"10\" face=\"courier\">$(_launch)</span>\n" --title="Help"
      ;;
   "")
      _launch
      ;;
   *)
      zenity --height=100 --width=200 --info --text="Invalid option !"
      ;;
esac
