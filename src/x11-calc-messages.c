/*
 * x11-calc-text.c - RPN (Reverse Polish) calculator simulator.
 *
 * Copyright(C) 2022 - MT
 *
 * With thanks to Alejandro GARZA for providing the Spanish translation and
 * Martin HEPPERLE for translating the text into German.  Any errors in the
 * text are as the result of changes I've made to limit the characters used
 * to those in the 8-bit ACSII character set.
 *
 * To use a specific language set the environment variable before building.
 *
 * Spanish -   export LANG=es_ES.UTF-8
 * German -    export LANG=de_DE.UTF-8
 * French -    export LANG=fr_FR.UTF-8
 * Swedish -   export LANG=sv_SE.UTF-8
 * English -   export LANG=en_GB.UTF-8
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
 * 21 Jan 22         - Initial version - MT
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
 * 12 Mar 22         - Updated German translation again - MT
 * 31 Mar 22         - Modified to compile on NetBSD - MT
 * 07 Dec 22         - Display register number as a 3 digit value - MT
 * 11 Dec 22         - Renamed models with continuous memory and added HP25
 *                     HP33e, and HP38e - MT
 * 24 Dec 22         - Added and explicit check for '__APPLE__' in order to
 *                     allow Mac OS  to be handled in the same way as other
 *                     unix like systems - MT
 * 18 Jan 23         - Shortened help message (max string length for C90 is
 *                     509 characters) - MT
 * 01 Nov 23         - Changed the missing argument text so it is common to
 *                     all platforms - MT
 * 10 Feb 24         - Added an error message to warn the user that the ROM
 *                     contents are empty - MT
 * 16 Feb 24         - Added 'Can't create' error message - MT
 * 25 Feb 24         - Expanded 'Cannot load font' error message - MT
 * 29 Feb 24         - Changed description from simulator to emulator - MT
 * 12 Apr 24         - Updated error messages to make the out of range text
 *                     more generic (only German translation changed) - MT
 *                   - Added a new command line option '--zoom' to the help
 *                     text - MT
 * 04 May 24         - Updated French translations - macmpi
 * 06 May 24         - Use only 7-bit ASCII characters - MT
 * 12 Jun 25         - Fixed some spelling mistakes in comments - MT
 *                   - Added  error messages for the number of  breakpoints
 *                     and instruction traps - MT
 * 29 Jun 25         - Changed command line option for the european display
 *                     format to '-c' or '--comma', and updated text - MT
 * 10 Sep 25         - Changed message variable names - MT
 * 30 Oct 25         - Added invalid window geometry message - MT
 * 05 Nov 25         - Fixed compile errors caused by variable names - MT
 *                   - Use correct language code for Swedish - MT
 *                   - Use arrays to hold multiple lines as this allows pre
 *                     processor directives to modify output - MT
 *                   - Moved error(), warning() and usage() routines - MT
 * 17 Nov 25         - Updated invalid address messages to use a consistent
 *                     number of digits for the address - MT
 * 22 Nov 25         - Added read and write error messages - MT
 *
 */

#define NAME           "x11-calc-messages"
#define BUILD          "0036"
#define DATE           "22 Nov 25"
#define AUTHOR         "MT"

#include <errno.h>     /* errno */

#include <stdarg.h>    /* vargs(), etc */
#include <string.h>    /* strlen(), etc */
#include <stdio.h>     /* fprintf(), etc */
#include <stdlib.h>    /* getenv(), etc */

#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include "x11-calc-label.h"
#include "x11-calc-switch.h"
#include "x11-calc-button.h"
#include "x11-calc-card.h"

#include "x11-calc-cpu.h"

#if defined(HP10c) || defined(HP11c) || defined(HP12c)|| defined(HP15c) || defined(HP16c)
#define HEXADECIMAL
#endif

/* Remember to define prototype in include file for any new strings */

const char *h_msg_licence[] =
{
   "Copyright(C) %s %s\n",
   "License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>.\n",
   "This is free software: you are free to change and redistribute it.\n",
   "There is NO WARRANTY, to the extent permitted by law.\n",
   NULL
};

const char *h_err_creating_file = "Can't create '%s'.\n";  /** Translation needed */
const char *h_err_memmory_alloc = "Memory allocation failed in %s line : %d\n";  /** Translation needed */
const char *h_err_ROM = "Empty ROM - no firmware loaded.\n";  /** Translation needed */
const char *h_err_geometry = "Invalid geometry: %s (expected: WxH[+X+Y])\n";  /** Translation needed */

#if defined(HEXADECIMAL)
const char *h_msg_opcode = "%1x-%03x  %03x   ";
const char *h_msg_address = "%1x-%03x";
const char *h_msg_number = "%03x";
const char *h_msg_negative_offset = "-0x%x";
const char *h_msg_positive_offset = "+0x%x";
const char *h_msg_rom = "%x:%x";
#else
const char *h_msg_opcode = "%1o-%04o %04o  ";
const char *h_msg_address = "%1o-%04o";
const char *h_msg_number = "%04o";
const char *h_msg_negative_offset = "-0%o";
const char *h_msg_positive_offset = "+0%o";
const char *h_msg_rom = "%o:%o";
#endif

#if defined(LANG_es)
const char *h_msg_loading = "Cargando '%s'\n";
const char *h_msg_saving = "Guardando '%s'\n";

const char *h_err_register_alloc = "Error de ejecucion\t: %s linea: %d: iFallo la asignacion de memoria!\n";
const char *h_err_opening_file = "No se puede abrir '%s'\n";
const char *h_err_reading_file = "Error reading file\n";
const char *h_err_writing_file = "Error writing to file\n";

const char *h_err_display = "No se pudo conectar al servidor X '%s'\n";
const char *h_err_display_properties = "No se pudo obtener las propiedades del monitor\n";
const char *h_err_display_colour = "Requiere un monitor de %d bits de color\n";
const char *h_err_pixmap = "No se puede crear un pixmap\n";
const char *h_err_font = "No se pudo cargar la fuente '%s' (x11 base bitmap fonts required)\n";

#if defined(HEXADECIMAL)
const char *h_err_unexpected_opcode = "Codigo de operación inesperado (%03x) en %1x-%03x en %s en la línea : %d\n";
const char *h_err_unexpected_error = "Error inesperado en %1x-%03x en %s: en la línea: %d\n";
const char *h_err_invalid_address = "Direccion (%03x) fuera de rango en %1x-%03x en %s en la línea : %d\n";
const char *h_err_invalid_register = "Registro invalido (REG[%03d]) en %1x-%03x en %s en la línea : %d\n";
# else
const char *h_err_unexpected_opcode = "Codigo de operación inesperado (%04o) en %1o-%04o en %s en la línea : %d\n";
const char *h_err_unexpected_error = "Error inesperado en %1o-%04o en %s: en la línea: %d\n";
const char *h_err_invalid_address = "Direccion (%04o) fuera de rango en %1o-%04o en %s en la línea : %d\n";
const char *h_err_invalid_register = "Registro inválido (REG[%03d]) en %1o-%04o en %s en la línea : %d\n";
#endif /* Hexadecimal */

const char *h_err_missing_argument = "opcion requiere un argumento -- '%s'\n";

#if defined(unix) || defined(__unix__) || defined(__APPLE__)
const char *h_msg_usage[] =
{
   "Uso: %s [OPCION]... [ARCHIVO]\n",
   "Un emulador de calculadora RPN para X11.\n\n",
   "  -b  ADDR                 punto de interrupcion (octal or hex)\n",
   "  -i  OPCODE               instruccion de trampa (octal or hex)\n",
   "  -r  FILE                 leer el contenido de la ROM de FILE\n",
   "  -s                       un paso\n",
   "  -t                       seguimiento de la ejecucion\n",
#if defined(HP31e) || defined(HP32e) || defined(HP33e) || defined(HP33c) || defined(HP34c) || defined(HP37e) || defined(HP38e) || defined(HP38c)
   "  -c, --comma              utiliza una coma como separador decimal\n",
#endif
   "      --cursor             mostrar cursor (default)\n",
   "      --no-cursor          ocultar cursor\n",
   "      --geometry +x+y      specify initial windows position\n",
   "      --numlock            use numeric keypad (even if numlock is not on)\n",
#if defined(CONTINIOUS)
   "      --reset              do not restore saved state (factory reset)\n",
#endif
   "      --zoom ZOOM          ampliar el tamaño de la ventana\n",
   "      --help               mostrar esta ayuda y salir\n",
   "      --version            mostrar version y salir\n\n",
   NULL
};
const char *h_err_invalid_operand = "operando(s) inválido\n";
const char *h_err_invalid_option = "opcion invalida -- '%c'\n";
const char *h_err_duplicate_option = "opcion duplicato -- '%c'\n";
const char *h_err_unrecognised_option = "opcion no reconocida '%s'\n";
const char *h_err_invalid_number = "no es un numero octal -- '%s'\n";
const char *h_err_numeric_range = "fuera del rango -- '%s'\n";
const char *h_err_max_breakpoints = "numero maximo de puntos de interrupcion excedido\n";
const char *h_err_invalid_argument = "argumento esperado no es -- '%c'\n";
#else
const char *h_msg_usage[] =
{
   "Uso: %s [OPCION]... [ARCHIVO]\n",
   "Una emulador de calculadora RPN para X11.\n\n",
   "  /cursor                  mostrar cursor (default)\n",
   "  /nocursor                ocultar cursor\n",
   "  /step                    un paso\n",
   "  /trace                   seguimiento de la ejecucion\n",
   "  /version                 mostrar version y salir\n",
   "  /?, /help                mostrar esta ayuda y salir\n",
   NULL
};
const char *h_err_invalid_operand = "parametro(s) invalido(s)\n";
const char *h_err_invalid_option = "opcion invalida %s\n";
#endif /* Unix */


#elif defined(LANG_de)
const char *h_msg_loading = "Lade '%s'\n";
const char *h_msg_saving = "Speichere '%s'\n";

const char *h_err_register_alloc = "Laufzeitfehler\t: %s Zeile : %d : Speicheranforderung fehlgeschlagen!\n";
const char *h_err_opening_file = "Kann '%s' nicht oeffnen\n";
const char *h_err_reading_file = "Error reading file\n";
const char *h_err_writing_file = "Error writing to file\n";

const char *h_err_display = "Kann keine verbindung zum X Server '%s' herstellen\n";
const char *h_err_display_properties = "Kann eigenschaften des displays nicht abfragen\n";
const char *h_err_display_colour = "Ein %d-bit farbdisplay wird benoetigt\n";
const char *h_err_pixmap = "Pixmap kann nicht erstellt werden\n";
const char *h_err_font = "Kann schrift '%s' nicht laden (x11 base bitmap fonts required)\n";

#if defined(HEXADECIMAL)
const char *h_err_unexpected_opcode = "Unerwarteter opcode (%03x) an %1x-%03x in %s zeile: %d\n";
const char *h_err_unexpected_error = "Nicht abgefangener fehler an %1x-%03x in %s zeile: %d\n";
const char *h_err_invalid_address = "Ungueltige adresse (%03x) an %1x-%03x in %s zeile : %d\n";
const char *h_err_invalid_register = "Ungueltiges register (REG[%03d]) an %1x-%03x in %s zeile : %d\n";
# else
const char *h_err_unexpected_opcode = "Unerwarteter opcode (%04o) bei %1o-%04o in %s zeile : %d\n";
const char *h_err_unexpected_error = "Nicht abgefangener fehler an %1o-%04o in %s zeile : %d\n";
const char *h_err_invalid_address = "Ungültige adresse (%04o) an %1o-%04o in %s zeile : %d\n";
const char *h_err_invalid_register = "Ungultiges register (REG[%03d]) an %1o-%04o in %s zeile : %d\n";
#endif /* Hexadecimal */

const char *h_err_missing_argument = "option benoetigt ein argument -- '%s'\n";

#if defined(unix) || defined(__unix__) || defined(__APPLE__)
const char * h_msg_usage[] =
{
   "Verwendung: %s [OPTION]... [DATEI]\n",
   "Eine RPN rechner-emulator fur X11.\n\n",
   "  -b  ADDR                 haltepunkt an adresse setzen (oktal)\n",
   "  -i  OPCODE               haltepunkt auf Opcode setzen  (oktal)\n",
   "  -r  FILE                 lesen sie den ROM inhalt von FILE\n",
   "  -s                       einzelschritt\n",
   "  -t                       ausfuehrung protokollieren\n",
#if defined(HP31e) || defined(HP32e) || defined(HP33e) || defined(HP33c) || defined(HP34c) || defined(HP37e) || defined(HP38e) || defined(HP38c)
   "  -c, --comma              dezimaltrennzeichen ist ein komma\n",
#endif
   "      --cursor             cursor anzeigen (default)\n",
   "      --no-cursor          cursor verbergen\n",
   "      --geometry +x+y      specify initial window position\n",  /** Translation needed */
   "      --numlock            use numeric keypad (even if numlock is off)\n", /** Translation needed */
#if defined(CONTINIOUS)
   "      --reset              do not restore saved state (factory reset)\n",  /** Translation needed */
#endif
   "      --zoom ZOOM          fenstergrobe erweitern\n",
   "      --help               diese hilfe anzeigen und dann beenden\n",
   "      --version            versionsinformationen ausgeben und dann beenden\n\n",
   NULL
};
const char *h_err_invalid_operand = "uvalor duplicadongueltige(r) operand(en)\n";
const char *h_err_invalid_option = "ungueltige option -- '%c'\n";
const char *h_err_duplicate_option = "doppelter option -- '%c'\n";
const char *h_err_unrecognised_option = "unbekannte option '%s'\n";
const char *h_err_invalid_number = "keine gueltige oktalzahl -- '%s'\n";
const char *h_err_numeric_range = "ausserhalb des zahlenbereichs -- '%s'\n";
const char *h_err_max_breakpoints = "maximale anzahl an haltepunkten uberschritten\n";
const char *h_err_invalid_argument = "argument erwartet, nicht -- '%c'\n";
#else
const char *h_msg_usage[] =
{
   "Verwendung: %s [OPTION...] [DATEI]\n",
   "Eine RPN rechner-emulator fur X11.\n\n",
   "  /cursor                  cursor anzeigen (default)\n",
   "  /nocursor                cursor verstecken\n",
   "  /step                    einzelschritt\n",
   "  /trace                   ausfuhrung protokollieren\n",
   "  /version                 versionsinformationen ausgeben und dann beenden\n",
   "  /?, /help                diese hilfe anzeigen und dann beenden\n",
   NULL
};
const char *h_err_invalid_operand = "ungueltige(r) parameter\n";
const char *h_err_invalid_option = "ungueltige option %s\n";
#endif /* Unix */


#elif defined(LANG_fr)
const char *h_msg_loading = "Chargement de '%s'\n";
const char *h_msg_saving = "Enregistrement de '%s'\n";

const char *h_err_register_alloc = "Erreur d'execution\t : Ligne %s : %d : Echec de l'allocation memoire !\n";
const char *h_err_opening_file = "Impossible d'ouvrir '%s'\n";
const char *h_err_reading_file = "Error reading file\n";
const char *h_err_writing_file = "Error writing to file\n";

const char *h_err_display = "Impossible de se connecter au serveur X '%s'\n";
const char *h_err_display_properties = "Impossible d'obtenir les proprietes d'affichage\n";
const char *h_err_display_colour = "Necessite un affichage couleur %d bits\n";
const char *h_err_pixmap = "Impossible de creer pixmap\n";
const char *h_err_font = "Impossible de charger la police '%s' (x11 base bitmap fonts required)\n";

#if defined(HEXADECIMAL)
const char *h_err_unexpected_opcode = "Instruction inattendue (%03x) a %1x-%03x , ligne %s : %d\n";
const char *h_err_unexpected_error = "Erreur inattendue a %1x-%03x , ligne %s : %d\n";
const char *h_err_invalid_address = "Adresse (%03x) hors plage a %1x-%03x , ligne %s : %d\n";
const char *h_err_invalid_register = "Registre invalide (REG[%03d]) a %1x-%03x , ligne %s : %d\n";
# else
const char *h_err_unexpected_opcode = "Instruction inattendue (%04o) a %1o-%04o , ligne %s : %d\n";
const char *h_err_unexpected_error = "Erreur inattendue a %1o-%04o , ligne %s : %d\n";
const char *h_err_invalid_address = "Adresse (%04o) hors plage a %1o-%04o , ligne %s : %d\n";
const char *h_err_invalid_register = "Registre invalide (REG[%03d]) a %1o-%04o , ligne %s : %d\n";
#endif /* Hexadecmal */

const char *h_err_missing_argument = "l'option necessite un argument -- '%s'\n";

#if defined(unix) || defined(__unix__) || defined(__APPLE__)
const char *h_msg_usage[] =
{
   "Utilisation : %s [OPTION]... [FICHIER]\n",
   "Une émulateur de RPN calculatrice pour X11.\n\n",
   "  -b  ADDR                 définir un point d'arrêt (octal or hex)\n",
   "  -i  OPCODE               définir un piège d'instruction (octal or hex)\n",
   "  -r  FILE                 lire le contenu de la ROM de FILE\n",
   "  -s                       single step\n",
   "  -t                       trace execution\n",
#if defined(HP31e) || defined(HP32e) || defined(HP33e) || defined(HP33c) || defined(HP34c) || defined(HP37e) || defined(HP38e) || defined(HP38c)
   "  -c, --comma              utiliser une virgule comme separateur decimal\n",
#endif
   "      --cursor             curseur d'affichage (par défaut)\n",
   "      --no-cursor          masquer le curseur\n",
   "      --geometry +x+y      specify initial window position\n",  /** Translation needed */
   "      --numlock            use numeric keypad (even if numlock is off)\n", /** Translation needed */
#if defined(CONTINIOUS)
   "      --reset              do not restore saved state (factory reset)\n",  /** Translation needed */
#endif
   "      --zoom ZOOM          agrandir la taille de la fenêtre\n",
   "      --help               afficher cette aide et quitter\n",
   "      --version            affiche les informations de version et quitte\n\n",
   NULL
};
const char *h_err_invalid_operand = "operande(s) invalide(s)\n";
const char *h_err_invalid_option = "option invalide -- '%c'\n";
const char *h_err_duplicate_option = "valeur en double -- '%c'\n";
const char *h_err_unrecognised_option = "option non reconnue '%s'\n";
const char *h_err_invalid_number = "pas un nombre octal -- '%s'\n";
const char *h_err_numeric_range = "hors de portee -- '%s'\n";
const char *h_err_max_breakpoints = "nombre maximal de points d'arret depasse\n";
const char *h_err_invalid_argument = "argument attendu -- '%c'\n";
#else
const char *h_msg_usage[] =
{
   "Utilisation : %s [OPTION]... [FICHIER]\n",
   "Une émulateur de RPN calculatrice pour X11.\n\n",
   "  /cursor                  curseur d'affichage (par defaut)\n",
   "  /nocursor                masquer le curseur\n",
   "  /step                    trace execution\n",
   "  /trace                   trace execution\n",
   "  /version                 affiche les informations de version et quitte\n",
   "  /?, /help                afficher cette aide et quitter\n",
   NULL
};
const char *h_err_invalid_operand = "parametre(s) invalide(s)\n";
const char *h_err_invalid_option = "option invalide %s\n";
#endif /* Unix */


#elif defined(LANG_sv)

const char *h_msg_loading = "Laddar '%s'\n";
const char *h_msg_saving = "Sparar '%s'\n";

const char *h_err_register_alloc = "Exekveringsfel\t: %s rad : %d : Fel vid Minnesallokering!\n";
const char *h_err_opening_file = "Kan inte oeppna '%s'\n";
const char *h_err_reading_file = "Error reading file\n";
const char *h_err_writing_file = "Error writing to file\n";

const char *h_err_display = "Kan inte ansluta till X server '%s'\n";
const char *h_err_display_properties = "Kan inte hamta display-egenskaper\n";
const char *h_err_display_colour = "Kraever %d-bit faergdjup\n";
const char *h_err_pixmap = "Kan inte skaps pixmap\n";
const char *h_err_font = "Kan inte ladda font '%s' (x11 base bitmap fonter kraevs)\n";

#if defined(HEXADECIMAL)
const char *h_err_unexpected_opcode = "Ovaentad opcode (%03x) vid %1x-%03x i %s rad: %d\n";
const char *h_err_unexpected_error = "Ovaentat fel vid %1x-%03x i %s rad: %d\n";
const char *h_err_invalid_address = "Ogiltig adress (%03x) vid %1x-%03x i %s rad: %d\n";
const char *h_err_invalid_register = "Ogilitigt register (REG[%03d]) vid %1x-%03x i %s rad: %d\n";
# else
const char *h_err_unexpected_opcode = "Ovaentad opcode (%04o) vid %1o-%04o i %s rad: %d\n";
const char *h_err_unexpected_error = "Ovaentat fel vid %1o-%04o i %s rad: %d\n";
const char *h_err_invalid_address = "Ogiltig adress (%04o) vid %1o-%04o i %s rad: %d\n";
const char *h_err_invalid_register = "Ogiltigt register (REG[%03d]) vid %1o-%04o i %s rad: %d\n";
#endif /* Hexadecimal */

const char *h_err_missing_argument = "option requires an argument -- '%s'\n";

#if defined(unix) || defined(__unix__) || defined(__APPLE__)
const char *h_msg_usage[] =
{
   "Anvaendning: %s [OPTION]... [FILE]\n",
   "En RPN kalkylator-emulator foer X11.\n\n",
   "  -b  ADDR                 definiera en brytpunkt (octal or hex)\n",
   "  -i  OPCODE               definiera en instruktions-brytpunkt (octal or hex)\n",
   "  -r  FILE                 ladda fraan en ROM FILE\n",
   "  -s                       enkelstega (single step)\n",
   "  -t                       logga (trace)\n",
#if defined(HP31e) || defined(HP32e) || defined(HP33e) || defined(HP33c) || defined(HP34c) || defined(HP37e) || defined(HP38e) || defined(HP38c)
   "  -c, --comma              anvaend komma som decimal-avgraensare\n",
#endif
   "      --cursor             visa markoer (cursor)\n",
   "      --no-cursor          doelj markoer (cursor)\n",
   "      --geometry +x+y      specify initial window position\n",  /** Translation needed */
   "      --numlock            use numeric keypad (even if numlock is off)\n", /** Translation needed */
#if defined(CONTINIOUS)
   "      --reset              do not restore saved state (factory reset)\n",  /** Translation needed */
#endif
   "      --zoom ZOOM          oeka fönster-storleken\n",
   "      --help               visa denna hjaelp och avsluta\n",
   "      --version            visa versions-information och avsluta\n\n",
   NULL
};
const char *h_err_invalid_operand = "Felaktig(a) operand(er)\n";
const char *h_err_invalid_option = "Felaktig option -- '%c'\n";
const char *h_err_duplicate_option = "Dubblerad option -- '%c'\n";
const char *h_err_unrecognised_option = "Obekant option '%s'\n";
const char *h_err_invalid_number = "Inte ett oktalt tal -- '%s'\n";
const char *h_err_numeric_range = "Felaktigt vaerde -- '%s'\n";
const char *h_err_max_breakpoints = "Maximalt antal brytpunkter oeverskridet\n";
const char *h_err_invalid_argument = "Foervaentat argument, inte -- '%c'\n";
#else
const char *h_msg_usage[] =
{
   "Anvaendning: %s [OPTION...] [FILE]\n",
   "En RPN kalkylator-emulator foer X11.\n\n",
   "  /cursor                  visa markoer (default)\n",
   "  /nocursor                doelj markoer\n",
   "  /step                    enkelstega (single step)\n",
   "  /trace                   logga (trace)\n",
   "  /version                 visa denna hjaelp och avsluta\n",
   "  /?, /help                visa versions-information och avsluta\n\n",
   NULL
};
const char *h_err_invalid_operand = "Felaktig(a) parameter(ar)\n";
const char *h_err_invalid_option = "Felaktig option %s\n";
#endif /* Unix */


#else /* Language defaults to English */

const char *h_msg_loading = "Loading '%s'\n";
const char *h_msg_saving = "Saving '%s'\n";

const char *h_err_register_alloc = "Run-time error\t: %s line : %d : Memory allocation failed!\n";
const char *h_err_opening_file = "Unable to open '%s'\n";
const char *h_err_reading_file = "Error reading file\n";
const char *h_err_writing_file = "Error writing to file\n";

const char *h_err_display = "Cannot connect to X server '%s'\n";
const char *h_err_display_properties = "Unable to get display properties\n";
const char *h_err_display_colour = "Requires a %d-bit colour display\n";
const char *h_err_pixmap = "Can't create pixmap\n";
const char *h_err_font = "Cannot load font '%s' (x11 base bitmap fonts required)\n";

#if defined(HEXADECIMAL)
const char *h_err_unexpected_opcode = "Unexpected opcode (%03x) at %1x-%03x in %s line : %d\n";
const char *h_err_unexpected_error = "Unexpected error at %1x-%03x in %s line : %d\n";
const char *h_err_invalid_address = "Address (%03x) out of range at %1x-%03x in %s line : %d\n";
const char *h_err_invalid_register = "Invalid register (REG[%03d]) at %1x-%03x in %s line : %d\n";
# else
const char *h_err_unexpected_opcode = "Unexpected opcode (%04o) at %1o-%04o in %s line : %d\n";
const char *h_err_unexpected_error = "Unexpected error at %1o-%04o in %s line : %d\n";
const char *h_err_invalid_address = "Address (%04o) out of range at %1o-%04o in %s line : %d\n";
const char *h_err_invalid_register = "Invalid register (REG[%03d]) at %1o-%04o in %s line : %d\n";
#endif /* Hexadecimal */

const char *h_err_missing_argument = "option requires an argument -- '%s'\n";

#if defined(unix) || defined(__unix__) || defined(__APPLE__)
const char *h_msg_usage[] =
{
   "Usage: %s [OPTION]... [FILE]\n",
   "An RPN calculator emulator for X11.\n",
   "\n",
   "  -b  ADDR                 breakpoint address (octal or hex)\n",
   "  -i  OPCODE               trap instruction (octal or hex)\n",
   "  -r  FILE                 load ROM contents from FILE\n",
   "  -s                       single step\n",
   "  -t                       trace execution\n",
#if defined(HP31e) || defined(HP32e) || defined(HP33e) || defined(HP33c) || defined(HP34c) || defined(HP37e) || defined(HP38e) || defined(HP38c)
   "  -c, --comma              use comma as decimal separator\n",
#endif
   "      --cursor             show cursor (default)\n",
   "      --no-cursor          hide cursor\n",
   "      --geometry +x+y      specify initial window position\n",
   "      --numlock            use numeric keypad (even if numlock is off)\n",
#if defined(CONTINIOUS)
   "      --reset              do not restore saved state (factory reset)\n",
#endif
   "      --zoom ZOOM          enlarge window size\n",
   "      --help               show this help and exit\n",
   "      --version            show version and exit\n\n",
   NULL
};
const char *h_err_invalid_operand = "invalid operand(s)\n";
const char *h_err_invalid_option = "invalid option -- '%c'\n";
const char *h_err_duplicate_option = "duplicate option -- '%c'\n";
const char *h_err_unrecognised_option = "unrecognised option '%s'\n";
const char *h_err_invalid_number = "not an octal number -- '%s'\n";
const char *h_err_numeric_range = "out of range -- '%s'\n";
const char *h_err_max_breakpoints = "maximum number of breakpoints exceeded\n";
const char *h_err_invalid_argument = "expected argument not -- '%c'\n";
#else
const char *h_msg_usage[] =
{
   "Usage: %s [OPTION]... [FILE]\n",
   "An RPN calculator emulator for X11.\n\n",
   "  /cursor                  show cursor (default)\n",
   "  /nocursor                hide cursor\n",
   "  /step                    single step\n",
   "  /trace                   trace execution\n",
   "  /version                 show version and exit\n\n",
   "  /?, /help                show this help and exit\n",
   NULL
};
const char *h_err_invalid_operand = "invalid parameter(s)\n";
const char *h_err_invalid_option = "invalid option %s\n";
#endif /* Unix */

#endif /* Language */

void v_error(int i_errno, const char *s_format, ...)  /* Print formatted error message and exit returning errno */
{
   va_list t_args;
   if (!(i_errno)) i_errno = -1;  /* If errno not set return -1 */
   va_start(t_args, s_format);
   fprintf(stderr, "%s: ", FILENAME);
   vfprintf(stderr, s_format, t_args);
   va_end(t_args);
   exit(i_errno);
}

void v_warning(const char *s_format, ...)  /* Print formatted warning message */
{
   va_list t_args;
   va_start(t_args, s_format);
   fprintf(stderr, "%s: ", FILENAME);
   vfprintf(stderr, s_format, t_args);
   va_end(t_args);
}

void v_usage(FILE *h_file, const char **s_text, ...)  /* Print multiple lines of text */
{
    va_list t_args;
    int i_count;

    va_start(t_args, s_text);
    for (i_count = 0; s_text[i_count] != NULL; i_count++)
    {
        if (i_count == 0)
            vfprintf(h_file, s_text[i_count], t_args);  /* Display first line using arguments */
        else
            fprintf(h_file, "%s", s_text[i_count]);  /* Display remainder of the text */
    }
    va_end(t_args);
}

