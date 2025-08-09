#include "StringUtils.h"
#include "StandardConstants.h"


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
int compareStrings( const char *leftStr, const char *rightStr )
   {
    int diff, index = 0;
    int leftStrLen = getStringLength( leftStr );
    int rightStrLen = getStringLength( rightStr );

    while( index < leftStrLen && index < rightStrLen )
       {
        diff = toLowerCaseLetter( leftStr[ index ] ) 
                                       - toLowerCaseLetter( rightStr[ index ] );

        if( diff != 0 )
           {
            return diff;
           }

        index = index + 1;
       }

    return leftStrLen - rightStrLen;
   }

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
void copyString( char *destStr, const char *srcStr )
   {
      //initialize variables
      int index = 0;

      //check if source/dest are not the same(aliasing)
      if (destStr != srcStr)
      {
         while( srcStr[ index ] != NULL_CHAR )
            {
            destStr[ index ] = srcStr[ index ];

            index++;

            destStr[ index ] = NULL_CHAR;
            }
      }
   }

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
int getStringLength( const char *str )
   {
    int index = 0;

    //loop to end of string, preven overflow
    while( index < STD_STR_LEN && str[ index ] != NULL_CHAR )
       {
        index = index + 1;
       }

    return index;
   }

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
void printSpaces( int numSpaces )
   {
    if( numSpaces > 0 )
       {
        printf( "%c", SPACE );

        printSpaces( numSpaces - 1 );
       }
   }

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
char toLowerCaseLetter( char testChar )
   {
    if( testChar >= 'A' && testChar <= 'Z' )
       {
        return testChar - 'A' + 'a';
       }

    return testChar;
   }

/*
Name: concatenateString
Process: concatinate two strings into one
Function input/parameters: destination string, source string
Function output/parameters: concatenated String to destStr
Function output/returned: 
Device input/---: none
Device output/---: none
Dependencies: copyString, getStringLength, malloc, free
*/
void concatenateStrong(char *destStr, const char* sourceStr)
{

   //get destination index
      //function: getStringLength
   int destIndex = getStringLength(destStr);

   //get source string length
      //function: getStringLength
   int sourceStrLen = getStringLength(sourceStr);

   //create temporary string pointer
   char* tempStr;

   //create other variables
   int sourceIndex = 0;

   //copy source string in case of aliasing
      //function: malloc, copystring
   tempStr = (char*)malloc(sizeof(sourceStrLen+1));
   copyString(tempStr, sourceStr);

   //loop to end of source string
   while (tempStr[sourceIndex] != NULL_CHAR && destIndex < MAX_STR_LEN)
   {
      //assign characters to end of desitnation string
      destStr[destIndex] = tempStr[sourceIndex];

      //update indices
      destIndex++; sourceIndex++;

      //set temporary end of destination
      destStr[destIndex] = NULL_CHAR;
   }
   //end loop

   //release memory used for temp string
      //function: free
   free(tempStr);
}

/*
Name: findSubString
Process: finds index of substring(if it exists) in testStr
Function input/parameters: Source String and substring
Function output/parameters: none
Function output/returned: index of substring
Device input/---: none
Device output/---: none
Dependencies: getStringLength
*/
int findSubString(const char* testStr, const char* substring)
{
   //initialize varibles

   //initialize test string length
   int testStrLen = getStringLength(testStr);

   //initialize master index
   int masterIndex = 0;

   //initialize other variables
   int searchIndex, internalIndex;

   while (masterIndex < testStrLen)
   {
      //set internal loop index to current test string index
      internalIndex = masterIndex;

      //set internal search index to zero
      searchIndex = 0;

      //loop to end of test string while test and substr are same
      while (internalIndex <= testStrLen
             && testStr[internalIndex] == substring[searchIndex])
      {
         //increment test string, substring indices
         internalIndex++;
         searchIndex++;

         //check for end of substring
         if (substring[searchIndex] == NULL_CHAR)
         {
            //return beginning location of sub string
            return masterIndex;
         }
      }
      //end internal comparison loop
      //increment current beginning location index
      masterIndex++;
   }
   //assume substring is not found and test failed, return SUBSTRING_NOT_FOUND
   return SUBSTRING_NOT_FOUND;
}

/*
Name: getStringConstrained
Process: get string from a file with certain parameters set
Function input/parameters: File and options
Function output/parameters: captured string if it was successful
Function output/returned: bool for success or failure
Device input/---: none
Device output/---: none
Dependencies: fgetc
*/
bool getStringConstrained(
   FILE *inStream,
   bool clearLeadingNonPrintable,
   bool clearLeadingSpace,
   bool stopAtNonPrintable,
   char delimiter,
   char* capturedString
)
{
   //initialize variables
   int intChar = EOF;
   int index = 0;

   //initialize output string
   capturedString[index] = NULL_CHAR;

   //capture first value in stream
   intChar = fgetc(inStream);

   while (intChar != EOF 
   && ((clearLeadingNonPrintable && intChar < (int) SPACE) 
   || (clearLeadingSpace && intChar == (int) SPACE) ))
   {
      //get next character
         //function: fgetc
      intChar = fgetc(inStream);
   }
   //end clear nonprintable/space loop

   //check for end of file found
   if (intChar == EOF)
   {
      //return failed Operation
      return false;
   }

   //loop to capture input
   while (
      //continue if not at end of file and MAX_STR_LEN not reached
      (intChar != EOF && index < MAX_STR_LEN-1)
      //and cotninues if not printable flag set and characters are printable
      && ((stopAtNonPrintable && intChar >= (int)SPACE)
         || (!stopAtNonPrintable))
      //and
      //continue if specified delimiter is not found
      && (intChar != (char)delimiter)
      )
   {
      //place character in array element
      capturedString[index] = (char) intChar;

      //increment array index
      index++;

      //set next element to null character / end of c-string
      capturedString[index] = NULL_CHAR;

      //get Next character as interger
         //function: fgetc
      intChar = fgetc(inStream);
   }
   //end loop

   //return success
   return true;
   
}

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
   )
{
   return getStringConstrained(
      inStream,       //file stream pointer
      true,           //clears leading non printable character
      true,           //bool clearLeadingSpace
      true,           //Stops at non printable
      delimiter,      //Stops at delimiter
      capturedString  //return string
   );
}

/*
Name: getStringToLineEnd
Process: get string from a file to end of line
Function input/parameters: File
Function output/parameters: captured string if it was successful
Function output/returned: bool for success or failure
Device input/---: none
Device output/---: none
Dependencies: getStringConstrained
*/
bool getStringToLineEnd(
   FILE* inStream,
   char *capturedString
)
{
   return getStringConstrained(
      inStream,       //file stream pointer
      true,           //clears leading non printable character
      true,           //bool clearLeadingSpace
      true,           //Stops at non printable
      NULL_CHAR,      //Stops at delimiter
      capturedString  //return string
   );
}

/*
Name: getSubString
Process: Finds a substring
Function input/parameters: dest string, source string, start and end index
Function output/parameters: destStr with substring
Function output/returned: none
Device input/---: none
Device output/---: none
Dependencies: getStringLength, copyString, malloc, free
*/
void getSubString(
   char* destStr, const char *sourceStr,
   int startIndex, int endIndex
)
{
   //initialize variables
      //set length of source string
      //function: getStringLength
      int sourceStrLen = getStringLength(sourceStr);

   //initialize destionation index to zero
   int destIndex = 0;

   //initialize source index to start index (parameter)
   int sourceIndex = startIndex;

   //create pointer for temp string
   char *tempStr;

   //check for indices within limits
   if (startIndex >= 0 && startIndex <= endIndex
       && endIndex < sourceStrLen)
   {
      //create temporary string
         //function: malloc, copyString
      tempStr = (char*)malloc(sourceStrLen+1);
      copyString(tempStr, sourceStr);

      //loop across requested  substring
      while (sourceIndex <= endIndex)
      {
         //assign source character to destination element
         destStr[destIndex] = tempStr[sourceIndex];

         //increment indices
         destIndex++;
         sourceIndex++;

         //set temporary end of destination string
         destStr[destIndex] = NULL_CHAR;
      }
      //end loop

      //return memory for temporary string
         //function: free
      free(tempStr);
   }
}

/*
Name: setStrToLowerCase
Process: using toLowerCase loops through string lower casing whole string
Function input/parameters: dest string, source string
Function output/parameters: lowercase string
Function output/returned: none
Device input/---: none
Device output/---: none
Dependencies: getStringLength, malloc, copyString, toLowerCase, free
*/
void setStrToLowerCase(char* destStr, const char* sourceStr)
{
   //initialize variables

   //get source string length
      //functions: getString length
   int sourceStrLen = getStringLength(sourceStr);

   //create temporary string pointer
   char* tempStr;

   //create other variables
   int index = 0;

   //copy source string in case of aliasing
      //function malloc, copystring
   tempStr = (char*)malloc(sizeof(sourceStrLen+1));
   copyString(tempStr, sourceStr);

   //loop across source string
   while (tempStr[index] != NULL_CHAR && index < MAX_STR_LEN)
   {
      //set individual characters to lower case as needed
      //assign to destination string
      destStr[index] = toLowerCase(tempStr[index]);

      //updare index
      index++;

      //set temporary end of destination string
      destStr[index] = NULL_CHAR;
   }
   //end loop

   //release memory used for temp string
      //function: free
   free(tempStr);
}

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
char toLowerCase(char testChar)
{
   //check for upper case letter
   if (testChar >= 'A' && testChar <= 'Z')
   {
      return testChar - 'A' + 'a'; 
   }

   //otherwise assume not uppercase and return unchanged char
   return testChar;
}

