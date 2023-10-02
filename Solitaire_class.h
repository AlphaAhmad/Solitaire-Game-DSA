#pragma once
#include "Stack.h"
#include "Card.h"
#include<algorithm>
#include <ctime>
#include <cstdlib>
#include "deque.h"
#include <vector>
#include "Undo_redo.h"
#include <windows.h>
#include "MMSystem.h"

class Solitaire
{
private:
	// object
	undo_redo obj;
	int score = 0;
	// variables part
	int row = 0, col = 0;
	int hardness_level;
	bool front_7_deck_bool = false; // if this becomes true this means we have picked cards from front 7 stacks
	bool last_4_deck_bool = false;
	bool retrieve_stack_bool = false;
	int choosen_index_1 = 0, choosen_index_2 = 0; // 1st index is for all card retrieving stacks but index_2 is only for front 7 stacks
	// arrays part

	vector<pair<int, char>> cards_52_deck; // 52 cards deck from which we are going to load the whole game
	vector<front_decks> front_deck_array; // front 7 decks which we use for stacking the cards
	vector<stack<card>> final_4_decks; // the decks on which all the cards are placed of same colour in an order A,2,3....K
	// 1st one: Spades    2nd one: Diamonds    3rd one: Hearts		4th one: Clubs

	Stack  card_picker; // for picking cards from the deque
	card_holder_dequeue de;

	//
	vector<card> pick_and_drop_stack; // stack which is going to be used from picking cards from one place to another 


public:

	// making copy constructor of Solitaire for undo-redo
	Solitaire(Solitaire& S) 
	{
		// replacting variables
		this->row= 0;//= S.row;
		this->col= 0;//= S.col;
		this->front_7_deck_bool = false;//= S.front_7_deck_bool;
		this->last_4_deck_bool = false;//= S.last_4_deck_bool;
		this->retrieve_stack_bool = false;//= S.retrieve_stack_bool;
		this->choosen_index_1 = 0;//S.choosen_index_1;
		this->choosen_index_2 = 0;// S.choosen_index_2;
		this->hardness_level = S.hardness_level;
		// replicating data structures
		
		// ----- 1
		this->cards_52_deck.clear();
		this->cards_52_deck.shrink_to_fit();
		for (int i = 0; i < S.cards_52_deck.size(); i++)
		{
			this->cards_52_deck.push_back(S.cards_52_deck[i]);
		}
		// ----- 2
		this->front_deck_array.clear();
		this->front_deck_array.shrink_to_fit();
		for (int i = 0; i < S.front_deck_array.size(); i++)
		{
			this->front_deck_array.push_back(S.front_deck_array[i]);
		}
		// ----- 3
		this->final_4_decks.clear();
		this->final_4_decks.shrink_to_fit();
		for (int i = 0; i < S.final_4_decks.size(); i++)
		{
			stack<card>k = S.final_4_decks[i];
			this->final_4_decks.push_back(k);
		}

		// ----- 4

		this->card_picker.Clear();
		this->card_picker.get_whole_stack().shrink_to_fit();
		for (int i = 0; i < card_picker.return_Size(); i++)
		{
			this->card_picker.get_whole_stack().push_back(S.card_picker.get_whole_stack()[i]);
		}
		 
		// ----  5
		this->de.clear_size_and_capacity();
		for (int i = 0; i < S.de.get_size(); i++)
		{
			this->de.Push_back(S.de[i]);
		}

		// no need to copy pick and drop stack


	}

	// ++++++++++++++   Constructor
	Solitaire()
	{
		cards_52_deck = vector<pair<int, char>>(52);


		gotoRowCol(30, 30);
		cout << "Which hardness level you want to choose (1.EASY   2.MEDIUM  3.HARD): ";
		cin >> this->hardness_level;
		while (this->hardness_level > 3 || this->hardness_level < 1)
		{
			system("cls");
			gotoRowCol(30, 30);
			cout << "Input Invalid: Pleast enter the hardness level from 1-3: ";
			cin>> this->hardness_level;
		}
		system("cls");

		// first creating an array of 52 cards of the game (deck of 52 cards)
		for (int i = 0; i < 52; i++)
		{

			cards_52_deck[i].first = (i % 13) + 1; // because each type (hearts, spades,...) have 13 cards each

			//  order in array: 1st clubs ; 2nd diamonds ; 3rd hearts ; 4th spades

			if ((i / 13) == 0)
			{
				cards_52_deck[i].second = 'c';  // c -> stands for clubs
			}
			else if ((i / 13) == 1)
			{
				cards_52_deck[i].second = 'd';  // d -> stands for diamonds
			}
			else if ((i / 13) == 2)
			{
				cards_52_deck[i].second = 'h';  // c -> stands for hearts
			}
			else
			{
				cards_52_deck[i].second = 's';  // s -> stands for spades
			}
		}

		// now shuffeling that array (deck of 52 cards) randomly
		srand(time(0));
		for (int i = 0; i < 200; i++)  // doing shuffeling by 150 swaps
		{
			unsigned int first_index = rand() % 52;
			unsigned int second_index = rand() % 52;
			swap(cards_52_deck[first_index], cards_52_deck[second_index]);
		}

		// now we are going to initialize the front decks and the remaining cards will go into the deque deck
		int index = 0;
		for (int i = 0; i < 7; i++)
		{
			vector<pair<int, char>> card_inserted;
			for (int j = index, k = 1; k <= i + 1; index++, k++, j++)
			{
				// cards to be inserted
				card_inserted.push_back(cards_52_deck[j]);
				//cards_52_deck.erase(cards_52_deck.begin());

			}

			// 1st deck will initially have 1 size and	then the size will increase with the increasing deck number		
			front_decks d(i + 1, i, card_inserted); // creting each deck of 7 and then pushing it into the front_deck_array
			front_deck_array.push_back(d);
		}

		// initializing the deck deque

		while (index != cards_52_deck.size())
		{
			card c(cards_52_deck[index].first, cards_52_deck[index].second);
			this->de.Push_back(c);
			index++;
		}

		// now initializing 4 final decks
		for (int i = 1; i <= 4; i++)
		{
			stack<card> V; // these are initially going to be empty
			final_4_decks.push_back(V);
		}


	}

	//++++++++++++++++++++ box printer

		// this is actualy going to ne used to make the deck holder 
	void box_printer(int row, int col, int dim)
	{
		int box_dim = dim;  
		SetClr(WHITE);
		for (int i = 0; i < box_dim; i++)
		{
			for (int j = 0; j < box_dim; j++)
			{
				if (i == 0 || i == (box_dim - 1) || j == 0 || j == (box_dim - 1))
				{
					gotoRowCol(row + i, col + j);
					cout << char(-37);
				}

			}
		}
	}


	//++++++++++++++++= printing the whole interface (includeing all the classes)
	void print_board()
	{
		//           Remember : card dimension 9x8

		// printing queue holder box
		box_printer(1, 13,14);
		// now printing the dequeue holding the with drawing cards
		if (!de.is_empty())
		{
			card decoy_card(1, 'x'); // just making a fake card for display
			decoy_card.card_printer(3, 15, false);
		}

		// now printing the stack which was extraced from the deque stack 
		int stack_initial_col = 40;
		for (int i = 0; i < card_picker.get_whole_stack().size(); i++)
		{										// v--initially we are going to start printing fron 3rd row and 40th column
			card_picker.get_whole_stack()[i].card_printer(3, 30 + (i * 20), true);
			// max value: 30+(3*20) = 90
		}
		// now we are going to print all the front decks
			// traversing ito the array of 7 decks
		int front_decks_start_column = 2;
		int front_decks_start_row = 19;

		for (int i{}; i < this->front_deck_array.size(); i++)
		{    // traversing into each deck
			for (int j = 0; j < front_deck_array[i].get_whole_stack().size(); j++)
			{
				bool should_reveal_card = true;
				// checking that should the card be hided which is about to be printed
				if (front_deck_array[i].how_much_cards_to_hide() > j)
				{
					should_reveal_card = false;
				}
				// printing the all the decks 1 by 1 
				front_deck_array[i].get_whole_stack()[j].card_printer(front_decks_start_row + (12 * i), front_decks_start_column + (10 * j), should_reveal_card);
				
			}

			// now we are going to print the stacks that are going to hold the final cards (last_4_stacks)
			for (int i = 0; i < 4; i++)
			{

				box_printer(1, 110 + (17 * i),14);

				if (i == 0)
				{
					if (!this->final_4_decks[i].empty()) // agr 1st final_deck empty nhi ha
					{
						this->final_4_decks[i].top().card_printer(3, 112, true);
					}
					else
					{
						gotoRowCol(7, 114 + (17 * i));
						cout << "SPADES";
					}
				}
				else if (i == 1)
				{
					if (!this->final_4_decks[i].empty()) // agr 2nd final_deck empty nhi ha
					{
						this->final_4_decks[i].top().card_printer(3, 112 + (17 * i), true);
					}
					else
					{
						gotoRowCol(7, 114 + (17 * i));
						cout << "DIMONDS";
					}

				}
				else if (i == 2)
				{
					if (!this->final_4_decks[i].empty()) // agr 3rd final_deck empty nhi ha
					{
						this->final_4_decks[i].top().card_printer(3, 112 + (17 * i), true);
					}
					else
					{
						gotoRowCol(7, 114 + (17 * i));
						cout << "HEARTS";
					}

				}
				else if (i == 3)
				{
					if (!this->final_4_decks[i].empty())// agr 4th final_deck empty nhi ha
					{
						this->final_4_decks[i].top().card_printer(3, 112 + (17 * i), true);
					}
					else
					{
						gotoRowCol(7, 114 + (17 * i));
						cout << "CLUBS";
					}

				}
			}
		}
		SetClr(15);
		// printing undo button
		box_printer(1,180,8);
		SetClr(12);
		gotoRowCol(3, 182);
		cout << "undo";

		// printing redo button
		SetClr(15);
		box_printer(13, 180,8);
		SetClr(12);
		gotoRowCol(15, 182);
		cout << "redo";

		SetClr(15);
		gotoRowCol(103,165);
		cout << "Current Score : " << this->score;
	}
protected:

			//++++++++++++++++   Checking functions ++++++++++++++++++++

			// NOTE: We are not putting these check functions in there respctive classes because they dont
			// use any class functinality  they are only using index calculation but we have "protected" them

// this function is only dealing with front 7 stacks       

				// this function is also going to be used in placing cards
				// but there we only need the index of which front stack						
				// was choosen not the second index of its element
					// thats why using "bool need_second_index"
	bool front_7_stack_index_finder(int _r, int _c, int& index_1, int& index_2 , bool need_second_index)
{														
		// finding the number of stack that has been choosed (index_1)
		if (_r >= 19 && _r <= 28)
		{
			index_1 = 0;
		}
		else if (_r >= 31 && _r <= 40)
		{
			index_1 = 1;
		}
		else if (_r >= 43 && _r <= 52)
		{
			index_1 = 2;
		}
		else if (_r >= 55 && _r <= 64)
		{
			index_1 = 3;
		}
		else if (_r >= 67 && _r <= 76)
		{
			index_1 = 4;
		}
		else if (_r >= 79 && _r <= 88)
		{
			index_1 = 5;
		}
		else if (_r >= 91 && _r <= 100)
		{
			index_1 = 6;
		}
		else
		{
			return false; // none of the stack was choosed
		}
		// now finding the card of the stack that has been choosed (index_2)
		if (need_second_index)
		{
			for (int i = 0; i <= 19; i++)  // A stack can hold max 19 cards 
			{
				if (_c >= (2 + (10 * i)) && _c < (12 + (10 * i)))
				{
					index_2 = i;
					return true;
				}
			}
			return false;
		}
		else
		{
			index_2 = 0;
			return true;
		}

	}

	// this function is only dealing with last 4 stacks
	bool getting_index_of_last_stacks(int _r, int _c, int& index)
	{
		// We only need to find which stack is choosen so only 1 index is needed
		if (_r <= 14 && _c >= 110)
		{
			// found the 1st index
			for (int i = 0; i < 4; i++) // we have only 4 stacks
			{
				if (_c >= 110 + (17 * i) && _c < (110 + (17 * i)) + 14)
				{
					index = i;
					return true; // stack has been picked
				}

			}
		}
		return false; // any of the last 4 stacks were not chosen

	}

	// function for checking the stack drawn from the cards queue
	bool checking_queue_drawn_stack(int _r, int _c)
	{
		if (_r <= 12)
		{
			if (_c >= 30 && _c <= 90)
			{
				return true;
			}
		}
		return false;
	}

public:

	//+++++++++  validates and picks the cards from a stack (this one does not covers the deck_queue (de) to card_picker_stack transfer)
	// if this becomes true this means we have picked cards from front 7 stacks, retriving deck or from last_4_decks
	
	
	bool Picking_cards(int row,int col,vector<card> &cards_picked, int &choosen_index_1,int &choosen_index_2,bool &front_7_deck_bool , bool &last_4_deck_bool , bool &retrieve_stack_bool)
	{
		

		int last_stack_index = 0; // only need 1 index needed for last stacks (last_4_stacks)
		int front_stack_index_1 = 0, front_stack_index_2 = 0;  // indexes for front 7 stacks only

		// +++++++++  1st we are going to check wheather the front decks were picked or not
		// if the user has choosen 1 of the front stacks
		if (this->front_7_stack_index_finder(row, col, front_stack_index_1, front_stack_index_2,true))
		{
			// checking if the choosen index of the stack is smaller then its size, if no then take inputs again
			if (front_stack_index_2 >= this->front_deck_array[front_stack_index_1].return_Size())
			{
				return false;   // cards were not picked
			}
			if (front_stack_index_2 < this->front_deck_array[front_stack_index_1].how_much_cards_to_hide())
			{
				return false;   // the cardd picked are hidden to the player (in simple words inexcessable right now)
			}
			if (this->front_deck_array[front_stack_index_1].empty())
			{
				return false; // front stack ha hi empty
			}
			else
			{
 				choosen_index_1 = front_stack_index_1; choosen_index_2 = front_stack_index_2; // saving the indexes in the main
				this->front_deck_array[front_stack_index_1].card_deck_retriever(front_stack_index_2, cards_picked);

				return front_7_deck_bool=true; // cards were picked
			}

		}

		//  ++++++++  Now we are going to check wheather indexes choosen are of the last stacks (final_4_decks)

		else if (this->getting_index_of_last_stacks(row, col, last_stack_index)) 
		{
			if (!this->final_4_decks[last_stack_index].empty()) // if the stack is not empty
			{
				choosen_index_1 = last_stack_index;
				cards_picked.push_back(this->final_4_decks[last_stack_index].top());
				return last_4_deck_bool= true; // card has been picked
			}
			else
			{
				return false;
			}
		}

		// now we are going to check wheather the choosen stack is the one drawn from the queue
		else if (checking_queue_drawn_stack(row, col))
		{
			if (!this -> card_picker.empty())
			{
				cards_picked.push_back(this->card_picker.top());
				return retrieve_stack_bool=true;
			}
			
		}
		return false;

	}

	bool final_deck_condtions(int last_stack_index,vector<card>& cards_picked)
	{
		if (last_stack_index == 0 && cards_picked[0].get_card_type() == 's')
		{
			
			return true;
		}
		else if (last_stack_index == 1 && cards_picked[0].get_card_type() == 'd')
		{
			
			return true;
		}
		else if (last_stack_index == 2 && cards_picked[0].get_card_type() == 'h')
		{
			
			return true;
		}
		else if (last_stack_index == 3 && cards_picked[0].get_card_type() == 'c')
		{
			
			return true;
		}
		return false;
	}
	
	
	//  
	void finding_the_stack_and_deleting_cards(int choosen_index_1,int choosen_index_2, bool& front_7_deck_bool, bool& last_4_deck_bool, bool& retrieve_stack_bool)
	{
		if (front_7_deck_bool)
		{
			if (this->front_deck_array[choosen_index_1].how_much_cards_to_hide() != 0 && this->front_deck_array[choosen_index_1].how_much_cards_to_hide() == choosen_index_2)
			{
				this->front_deck_array[choosen_index_1].how_much_cards_to_hide()--;
			}
				this->front_deck_array[choosen_index_1].cards_deck_deleter(choosen_index_2); // deleting from previous
			
		}
		else if (last_4_deck_bool)
		{
			this->final_4_decks[choosen_index_1].pop();
		}
		else if (retrieve_stack_bool)
		{
			if (!this->card_picker.empty())
			{
				this->card_picker.pop();
			}
			
		}
	}


	// now this is going to validate wheather the cards sholud be placed or not and if yes then it will place it
	bool placing_cards(int &score,int row, int col, vector<card>& cards_picked, int& choosen_index_1, int& choosen_index_2, bool& front_7_deck_bool, bool& last_4_deck_bool, bool& retrieve_stack_bool)
	{
		int last_stack_index=0;
		int front_stack_index_1 = 0, front_stack_index_2 = 0;  // indexes for front 7 stacks only


		if (this->front_7_stack_index_finder(row, col, front_stack_index_1, front_stack_index_2, false))
		{     // if the stack on which we are going to place the cards is empty then
			if (this->front_deck_array[front_stack_index_1].empty())
			{
				if (cards_picked[0].get_card_number() == 13) // if the 1st card that is about to be placed is king
				{
					
					this->obj.save_progress(score,hardness_level,  front_deck_array, final_4_decks, card_picker, de);
					this->front_deck_array[front_stack_index_1].card_deck_placer(cards_picked); // placing onto new 
					finding_the_stack_and_deleting_cards( choosen_index_1,choosen_index_2, front_7_deck_bool, last_4_deck_bool,  retrieve_stack_bool);
					system("cls");
					return true;

				}
				else
				{
					return false; // if it is not king then cards are not placed
				}
			}
			else // if not empty
			{  // max capacity of a front deck stack is 19 cards
				if (this->front_deck_array[front_stack_index_1].return_Size()<=19)
				{
						// if the colour of the card about to be placed is same as on the top do not place
					if (this->front_deck_array[front_stack_index_1].top().get_card_clr() != cards_picked[0].get_card_clr())
					{		// check if the card on the top of front deck is 1 number greater than the card about to be placed infront of him
						if (this->front_deck_array[front_stack_index_1].top().get_card_number()-1== cards_picked[0].get_card_number() )
						{
							this->obj.save_progress(score,hardness_level, front_deck_array, final_4_decks, card_picker, de);
							this->front_deck_array[front_stack_index_1].card_deck_placer(cards_picked); // placing onto new 
							//this->front_deck_array[choosen_index_1].cards_deck_deleter(choosen_index_2); // deleting from previous
							// reducing the number of cards that this particular front stack is going to hide
							finding_the_stack_and_deleting_cards(choosen_index_1, choosen_index_2, front_7_deck_bool, last_4_deck_bool, retrieve_stack_bool);
							system("cls");
							return true;
						}

					}
				}
				
			
			}
			return false;
		}

		// now checking if the placing is being done on the last 4 decks
		else if(getting_index_of_last_stacks(row,col,last_stack_index))
		{		// max number of cards that can be put on the last stacks is one
			if (cards_picked.size() == 1)
			{
				if (this->final_4_decks[last_stack_index].empty())
				{
					if (cards_picked[0].get_card_number() == 1)
					{
						// both in these conditions we need to push
						if ((last_stack_index == 0 || last_stack_index == 3) && (cards_picked[0].get_card_clr() == 8))
						{
							if (final_deck_condtions(last_stack_index, cards_picked))
							{
								this->obj.save_progress(score,hardness_level, front_deck_array, final_4_decks, card_picker, de);
								this->final_4_decks[last_stack_index].push(cards_picked[0]);
								score += cards_picked[0].get_card_number();
								finding_the_stack_and_deleting_cards(choosen_index_1, choosen_index_2, front_7_deck_bool, last_4_deck_bool, retrieve_stack_bool);
								system("cls");
								return true;
							}
							
						}
						else if ((last_stack_index == 1 || last_stack_index == 2) && (cards_picked[0].get_card_clr() == 12))
						{
							if (final_deck_condtions(last_stack_index, cards_picked))
							{
								this->obj.save_progress(score,hardness_level, front_deck_array, final_4_decks, card_picker, de);
								this->final_4_decks[last_stack_index].push(cards_picked[0]);
								score += cards_picked[0].get_card_number();
								finding_the_stack_and_deleting_cards(choosen_index_1, choosen_index_2, front_7_deck_bool, last_4_deck_bool, retrieve_stack_bool);
								system("cls");
								return true;
							}
							
							
						}
					}
				}
				else if (this->final_4_decks[last_stack_index].size() < 13)
				{    // agr cards ka colour same  ha too ak cards dosra pa rakha ja sakta ha
					if (this->final_4_decks[last_stack_index].top().get_card_clr() == cards_picked[0].get_card_clr())
					{
						if (this->final_4_decks[last_stack_index].top().get_card_number() == cards_picked[0].get_card_number()-1)
						{
							if (final_deck_condtions(last_stack_index, cards_picked))
							{
								this->obj.save_progress(score,hardness_level, front_deck_array, final_4_decks, card_picker, de);
								this->final_4_decks[last_stack_index].push(cards_picked[0]); // cards pushed
								score += cards_picked[0].get_card_number();
								finding_the_stack_and_deleting_cards(choosen_index_1, choosen_index_2, front_7_deck_bool, last_4_deck_bool, retrieve_stack_bool);
								system("cls");
								return true;
							} 
							
						}
					}
				}
			}
		}

		return false; // card not placed
	}
	
	// checking if the queue has been selected
	bool queue_selected(int row,int col)
	{
		if (row <= 15 && (col >= 13 && col <= (13 + 14)))
		{
			return true; // queue has been clicked
		}
		
		return false; // queue has not been clicked
	}

	// if the queue is selected then perform queue operation
	void queue_operation()
	{
		// the stack has been replaced in the queue
		while (!this->card_picker.empty())
		{
			/*this->de.pushFront(this->card_picker.top());
			this->card_picker.pop();*/
			this->de.push_stack_back(this->card_picker);
		}
		// now we are going to draw new cards from the queue
		this->de.retrieve_stack(this->hardness_level, this->card_picker); //+++++++  check if this is working properly
	}
	
	bool have_won()
	{
		
		for (int i = 0; i < 4; i++)
		{
			if (this->final_4_decks[i].size() != 13)
			{
				return false; // game is still going
			}
		}
		return true; // game has ended player has Won;
	}
	
	

	void play_solitaire()
	{
		while (!have_won())
		{
			
			print_board();
			pick_and_drop_stack.clear();
			int previous_row = row, previous_col = col;
			front_7_deck_bool = false; last_4_deck_bool = false; retrieve_stack_bool = false;
			//this->obj.save_progress(hardness_level, front_deck_array, final_4_decks, card_picker, de);
			getRowColbyLeftClick(row, col); //Taking input from the mouse to place the cards
			while (row == previous_row && previous_col == col)
			{
				getRowColbyLeftClick(row, col);
			}

			if (obj.undo_preseed(row, col))
			{
				obj.infinite_undo(score,row, col, front_7_deck_bool, last_4_deck_bool, retrieve_stack_bool, choosen_index_1, choosen_index_2, front_deck_array, final_4_decks, card_picker, de);
				system("cls");
				continue;
			}
			else if (obj.redo_pressed(row, col))
			{
				obj.infinite_redo(score,row, col, front_7_deck_bool, last_4_deck_bool, retrieve_stack_bool, choosen_index_1, choosen_index_2, front_deck_array, final_4_decks, card_picker, de);
				system("cls");
				continue;
			}
			// if we did not picked card then we need to check if  
			if (Picking_cards(row,col,this->pick_and_drop_stack,choosen_index_1,choosen_index_2,front_7_deck_bool,last_4_deck_bool,retrieve_stack_bool))  
			{
				PlaySound(L"flipcard.wav", NULL, SND_FILENAME | SND_ASYNC);
				// if we have picked cards
				previous_row = row; previous_col = col;
				getRowColbyLeftClick(row, col); //Taking input from the mouse to place the cards  
				while (row == previous_row && previous_col == col)
				{
					getRowColbyLeftClick(row, col);
				}
				if (placing_cards(score, row, col, this->pick_and_drop_stack, choosen_index_1, choosen_index_2, front_7_deck_bool, last_4_deck_bool, retrieve_stack_bool))
				{
					PlaySound(L"put down.wav", NULL, SND_FILENAME | SND_ASYNC);
				}
			}
			else if (queue_selected(row, col)) // if we want to retrieve cards from the  queue
			{
				this->obj.save_progress(score,hardness_level, front_deck_array, final_4_decks, card_picker, de);
				PlaySound(L"shuffling-cards_trim.wav", NULL, SND_FILENAME | SND_ASYNC);
				this->queue_operation(); // then perform queue operation
				system("cls");
			}
		}
	}
};





