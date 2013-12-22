
// ConsoleColors.cpp

// Tests the various console color combinations

#include <stdio.h>
#include <stdlib.h>





/// Evaluates to the number of elements in an array (compile-time!)
#define ARRAYCOUNT(X) (sizeof(X) / sizeof(*(X)))





int main(void)
{
	static const char * Attribs[] =
	{
		"0",  // All attribs off
		"1",  // bold
		"2",  // faint
		"7",  // inverse
		"1;7",  // bold inverse
		"2;7",  // faint inverse
	} ;
	for (int i = 0; i < ARRAYCOUNT(Attribs); i++)
	{
		const char * Attrib = Attribs[i];
		for (int fg = 30; fg <= 37; fg++)
		{
			for (int bg = 40; bg <= 47; bg++)
			{
				printf("\x1b[%s;%d;%dm %s;%d;%d ", Attrib, fg, bg, Attrib, fg, bg);
			}  // for bg
			puts("\x1b[0m");  // Reset terminal back to normal colors	for the newline
		}  // for fg
	}  // for i - Attribs[]

	for (int i = 1; i < ARRAYCOUNT(Attribs); i++)
	{
		const char * Attrib = Attribs[i];
		for (int fg = 30; fg <= 37; fg++)
		{
			for (int bg = 40; bg <= 47; bg++)
			{
				printf("\x1b[%d;%d;%sm %d;%d;%s ", fg, bg, Attrib, fg, bg, Attrib);
			}  // for bg
			puts("\x1b[0m");  // Reset terminal back to normal colors	for the newline
		}  // for fg
	}  // for i - Attribs[]

	return 0;
}




