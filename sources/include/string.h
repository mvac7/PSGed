#ifndef __STRING_H__
#define __STRING_H__

#include "newTypes.h"


/* =============================================================================
Description : 
Input    : string (string)
Output   : length (byte)
============================================================================= */
byte strlen(char* text);


/* =============================================================================
strcpy                                
Description : Copies the C string pointed by source into the array pointed by 
              destination, including the terminating null character. 
Input    : destination (string)
           source (string)
Output   : -
Author   : -
============================================================================= */
void strcpy(char* destination, char* source);


/* =============================================================================
strncpy                                
Description : 
Input    : destination (string)
           source (string)
           length (byte)
Output   : -
Author   : -
============================================================================= */
void strncpy(char* destination, char* source, byte length);


/* =============================================================================
strcat                                
Description : Appends a copy of the source string to the destination string. 
Input    : destination (string)
           source (string)
Output   : -
Author   : -
============================================================================= */
void strcat(char* destination, char* source);



/* =============================================================================
strncat                                
Description : 
Input    : destination (string)
           source (string)
           length (byte)
Output   : -
Author   : -
============================================================================= */
void strncat(char* destination, char* source, byte length);


 



#endif