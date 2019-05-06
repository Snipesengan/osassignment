#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "spellconf.h"
#include "LinkedList.h"
#include "Functions.h"
#include "FileIO.h"

/************************************************************************
File: FileIO.c
Author: Cameron Kerr
References: https://en.wikipedia.org/wiki/Longest_word_in_English
Purpose: functions for reading the spellconf file and the dictionary file
	also for writing the ammened words to a file
Comments:
*************************************************************************/

int getNumLines(char* fileName)
{
	char* str = (char*)malloc(256 * sizeof(char)); /*initializing so it is not null;*/
	int numLines = 0;
	FILE* f = fopen(fileName, "r"); /*set file to write*/


	if(f != NULL)
	{
		/*store line as str*/
		str = fgets(str, 256, f);
		while(str != NULL) /*when the str is null it means the file has reached the end*/
		{
			numLines++; /*increment numLines for each line*/
			str = fgets(str, 256, f);
		}

		if(ferror(f))
		{
			perror("Error reading file");
		}
		fclose(f);
	}
	else
	{
		perror("error opening file");
	}

	free(str);
	return numLines;
}

/************************************************************************
Function: getNumLines
Imports: a file name(char*), pointer to a settings struct, number of lines(int)
Exports: none
Purpose: Counts the number of lines in the file
*************************************************************************/
Settings* readSpellConf(char* fileName, Settings* settings, int numLines)
{
	char settingStr[256];
	char valueStr[256];
	char* error; /*str that will be set to a certain error message and printed for that error*/

	char* dictFileName;
	int maxdifference;
	int autocorrect;
	int dictLength;

	int dictRead = 0; /*boolean to keep track if a dictName has been read*/
	int maxDifRead = 0; /*boolean to keep track if a maxDiff value has been read*/
	int autoCorRead = 0; /*boolean to keep track if a autoCorRead value has been read*/
	int failed = 0; /*boolean that is set to true when an error occurs */

	int numElements; /*how many elements successful when using fscanf*/
	int lineNum = 0; /*keep track of number of lines*/

	FILE* f; /*open the file*/
	f = fopen(fileName, "r"); /*set to read*/

	if(f != NULL)
	{
		while(failed == 0 && lineNum < numLines)
		{
			lineNum++;
			numElements = fscanf(f, "%s = %s\n", settingStr, valueStr);

			if(numElements == 2) /*if fscanf was successful*/
			{
				if(strcmp(settingStr, "dictfile") == 0 && dictRead == 0) /*check if first word is dictfile and if the dictionary file is read*/
				{
					dictLength = strlen(valueStr); /*gets the size of the dictionary filename string*/
					dictFileName = (char*)malloc(dictLength * sizeof(char)); /*allocate the memory */
					strcpy(dictFileName, valueStr);
					dictRead = 1; /*set dictionary file as read*/
				}
				else if(strcmp(settingStr, "maxdifference") == 0 && maxDifRead == 0) /*check if first word is maxdifference and if the dictionary file is read*/
				{
					maxdifference = atoi(valueStr); /*parse the value to an integer*/
					if(maxdifference <= 0) /*invalid if the maxdifference is 0 or below*/
					{
						error = "invalid value for maxdifference";
						failed = 1;
					}
					else
					{
						maxDifRead = 1;
					}

				}
				else if(strcmp(settingStr, "autocorrect") == 0 && autoCorRead == 0) /*check if first word is autocorrect and if the dictionary file is read*/
				{
					if(strcmp(valueStr, "yes") == 0) /*value must be yes or no otherwise reading file fails*/
					{
						autocorrect = 1;
						autoCorRead = 1;
					}
					else if(strcmp(valueStr, "no") == 0)
					{
						autocorrect = 0;
						autoCorRead = 1;
					}
					else
					{
						failed = 1;
						error = "Invalid value for autocorrect";
					}
				}
				else
				{
					/*none of the above if statements are true*/
					failed = 1;

					/*check if it failed because the same type of file read in too many times*/
					if(dictRead == 1 && maxDifRead == 1 && autoCorRead == 1 && numElements == 2)
					{
						error = "Multiple settings, double occurs";
					}
					/*if not, can conclude that failure occured because of a line with an incorrect setting*/
					else
					{
						error = "Invalid setting";
					}
				}

			}
			else
			{
				error = "Invalid structure";
				failed = 1;
			}

		}

		if(ferror(f))
		{
			perror("Error reading file.");
		}
		fclose(f);
	}
	else
	{
		perror("Error reading file.");
	}

	if(dictRead == 1 && maxDifRead == 1 && autoCorRead == 1 && failed == 0)
	{
		/*upon successful read, add the values stored to a settings struct*/
		strcpy(settings -> dictFile, dictFileName);
		settings -> maxDiff = maxdifference;
		settings -> autoCorrect = autocorrect;
	}
	else
	{
		/*if a fail occurs from the file structure print the specific error and the line it occured at
			the settings gets set to NULL so that the main function can test if file was read successfuly
			by checking for NULL*/
		printf("%s at line %d\n",error,lineNum);
		free(dictFileName);
		free(settings);
		settings = NULL;
	}

	return settings;
}

/************************************************************************
 * Function: readWordFile
 * Imports: a file name(char*), pointer to a number to store number of words read (int*)
 * Exports: pointer to a linked list (LinkedList*)
 * Purpose: Stores every word in a file into a linked list
 * Comments: This is used for reading both the user file and dictionary file
 * *************************************************************************/
LinkedList* readWordFile(char* fileName, int* numWordsPtr)
{
	char currWord[46]; /*size limited to 45 as that is the amount of letters of longest word in
						a major dictionary: https://en.wikipedia.org/wiki/Longest_word_in_English*/
	char* word;
	char chr; /*The currect character being read from the file*/
	int numWords = 0; /*Number of words read from file*/
	int i = 0; /*used to track the number char on a particular word for indexing*/

	LinkedList* list = createLinkedList();

	FILE* f;
	f = fopen(fileName, "r");

	if(f != NULL)
	{
		chr = fgetc(f);
		while(chr != EOF)
		{
			/*checks if the curr char is a letter or apostrophe*/
			if((chr >= 'a' && chr <= 'z') || (chr >= 'A' && chr <= 'Z') || chr == 39 /*apostophe*/)
			{
				/*If the character read is uppercase, convert it to lowercase*/
				if(chr >= 'A' && chr <= 'Z')
				{
 					convToLower(&chr);
				}

				/*adds the char to the array chars*/
				currWord[i] = chr;
				i++; /*increment i so that the rest of the current word will be added to the currWord array */
			}
			else if(i > 0)
			{
				/*this block of code allocates memory of the size of the word just made
					then it copies the word to the allocated space, it then inserts this
					allocated memory into the linked list*/
				currWord[i] = '\0';
				word = (char*)malloc((i + 1) * sizeof(char));
				strcpy(word, currWord);
				insertFirst(list, word);
				numWords++; /*numWords incremented because a word has just been added*/
				i = 0; /*i reset to 0 for next word to be read in*/
			}
			
			chr = fgetc(f); /*go to next char in file*/
		}

		if(i > 0) /*adds the last word from the file to the list after reaching EOF*/ 
		{
			currWord[i] = '\0';
			word = (char*)malloc((i + 1) * sizeof(char));
			strcpy(word, currWord);
			insertFirst(list, word);
			numWords++;
		}

		if(ferror(f))
		{
			perror("Error reading file");
		}
		fclose(f);
	}
	else
	{
		perror("error opening file");
	}

	*numWordsPtr = numWords; /*puts the number of words into the numWords pointer*/
	return list;
}

/************************************************************************
 * Function: writeArrayToFile
 * Imports: array of strings that have been checked and corrected(char* arr[]),
 * 	    length of array(int), name of file to write to(char*)
 * Exports: none
 * Purpose: Writes all the words from an array to the file
 * *************************************************************************/
void writeArrayToFile(char* array[], int arrayLength, char* fileName)
{
	FILE* f = fopen(fileName,"w");/*set filepointer to write*/
	int i = 0; /*used to incremenet through array in for loop*/

	if(f != NULL)
	{
		for(i = arrayLength - 1; i >= 0; i--) /*decrement from last index because array has the elements stored backwards */
		{
			fprintf(f, "%s\n", array[i]); /*print each string to a line*/
		}

		if(ferror(f))
		{
			perror("Error writing to file");
		}
		fclose(f);
	}
	else
	{
		perror("error opening file");
	}
}
