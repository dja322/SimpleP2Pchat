#ifndef STRING_UTILS_H
#define STRING_UTILS_H

#define SUBSTRING_NOT_FOUND -1
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>


/*
Name: compareStrings
Process: compares text quantities (c-style strings) without case sensitivity
         with the following outcomes:
         - for equal sized strings, if the first parameter is alphabetically
           greater than the second parameter, returns positive number
         - for equal sized strings, if the first parameter is alphabetically
           less than the second parameter, returns negative number
         - for equal sized strings, if the first parameter is alphabetically
           equal to the second parameter, returns zero
         - for unequal sized strings with common letters up to the length
           of the shortest string, returns difference in the length
           of the strings
Function input/parameters: strings to be compared(const char *)
Function output/parameters: none
Function output/returned: processed result as specified (int)
Device input/---: none
Device output/---: none
Dependencies: getStringLength, toLowerCaseLetter
*/
int compareStrings( const char *leftStr, const char *rightStr );

/*
Name: copyString
Process: copies characters from source string to destination string
         up to NULL_CHAR
Function input/parameters: source string (const char *)
Function output/parameters: destination string (char *)
Function output/returned: none
Device input/---: none
Device output/---: none
Dependencies: none
*/
void copyString( char *destStr, const char *srcStr );

/*
Name: getStringLength
Process: finds the number of characters in (length of) a c-style string
Function input/parameters: string to be processed (const char *)
Function output/parameters: none
Function output/returned: length of string (int)
Device input/---: none
Device output/---: none
Dependencies: none
*/
int getStringLength( const char *str );
/*
Name: printSpaces
Process: recursively prints the given number of spaces to the screen
Function input/parameters: number of spaces to print (int)
Function output/parameters: none
Function output/returned: none
Device input/---: none
Device output/monitor: spaces printed as specified
Dependencies: none
*/
void printSpaces( int numSpaces );

/*
Name: toLowerCaseLetter
Process: if character is upper case letter, 
         returns associated lower case letter,
         otherwise has no effect
Function input/parameters: character to be processed (char)
Function output/parameters: none
Function output/returned: character processed as specified (char)
Device input/---: none
Device output/---: none
Dependencies: none
*/
char toLowerCaseLetter( char testChar );

/*
Name: concatenateString
Process: concatinate two strings into one
Function input/parameters: destination string, source string
Function output/parameters: concatenated String to destStr
Function output/returned: 
Device input/---: none
Device output/---: none
Dependencies: none
*/
void concatenateString(char *destStr, const char* sourceStr);

/*
Name: findSubString
Process: finds index of substring(if it exists) in testStr
Function input/parameters: Source String and substring
Function output/parameters: none
Function output/returned: index of substring
Device input/---: none
Device output/---: none
Dependencies: none
*/
int findSubString(const char* testStr, const char* substring);

/*
Name: getStringConstrained
Process: get string from a file with certain parameters set
Function input/parameters: File and options
Function output/parameters: captured string if it was successful
Function output/returned: bool for success or failure
Device input/---: none
Device output/---: none
Dependencies: none
*/
bool getStringConstrained(
   FILE *inStream,
   bool clearLeadingNonPrintable,
   bool clearLeadingSpace,
   bool stopAtNonPrintable,
   char delimiter,
   char* capturedString
);

/*
Name: getStringToDelimiter
Process: get string to a certain deliminated character
Function input/parameters: File and options
Function output/parameters: captured string if it was successful
Function output/returned: bool for success or failure
Device input/---: none
Device output/---: none
Dependencies: getStringConstrained
*/
bool getStringToDelimiter (
   FILE* inStream,
   char delimiter,
   char* capturedString
   );

/*
Name: getStringToLineEnd
Process: get string from a file to end of line
Function input/parameters: File
Function output/parameters: captured string if it was successful
Function output/returned: bool for success or failure
Device input/---: none
Device output/---: none
Dependencies: none
*/
bool getStringToLineEnd(
   FILE* inStream,
   char *capturedString
);

/*
Name: getSubString
Process: Finds a substring
Function input/parameters: dest string, source string, start and end index
Function output/parameters: destStr with substring
Function output/returned: none
Device input/---: none
Device output/---: none
Dependencies: 
*/
void getSubString(
   char* destStr, const char *sourceStr,
   int startIndex, int endIndex
);


/*
Name: setStrToLowerCase
Process: using toLowerCase loops through string lower casing whole string
Function input/parameters: dest string, source string
Function output/parameters: lowercase string
Function output/returned: none
Device input/---: none
Device output/---: none
Dependencies: toLowerCase
*/
void setStrToLowerCase(char* destStr, const char* sourceStr);

/*
Name: toLowerCase
Process: finds index of substring(if it exists) in testStr
Function input/parameters: character to be lowered
Function output/parameters: none
Function output/returned: lowercase char
Device input/---: none
Device output/---: none
Dependencies: none
*/
char toLowerCase(char testChar);


#endif