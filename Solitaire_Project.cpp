#include <iostream>
#include <stack>
#include <algorithm>
#include <cmath>
#include "Solitaire_class.h"
#include "goto-row-column and utility functions.h"
#include "Undo_redo.h"
#include <windows.h>



int main()
{
	Solitaire S;
	S.play_solitaire();
}



//int main()
// {
//	bool played = PlaySound(TEXT("flipcard.wav"), NULL,	SND_FILENAME | SND_ASYNC | SND_LOOP);
//	cout << played;
//	while (true) {
//
//	}
//	return	1;
//}

//int main()
//{
//	int x, y;
//	getRowColbyLeftClick(y, x);// 1st row then column
//	cout << "Row: " << y << " Column: " << x << endl;
//
//}

//int main()
//{
//	card c(1,'c');
//	int row, col;
//	cin >> row >> col;
//	c.card_printer(row, col,true);
//}


//int main()
//{
//	
//	for (int i{}; i < 100; i++)
//	{
//		SetClr(i,0);
//		cout << i << endl;
//		SetClr(15, 16);
//	}
//
//}


