#ifndef __DISPLAY_H_
#define __DISPLAY_H_

typedef enum
{
	daAlignLeft=0,
	daAlignRight,
	daAlignCenter
}daAlign;

#define ROW1 0
#define ROW2 1
#define ROW3 2
#define ROW4 3
#define ROW5 4
#define ROW6 5

int fs_DisplayAtRow(int Row,char *Text,daAlign Alignment);
int fs_DisplayAtRowReverse(int Row,char *Text,daAlign Alignment);
int fs_DisplayAtRowOverWrite(int Row,char *Text,daAlign Alignment);
int fs_DisplayAtRowOverWriteReverse(int Row,char *Text,daAlign Alignment);
int fs_DisplayInit(void);
int fs_DisplayClearRow(int Row);
int fs_DisplayClearAll(void);
bool fs_DisplayClearWindowContents(void);

#endif
