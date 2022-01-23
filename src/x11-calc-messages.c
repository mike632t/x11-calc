/*
 * x11-calc-text.c - RPN (Reverse Polish) calculator simulator.
 *
 * Copyright(C) 2019 - MT
 *
 * Text messages.
 *
 * This  program is free software: you can redistribute it and/or modify it
 * under  the terms of the GNU General Public License as published  by  the
 * Free  Software Foundation, either version 3 of the License, or (at  your
 * option) any later version.
 *
 * This  program  is distributed in the hope that it will  be  useful,  but
 * WITHOUT   ANY   WARRANTY;   without even   the   implied   warranty   of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General
 * Public License for more details.
 *
 * You  should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * 21 Jan 22   0.1   - Initial version - MT
 * 23 Jan 22         - Added french language translation (from google) - MT
 *
 */

#define OCTAL

const char * h_msg_licence = "Copyright(C) %s %s\n\
License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>.\n\
This is free software: you are free to change and redistribute it.\n\
There is NO WARRANTY, to the extent permitted by law.\n";

#if defined(HEXADECIMAL)
const char * h_msg_opcode = "%1o-%04x %04x  ";
const char * h_msg_address = "%04x";
#else
const char * h_msg_opcode = "%1o-%04o %04o  ";
const char * h_msg_address = "%04o";
#endif

#if defined(LANG_fr)
const char * h_msg_loading = "Chargement de '%s'.\n";
const char * h_msg_saving = "Enregistrement de '%s'.\n";

const char * h_err_register_alloc = "Erreur d'execution\t : Ligne %s : %d : Echec de l'allocation de memoire !\n";
const char * h_err_opening_file = "Impossible d'ouvrir '%s'.\n";

const char * h_err_display = "Impossible de se connecter au serveur X '%s'.\n";
const char * h_err_display_properties = "Impossible d'obtenir les proprietes d'affichage.\n";
const char * h_err_display_colour = "Nécessite un affichage couleur %d bits.\n";
const char * h_err_font = "Cannot load font '%s'.\n";

#if defined(HEXADECIMAL)
const char * h_err_unexpected_opcode = "Code opération inattendu (%03x) à %1o-%04x dans la ligne %s : %d\n";
const char * h_err_unexpected_error = "Erreur inattendue à %1o-%04x dans la ligne %s : %d\n";
const char * h_err_invalid_address = "Adresse (%02o) hors plage à %1o-%04x dans la ligne %s : %d\n";
const char * h_err_invalid_register = "Registre invalide (REG[%d]) à %1o-%04x dans la ligne %s : %d\n";
# else
const char * h_err_unexpected_opcode = "Code opération inattendu (%04o) à %1o-%04o dans la ligne %s : %d\n";
const char * h_err_unexpected_error = "Erreur inattendue à %1o-%04o dans la ligne %s : %d\n";
const char * h_err_invalid_address = "Adresse (%02o) hors plage à %1o-%04o dans la ligne %s : %d\n";
const char * h_err_invalid_register = "Registre invalide (REG[%d]) à %1o-%04o dans la ligne %s : %d\n";
#endif /* Hexadecmal */

#if defined(unix)
const char * c_msg_usage = "Utilisation : %s [OPTION]... [FICHIER]\n\
Une simulation RPN Calculator pour X11.\n\n\
  -b  ADDR                 définir un point d'arrêt (octal)\n\
  -i, OPCODE               définir un piège d'instruction (octal)\n\
  -s,                      single step\n\
  -t,                      trace execution\n\
      --cursor             curseur d'affichage (par défaut)\n\
      --no-cursor          masquer le curseur\n\
      --help               afficher cette aide et quitter\n\
      --version            affiche les informations de version et quitte\n\n";
const char * h_err_invalid_operand = "opérande(s) invalide(s)\n";
const char * h_err_invalid_option = "option invalide -- '%c'\n";
const char * h_err_unrecognised_option = "option non reconnue '%s'\n";
const char * h_err_invalid_number = "pas un nombre octal -- '%s' \n";
const char * h_err_address_range = "hors de portée -- '%s' \n";
const char * h_err_missing_argument = "l'option necessite un argument -- '%s'\n";
const char * h_err_invalid_argument = "argument attendu -- '%c' \n";
#else
const char * c_msg_usage = "Utilisation : %s [OPTION]... [FICHIER]\n\
Une simulation RPN Calculator pour X11.\n\n\
  /cursor                  curseur d'affichage (par défaut)\n\
  /nocursor                masquer le curseur\n\
  /step                    trace execution\n\
  /trace                   trace execution\n\
  /version                 affiche les informations de version et quitte\n\
  /?, /help                afficher cette aide et quitter\n";

const char * h_err_invalid_operand = "parametre(s) invalide(s)\n";
const char * h_err_invalid_option = "option invalide %s\n";
#endif /* Unix */

#else /* Language defaults to English */

const char * h_msg_loading = "Loading '%s'.\n";
const char * h_msg_saving = "Saving '%s'.\n";

const char * h_err_register_alloc = "Run-time error\t: %s line : %d : Memory allocation failed!\n";
const char * h_err_opening_file = "Unable to open '%s'.\n";

const char * h_err_display = "Cannot connect to X server '%s'.\n";
const char * h_err_display_properties = "Unable to get display properties.\n";
const char * h_err_display_colour = "Requires a %d-bit colour display.\n";
const char * h_err_font = "Cannot load font '%s'.\n";

#if defined(HEXADECIMAL)
const char * h_err_unexpected_opcode = "Unexpected opcode (%03x) at %1o-%04x in %s line : %d\n";
const char * h_err_unexpected_error = "Unexpected error at %1o-%04x in %s line : %d\n";
const char * h_err_invalid_address = "Address (%02o) out of range at %1o-%04x in %s line : %d\n";
const char * h_err_invalid_register = "Invalid register (REG[%d]) at %1o-%04x in %s line : %d\n";
# else
const char * h_err_unexpected_opcode = "Unexpected opcode (%04o) at %1o-%04o in %s line : %d\n";
const char * h_err_unexpected_error = "Unexpected error at %1o-%04o in %s line : %d\n";
const char * h_err_invalid_address = "Address (%02o) out of range at %1o-%04o in %s line : %d\n";
const char * h_err_invalid_register = "Invalid register (REG[%d]) at %1o-%04o in %s line : %d\n";
#endif /* Hexadecimal */

#if defined(unix)
const char * c_msg_usage = "Usage: %s [OPTION]... [FILE]\n\
An RPN Calculator simulation for X11.\n\n\
  -b  ADDR                 set break-point (octal)\n\
  -i, OPCODE               set instruction trap (octal)\n\
  -s,                      single step\n\
  -t,                      trace execution\n\
      --cursor             display cursor (default)\n\
      --no-cursor          hide cursor\n\
      --help               display this help and exit\n\
      --version            output version information and exit\n\n";
const char * h_err_invalid_operand = "invalid operand(s)\n";
const char * h_err_invalid_option = "invalid option -- '%c'\n";
const char * h_err_unrecognised_option = "unrecognised option '%s'\n";
const char * h_err_invalid_number = "not an octal number -- '%s' \n";
const char * h_err_address_range = "out of range -- '%s' \n";
const char * h_err_missing_argument = "option requires an argument -- '%s'\n";
const char * h_err_invalid_argument = "expected argument not -- '%c' \n";
#else
const char * c_msg_usage = "Usage: %s [OPTION...] [FILE]\n\
An RPN Calculator simulation for X11.\n\n\
  /cursor                  display cursor (default)\n\
  /nocursor                hide cursor\n\
  /step                    trace execution\n\
  /trace                   trace execution\n\
  /version                 output version information and exit\n\n\
  /?, /help                display this help and exit\n";

const char * h_err_invalid_operand = "invalid parameter(s)\n";
const char * h_err_invalid_option = "invalid option %s\n";
#endif /* Unix */

#endif /* Language */

