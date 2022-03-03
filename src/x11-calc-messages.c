/*
 * x11-calc-text.c - RPN (Reverse Polish) calculator simulator.
 *
 * Copyright(C) 2022 - MT
 *
 * With thanks to Alejandro GARZA for providing the Spanish translation and
 * Martin HEPPERLE for translating the text into German.
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
 * 23 Jan 22         - Added a French language translation - MT
 * 24 Jan 22         - Added my first attempt at translating text  messages
 *                     into German and Spanish - MT
 *                   - Reduced the number of digits used to show 12-bit hex
 *                     values to 3 - MT
 *                   - Removed special characters - MT
 * 29 Jan 22         - Updated Spanish translation - MT
 * 31 Jan 22         - Updated German translation - MT
 * 02 Feb 22         - Added formatting strings for relative jumps - MT
 * 03 Mar 22         - Updated help text - MT
 *
 */

#if defined (HP10) || defined (HP11) || defined (HP12) || defined (HP15) || defined (HP16) || defined(HP41)
#define HEXADECIMAL
#endif

/* Remember to define prototype in include file for any new strings */

const char * h_msg_licence = "Copyright(C) %s %s\n\
License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>.\n\
This is free software: you are free to change and redistribute it.\n\
There is NO WARRANTY, to the extent permitted by law.\n";

#if defined(HEXADECIMAL)
const char * h_msg_opcode = "%1x-%03x  %03x   ";
const char * h_msg_address = "%03x";
const char * h_msg_negative_offset = "-0x%x";
const char * h_msg_positive_offset = "+0x%x";
const char * h_msg_rom = "%x:%x";
#else
const char * h_msg_opcode = "%1o-%04o %04o  ";
const char * h_msg_address = "%04o";
const char * h_msg_negative_offset = "-0%o";
const char * h_msg_positive_offset = "+0%o";
const char * h_msg_rom = "%o:%o";
#endif

#if defined(LANG_es)
const char * h_msg_loading = "Cargando '%s'.\n";
const char * h_msg_saving = "Guardando '%s'.\n";

const char * h_err_register_alloc = "Error de ejecucion\t: %s linea: %d: iFallo la asignacion de memoria!\n";
const char * h_err_opening_file = "No se puede abrir '%s'.\n";

const char * h_err_display = "No se pudo conectar al servidor X '%s'.\n";
const char * h_err_display_properties = "No se pudo obtener las propiedades del monitor.\n";
const char * h_err_display_colour = "Requiere un monitor de %d bits de color.\n";
const char * h_err_font = "No se pudo cargar la fuente '%s'.\n";

#if defined(HEXADECIMAL)
const char * h_err_unexpected_opcode = "Codigo de operación inesperado (%03x) en %1x-%03x en %s en la línea : %d\n";
const char * h_err_unexpected_error = "Error inesperado en %1x-%03x en %s: en la línea: %d\n";
const char * h_err_invalid_address = "Direccion (%02o) fuera de rango en %1x-%03x en %s en la línea : %d\n";
const char * h_err_invalid_register = "Registro invalido (REG[%d]) en %1x-%03x en %s en la línea : %d\n";
# else
const char * h_err_unexpected_opcode = "Codigo de operación inesperado (%04o) en %1o-%04o en %s en la línea : %d\n";
const char * h_err_unexpected_error = "Error inesperado en %1o-%04o en %s: en la línea: %d\n";
const char * h_err_invalid_address = "Direccion (%02o) fuera de rango en %1o-%04o en %s en la línea : %d\n";
const char * h_err_invalid_register = "Registro inválido (REG[%d]) en %1o-%04o en %s en la línea : %d\n";
#endif /* Hexadecimal */

#if defined(unix)
const char * c_msg_usage = "Uso: %s [OPCION]... [ARCHIVO]\n\
Simularor de calculadora RPN para X11.\n\n\
  -b  ADDR                 punto de interrupcion (octal)\n\
  -i  OPCODE               instruccion de trampa (octal)\n\
  -r  FILE                 leer el contenido de la ROM de FILE\n\
  -s,                      un paso\n\
  -t,                      seguimiento de la ejecucion\n\
      --cursor             mostrar cursor (default)\n\
      --no-cursor          ocultar cursor\n\
      --help               mostrar esta ayuda y salir\n\
      --version            mostrar version y salir\n\n";
const char * h_err_invalid_operand = "operando(s) inválido\n";
const char * h_err_invalid_option = "opcion invalida -- '%c'\n";
const char * h_err_unrecognised_option = "opcion no reconocida '%s'\n";
const char * h_err_invalid_number = "no es un numero octal -- '%s' \n";
const char * h_err_address_range = "fuera del rango  -- '%s' \n";
const char * h_err_missing_argument = "opcion requiere un argumento -- '%s'\n";
const char * h_err_invalid_argument = "argumento esperado no es -- '%c' \n";
#else
const char * c_msg_usage = "Uso: %s [OPCION]... [ARCHIVO]\n\
Una simulación de calculadora RPN para X11.\n\n\
  /cursor                  mostrar cursor (default)\n\
  /nocursor                ocultar cursor\n\
  /step                    un paso\n\
  /trace                   seguimiento de la ejecucion\n\
  /version                 mostrar version y salir\n\
  /?, /help                mostrar esta ayuda y salir\n";

const char * h_err_invalid_operand = "parametro(s) invalido(s)\n";
const char * h_err_invalid_option = "opcion invalida %s\n";
#endif /* Unix */


#elif defined(LANG_de)
const char * h_msg_loading = "Lade '%s'.\n";
const char * h_msg_saving = "Speichern '%s'.\n";

const char * h_err_register_alloc = "Laufzeitfehler\t: %s Zeile : %d : Speicherzuweisung fehlgeschlagen!\n";
const char * h_err_opening_file = "Kann '%s' nicht öffnen.\n";

const char * h_err_display = "Kann keine verbindung zum X-Server '%s' herstellen.\n";
const char * h_err_display_properties = "Kann eigenschaften des displays nicht abfragen.\n";
const char * h_err_display_colour = "Ein %d-bit farbdisplay wird benotigt.\n";
const char * h_err_font = "Kann schrift '%s' nicht laden.\n";

#if defined(HEXADECIMAL)
const char * h_err_unexpected_opcode = "Unerwarteter opcode (%03x) an %1x-%03x in %s zeile: %d\n";
const char * h_err_unexpected_error = "Nicht abgefangener fehler an %1x-%03x in %s zeile: %d\n";
const char * h_err_invalid_address = "Ungültige adresse (%02o) an %1x-%03x in %s zeile : %d\n";
const char * h_err_invalid_register = "Ungultiges register (REG[%d]) an %1x-%03x in %s zeile : %d\n";
# else
const char * h_err_unexpected_opcode = "Unerwarteter opcode (%04o) bei %1o-%04o in %s zeile : %d\n";
const char * h_err_unexpected_error = "Nicht abgefangener fehler an %1o-%04o in %s zeile : %d\n";
const char * h_err_invalid_address = "Ungültige adresse (%02o) an %1o-%04o in %s zeile : %d\n";
const char * h_err_invalid_register = "Ungultiges register (REG[%d]) an %1o-%04o in %s zeile : %d\n";
#endif /* Hexadecimal */

#if defined(unix)
const char * c_msg_usage = "Verwendung: %s [OPTION]... [DATEI]\n\
Eine RPN rechner simulation fur X11.\n\n\
  -b  ADDR                 haltepunkt an adresse setzen (oktal)\n\
  -i, OPCODE               haltepunkt anf opcode setzen (oktal)\n\
  -r  FILE                 lesen sie den ROM inhalt von FILE\n\
  -s,                      einzelschritt\n\
  -t,                      ausfuhrung protokollieren\n\
      --cursor             cursor anzeigen (default)\n\
      --no-cursor          cursor verstecken\n\
      --help               diese hilfe anzeigen und dann beenden\n\
      --version            versionsinformationen ausgeben und dann beenden\n\n";
const char * h_err_invalid_operand = "ungultige(r) operand(en)\n";
const char * h_err_invalid_option = "ungultige option -- '%c'\n";
const char * h_err_unrecognised_option = "unbekannte option '%s'\n";
const char * h_err_invalid_number = "keine gultige oktalzahl -- '%s' \n";
const char * h_err_address_range = "auberhalb des adressbereichs -- '%s' \n";
const char * h_err_missing_argument = "option benotigt ein argument -- '%s'\n";
const char * h_err_invalid_argument = "erwartetes argument, nicht -- '%c' \n";
#else
const char * c_msg_usage = "Verwendung: %s [OPTION...] [DATEI]\n\
Eine RPN rechner simulation fur X11.\n\n\
  /cursor                  cursor anzeigen (default)\n\
  /nocursor                cursor verstecken\n\
  /step                    einzelschritt\n\
  /trace                   ausfuhrung protokollieren\n\
  /version                 versionsinformationen ausgeben und dann beenden\n\
  /?, /help                diese hilfe anzeigen und dann beenden\n";

const char * h_err_invalid_operand = "ungultige(r) parameter\n";
const char * h_err_invalid_option = "ungultige option %s\n";
#endif /* Unix */


#elif defined(LANG_fr)
const char * h_msg_loading = "Chargement de '%s'.\n";
const char * h_msg_saving = "Enregistrement de '%s'.\n";

const char * h_err_register_alloc = "Erreur d'execution\t : Ligne %s : %d : Echec de l'allocation de memoire !\n";
const char * h_err_opening_file = "Impossible d'ouvrir '%s'.\n";

const char * h_err_display = "Impossible de se connecter au serveur X '%s'.\n";
const char * h_err_display_properties = "Impossible d'obtenir les proprietes d'affichage.\n";
const char * h_err_display_colour = "Nécessite un affichage couleur %d bits.\n";
const char * h_err_font = "Impossible de charger la police '%s'.\n";

#if defined(HEXADECIMAL)
const char * h_err_unexpected_opcode = "Code opération inattendu (%03x) à %1x-%03x dans la ligne %s : %d\n";
const char * h_err_unexpected_error = "Erreur inattendue à %1x-%03x dans la ligne %s : %d\n";
const char * h_err_invalid_address = "Adresse (%02o) hors plage à %1x-%03x dans la ligne %s : %d\n";
const char * h_err_invalid_register = "Registre invalide (REG[%d]) à %1x-%03x dans la ligne %s : %d\n";
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
  -r  FILE                 lire le contenu de la ROM de FILE\n\
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
const char * h_err_unexpected_opcode = "Unexpected opcode (%03x) at %1x-%03x in %s line : %d\n";
const char * h_err_unexpected_error = "Unexpected error at %1x-%03x in %s line : %d\n";
const char * h_err_invalid_address = "Address (%02o) out of range at %1x-%03x in %s line : %d\n";
const char * h_err_invalid_register = "Invalid register (REG[%d]) at %1x-%03x in %s line : %d\n";
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
  -r  FILE                 read ROM contents from FILE\n\
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
  /step                    single step\n\
  /trace                   trace execution\n\
  /version                 output version information and exit\n\
  /?, /help                display this help and exit\n";

const char * h_err_invalid_operand = "invalid parameter(s)\n";
const char * h_err_invalid_option = "invalid option %s\n";
#endif /* Unix */

#endif /* Language */
