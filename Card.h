 #pragma once
#include <stdexcept>
#include<string>
#include "goto-row-column and utility functions.h"
using namespace std;

class card
{
private:

	int card_number;
	char card_type; // clubs , diamonds, hearts, spades
	string card_symbol; // A, 2 ,3 4,..., J,Q,K (symbol representation)
	int clr; // colour of the card  Red or Grey

public:
	card()
	{
		this->card_number = 0;
		card_type = {};
		card_symbol = {};
		clr = {};
	}
	card(const int &card_value,const char& type)
	{
		this->card_number = card_value;
		this->card_type = type;
		// choosing card symbol
		if (card_number >= 2 && card_number <= 10)
		{
			this->card_symbol = to_string(card_number);  // check this   ++++++++++(THIS STORING OF CHAR IS WRONG)
		}
		else if (card_number == 1)
		{
			this->card_symbol = "A"; 
		}
		else if (card_number==11)
		{
			this->card_symbol = "J";
		}
		else if (card_number == 12)
		{
			this->card_symbol = "Q";
		}
		else if(card_number == 13)
		{
			this->card_symbol = "K";
		}
		else
		{
			throw exception("\nInvalid card number found in the card constructor\n");
		}

		// card color choosing
		if (this->card_type == 's' || this->card_type== 'c')
		{
			this->clr = 8; // dark grey colour
		}
		else if(this->card_type == 'h' || this->card_type == 'd')
		{
			this->clr = 12; // red colour
		}
		else
		{
			this->clr = 2;  // light green
		}
	}

	card(const card &c) // copy constructor
	{
		this->card_number = c.card_number;
		this->card_symbol = c.card_symbol;
		this->card_type = c.card_type;
		this->clr = c.clr;
	}

	void card_printer(int start_row,int start_col, bool is_reveal)
	{ 
		//++++++++  card dimensions are 9x8

		int dim_limit_row = 9;  
		int dim_limit_col = 8;
		// 1st checking if the card has to be revealed or not
		if (is_reveal)
		{
			// 1st printing card symbol
			gotoRowCol(start_row + (dim_limit_row / 2), start_col + (dim_limit_col / 2));
			cout << this->card_symbol;
			// then printing the type
			gotoRowCol(start_row + 2, start_col + 2);
			if (this->card_type == 's')
			{
				cout << "SPADES";
			}
			else if (this->card_type == 'd')
			{
				cout << "DIMOND";
			}
			else if (this->card_type == 'h')
			{
				cout << "HEART";
			}
			else if (this->card_type == 'c')
			{
				cout << "CLUBS";
			}
		}
		// and now printing the square shape of the card
		SetClr(this->clr);
		gotoRowCol(start_row, start_col);
		for (int i = {  }; i <=  dim_limit_row;i++)
		{
			for (int j{ }; j <=  dim_limit_col; j++)
			{
				
				if (i == 0 || i == dim_limit_row || j == 0 || j == dim_limit_col)
				{
					gotoRowCol(i+start_row, j+start_col);
					cout << char(-37);
				}
				if (!is_reveal)
				{
					if (i == j)
					{
						gotoRowCol(i + start_row, j + start_col);
						cout << char(-37);
					}
				}
				
			}
		}
		SetClr(15);
	}


	int get_card_number()
	{
		return this->card_number;
	}

	char get_card_type()
	{
		return this->card_type;
	}

	int get_card_clr()
	{
		return this->clr;
	}

};
