/*============================================================================
FILE  pp_mod.c

MEMBER OF process cmpp

Copyright 1991
Georgia Tech Research Corporation
Atlanta, Georgia 30332
All Rights Reserved

PROJECT A-8503

AUTHORS

    9/12/91  Steve Tynor

MODIFICATIONS

    <date> <person name> <nature of modifications>

SUMMARY

    This file contains the top-level driver function for preprocessing the
    "cfunc.mod" file.  First, the "ifspec.ifs" file is opened and parsed to
    get the data that will be needed in the .mod to .c translation  (See
    read_ifs.c).  Then the .mod file is translated.  Most of the work of the
    translation is handled by the UNIX 'lex' and 'yacc' utilities.  This
    translation is begun at the call to mod_yyparse() below.  See also files:

        mod_lex.l
        mod_yacc.y

    Note that to allow lex/yacc to be used twice (once for the ifspec.ifs
    file, and then again for the cfunc.mod file), the functions created by
    lex/yacc for the latter are translated using the UNIX text editor 'sed'
    under the direction of the Makefile and the following 'sed scripts':

        mod_lex.sed
        mod_yacc.sed

    Hence the call to 'mod_yyparse()' rather than 'yyparse()' below.

INTERFACES

    preprocess_mod_file()

REFERENCED FILES

    None.

NON-STANDARD FEATURES

    None.

============================================================================*/

#include  "cmpp.h"

/*---------------------------------------------------------------------------*/
static void change_extension (char *filename, char *ext, char *new_filename)
{
   int i = strlen (filename);
   
   strcpy (new_filename, filename);

   for (; i >= 0; i--) {
      if (new_filename[i] == '.') {
	 new_filename[i+1] = '\0';
	 break;
      }
   }
   strcat (new_filename, ext);
}

/*---------------------------------------------------------------------------*/

/*
preprocess_mod_file

Function preprocess_mod_file is the top-level driver function for
preprocessing a code model file (cfunc.mod).  This function calls
read_ifs_file() requesting it to read and parse the Interface
Specification file (ifspec.ifs) and place the information
contained in it into an internal data structure.  It then calls
mod_yyparse() to read the cfunc.mod file and translate it
according to the Interface Specification information.  Function
mod_yyparse() is automatically generated by UNIX lex/yacc
utilities.
*/


void preprocess_mod_file (
    char *filename)         /* The file to read */
{
   extern FILE *mod_yyin;
   extern FILE *mod_yyout;
   extern char *current_filename;
   extern int mod_yylineno;
   extern int mod_num_errors;
   extern Ifs_Table_t     *mod_ifs_table;
   
   Ifs_Table_t     ifs_table;   /* info read from ifspec.ifs file */
   Status_t        status;      /* Return status */
   char		   error_str[200];
   char		   output_filename[200];
   
   /*
    * Read the entire ifspec.ifs file and load the data into ifs_table
    */
   
   status = read_ifs_file (IFSPEC_FILENAME, GET_IFS_TABLE, &ifs_table);
   
   if (status != OK) {
      exit(1);
   }
   
   mod_yyin = fopen (filename, "r");
   if (mod_yyin == NULL) {
      sprintf(error_str, "ERROR - Could not open input .mod file: %s",
	      filename);
      print_error(error_str);
      return;
   }
   
   current_filename = filename;

   change_extension (filename, "c", output_filename);
   mod_yyout = fopen (output_filename, "w");

   if (mod_yyout == NULL) {
      sprintf(error_str, "ERROR - Could not open output .c: %s",
	      output_filename);
      print_error(error_str);
      return;
   }
   
   mod_ifs_table = &ifs_table;
   mod_num_errors = 0;

   fprintf (mod_yyout, "#line 1 \"%s\"\n", filename);
   fprintf (mod_yyout, "#include \"cm.h\"\n");
   fprintf (mod_yyout, "#line 1 \"%s\"\n", filename);

   mod_yylineno = 1;
   if (!mod_yyin) {
      sprintf (error_str, "Could not open .mod file: \"%s\"", filename);
      print_error (error_str);
      unlink (output_filename);
      exit(1);
   }
   if (!mod_yyout) {
      sprintf (error_str, "Could not create .c file: \"%s\"",
	       output_filename);
      print_error (error_str);
      unlink (output_filename);
      exit(1);
   }

   if (mod_yyparse() || (mod_num_errors > 0)) {
      sprintf (error_str, "Error parsing .mod file: \"%s\"", filename);
      print_error (error_str);
      unlink (output_filename);
      exit (1);
   }
   fclose (mod_yyout);
   mod_yyrestart(NULL);
}

/*---------------------------------------------------------------------------*/
int mod_yyerror (str)
     char *str;
{
   extern int mod_yylineno;
   extern char *mod_yytext;
   extern char *current_filename;
   extern char *prog_name;
   
   fprintf (stderr, "%s: Error: \"%s\": line %d (near \'%s\'):\n\t%s.\n",
            prog_name, current_filename, mod_yylineno, mod_yytext, str);
}

