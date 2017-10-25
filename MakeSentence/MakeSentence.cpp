#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#else
#include <stdlib.h>
#endif // _DEBUG
#include <stdio.h>
#include <conio.h>
#include <assert.h>

struct linkedList
{
	char word[256];
	linkedList *next;
	linkedList *prev;
};

char *MakeSentence(const char **words, int numWords);

void main(int i_argc, char ** i_argl)
{
	linkedList *firstWord = (linkedList *) malloc(sizeof(linkedList));
	linkedList *lastWord = firstWord;
	linkedList *curWord = firstWord;

	int wNum = 0;

	printf("Please enter the words you want in a sentence.\nPress Enter to end a word. Press Enter without typing a word to end the sentence.\n\n");

	do
	{
		gets_s(lastWord->word, 256);

		curWord = lastWord;
		if (curWord->word[0] != '\0')
		{
			lastWord->next = (linkedList *)malloc(sizeof(linkedList));
			lastWord = lastWord->next;
			lastWord->prev = curWord;
		}
		wNum++;
	} while (curWord->word[0] != '\0');

	if (wNum == 1)
	{
		printf("You input nothing.\n");
		return;
	}

	const char ** strings = (const char **)malloc(sizeof(char*) * wNum);
	strings[wNum - 1] = NULL;
	lastWord = lastWord->prev;
	free(lastWord->next);

	curWord = lastWord;
	for (int i = wNum - 2; i > -1; i--)
	{
		strings[i] = curWord->word;
		if (i > 0)
		{
			curWord = curWord->prev;
		}
	}

	assert(strings != NULL);
	char * pSentence = MakeSentence(strings, wNum);

	free(strings);
	for (int i = 0; i < wNum - 1; i++)
	{
		if (i < wNum - 2)
		{
			lastWord = lastWord->prev;
			free(lastWord->next);
		}
		else
			free(firstWord);
	}

	printf("The Sentence is: %s\n", pSentence);

	free(pSentence);

	printf("Press any key to exit.\n");
	_getch();

#if defined(_DEBUG)
	_CrtDumpMemoryLeaks();
#endif // _DEBUG
}

char *MakeSentence(const char **words, int numWords)
{
	int size = 0;
	int iter = 0;
	char letter = 'a';
	int arrayLen = numWords - 1;

	for (int i = 0; i < arrayLen; i++)
	{
		letter = 'a';
		iter = 0;

		while (letter != '\0')
		{
			letter = words[i][iter];
			iter++;
		}
		size += iter;
	}

	char *sentence = (char*) malloc(sizeof(char) * (size + 1));

	size = 0;
	for (int i = 0; i < arrayLen; i++)
	{
		letter = 'a';
		iter = 0;

		while (letter != '\0')
		{
			letter = words[i][iter];
			if (letter != '\0')
			{
				sentence[size] = letter;
			}
			else
			{
				if (i == arrayLen - 1)
				{
					sentence[size] = '.';
					sentence[size + 1] = '\0';
				}
				else
				{
					sentence[size] = ' ';
				}
			}
			iter++;
			size++;
		}
	}

	return sentence;
}