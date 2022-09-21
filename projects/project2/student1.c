#include <stdio.h>
#include <string.h>
#include "document.h"

int main() {
   Document doc;
   const char *doc_name = "Shiv Test";

   init_document(&doc, doc_name);

   /* Adding paragraph */
   add_paragraph_after(&doc, 0);
   add_line_after(&doc, 1, 0, "First Par, First Line");
   add_line_after(&doc, 1, 1, "First Par, Second Line");
   add_line_after(&doc, 1, 2, "First Par, Third Line");
   add_line_after(&doc, 1, 3, "First Par, Fourth Line");
   add_line_after(&doc, 1, 4, "First Par, Fifth Line");
   print_document(&doc);

   /* test add+remove 
   printf("Removing First Line...\n");
   remove_line(&doc, 1, 1);
   print_document(&doc);

   printf("Removing Fifth (Now the fourth) Line...\n");
   remove_line(&doc, 1, 4);
   print_document(&doc);

   printf("Inserting new paragraph before...\n");
   add_paragraph_after(&doc, 0);
   add_line_after(&doc, 1, 0, "New First Par, First Line");
   print_document(&doc);

   printf("Removing first line from new par...\n");
   remove_line(&doc, 1, 1);
   print_document(&doc); */

   /* rplace one letter 
   replace_text(&doc, "F", "Shiv");
   print_document(&doc); */

   /*replace multiple letters 
   replace_text(&doc, "First", "Aye");
   print_document(&doc); */

   /*replace multiple letters with one
   replace_text(&doc, "First", "S");
   print_document(&doc);  */

   /*replace multiple letters with a space inside 
   replace_text(&doc, "First Par", "F");
   print_document(&doc); */

   /*replace letter for letter 
   replace_text(&doc, "F", "F");
   print_document(&doc); */

   /*replace word for word 
   replace_text(&doc, "First", "First");
   print_document(&doc); */
   
   /*remove one letter 
   remove_text(&doc, "F");
   print_document(&doc); */

   /*remove multiple letters 
   remove_text(&doc, "First");
   print_document(&doc); */

   /*remove multiple letters with a space 
   remove_text(&doc, "First Par, ");
   print_document(&doc); */

   /*highlight one letter 
   highlight_text(&doc, "F");
   print_document(&doc);  */

   /*highlight multiple letters 
   highlight_text(&doc, "First");
   print_document(&doc); */

   /* highlight multiple letters with a space 
   highlight_text(&doc, "First Par, ");
   print_document(&doc); */

   /* highlight multiple letters alrady surrounded by a brcket (let previous one run)
   highlight_text(&doc, "[First Par, ]");
   print_document(&doc); */

   /* highlight one letter already surrounded by a bracket (let previous run) 
   highlight_text(&doc, "[F]");
   print_document(&doc); */

   return 0;
}
