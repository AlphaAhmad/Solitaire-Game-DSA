#pragma once
#include<algorithm>
#include <ctime>
#include <cstdlib>
#include "deque.h"
#include <vector>

using namespace std;

class data_container
{

private:
	int hardness_level;
	int score;
	vector<pair<int, char>> cards_52_deck; // 52 cards deck from which we are going to load the whole game
	vector<front_decks> front_deck_array; // front 7 decks which we use for stacking the cards
	vector<stack<card>> final_4_decks; // the decks on which all the cards are placed of same colour in an order A,2,3....K
	// 1st one: Spades    2nd one: Diamonds    3rd one: Hearts		4th one: Clubs

	Stack  card_picker; // for picking cards from the deque
	card_holder_dequeue de;

public:
	data_container()
	{
		hardness_level = 0; 
		score = 0;
	}

	void from_game_to_copy(int score,int hardness_level, vector<front_decks> front_deck_array, vector<stack<card>> final_4_decks, Stack  card_picker, card_holder_dequeue de)
	{

		this->hardness_level = hardness_level;
		this->score = score;
		// replicating data structures
		
		// ----- 2
		this->front_deck_array.clear();
		this->front_deck_array.shrink_to_fit();
		for (int i = 0; i < front_deck_array.size(); i++)
		{
			this->front_deck_array.push_back(front_deck_array[i]);
		}
		// ----- 3
		this->final_4_decks.clear();
		this->final_4_decks.shrink_to_fit();
		for (int i = 0; i < final_4_decks.size(); i++)
		{
			stack<card>k = final_4_decks[i];
			this->final_4_decks.push_back(k);
		}

		// ----- 4

		this->card_picker.Clear();
		this->card_picker.get_whole_stack().shrink_to_fit();
		for (int i = 0; i < card_picker.return_Size(); i++)
		{
			this->card_picker.get_whole_stack().push_back(card_picker.get_whole_stack()[i]);
		}
		// -----  5
		this->de.clear_size_and_capacity();
		for (int i = 0; i < de.get_size(); i++)
		{
			this->de.Push_back(de[i]);
		}

	}

	void from_copy_to_game(int &score,int &row,int &col, bool &front_7_deck_bool, bool &last_4_deck_bool, bool &retrieve_stack_bool, int &choosen_index_1,int & choosen_index_2,  vector<front_decks> &front_deck_array, vector<stack<card>> &final_4_decks, Stack  &card_picker, card_holder_dequeue &de)
	{
		row = 0;//= S.row;
		col = 0;//= S.col;
		front_7_deck_bool = false;//= S.front_7_deck_bool;
		last_4_deck_bool = false;//= S.last_4_deck_bool;
		retrieve_stack_bool = false;//= S.retrieve_stack_bool;
		choosen_index_1 = 0;//S.choosen_index_1;
		choosen_index_2 = 0;// S.choosen_index_2;
		hardness_level = this->hardness_level;
		score = this->score;
		// copying back data structures

	// ----- 1
		/*cards_52_deck.clear();
		cards_52_deck.shrink_to_fit();
		for (int i = 0; i < this->cards_52_deck.size(); i++)
		{
			cards_52_deck.push_back(this->cards_52_deck[i]);
		}*/
		// ----- 2
		front_deck_array.clear();
		front_deck_array.shrink_to_fit();
		for (int i = 0; i < this->front_deck_array.size(); i++)
		{
			front_deck_array.push_back(this->front_deck_array[i]);
		}
		// ----- 3
		final_4_decks.clear();
		final_4_decks.shrink_to_fit();
		for (int i = 0; i < this->final_4_decks.size(); i++)
		{
			stack<card>k = this->final_4_decks[i];
			final_4_decks.push_back(k);
		}

		// ----- 4

		card_picker.Clear();
		card_picker.get_whole_stack().shrink_to_fit();
		for (int i = 0; i < this->card_picker.return_Size(); i++)
		{
			card_picker.get_whole_stack().push_back(this->card_picker.get_whole_stack()[i]);
		}

		// -----  5
		de.clear_size_and_capacity();
		for (int i = 0; i < this->de.get_size(); i++)
		{
			de.Push_back(this->de[i]);
		}
	}


};



class undo_redo
{
private:
	
	vector<data_container>  Solitaire_copies;//= vector<data_container> (250);
	int index;
	//int size;
public:
	// constructor


	undo_redo()
	{
		index = 0;
		this->Solitaire_copies= vector<data_container> (1);
	}

	bool undo_preseed(int _r, int _c)
	{
		if (_c >= 180 && _r<= 9)
		{
			return true;
		}
		return false;
	}

	bool redo_pressed(int _r, int _c)
	{
		if (_c >= 180 && (_r >= 12 && _r<=21))
		{
			return true;
		}
		return false;
	}

	void infinite_undo(int &score,int& row, int& col, bool& front_7_deck_bool, bool& last_4_deck_bool, bool& retrieve_stack_bool, int& choosen_index_1, int& choosen_index_2, vector<front_decks>& front_deck_array, vector<stack<card>>& final_4_decks, Stack& card_picker, card_holder_dequeue& de)
	{
		if (this->index > 0)
		{
			//S = this->Solitaire_copies[--(this->index) ];
			this->Solitaire_copies[--this->index].from_copy_to_game(score,row, col,  front_7_deck_bool, last_4_deck_bool, retrieve_stack_bool, choosen_index_1,choosen_index_2, front_deck_array,final_4_decks,card_picker,de);
		}
		
	}

	void infinite_redo(int &score,int& row, int& col, bool& front_7_deck_bool, bool& last_4_deck_bool, bool& retrieve_stack_bool, int& choosen_index_1, int& choosen_index_2,  vector<front_decks>& front_deck_array, vector<stack<card>>& final_4_decks, Stack& card_picker, card_holder_dequeue& de)
	{
		if (this->index < this->Solitaire_copies.size()-2)
		{
			//S = this->Solitaire_copies[++this->index];
			this->Solitaire_copies[++this->index].from_copy_to_game(score,row, col, front_7_deck_bool, last_4_deck_bool, retrieve_stack_bool, choosen_index_1, choosen_index_2, front_deck_array, final_4_decks, card_picker, de);
		}
	}

	void save_progress(int &score,int &hardness_level , vector<front_decks>& front_deck_array, vector<stack<card>>& final_4_decks, Stack& card_picker, card_holder_dequeue& de) // so we dont have to make copy
	{
		if (index == Solitaire_copies.size()-1)
		{
			//this->Solitaire_copies[index] = S;// making deep copy
			this->Solitaire_copies[this->index].from_game_to_copy(score,hardness_level , front_deck_array, final_4_decks, card_picker, de);
			data_container  dummy_S;
			this->Solitaire_copies.push_back(dummy_S); // making next slot with a dummy parameter and later it will be over written
		}
		else if (index < Solitaire_copies.size()-1 )
		{
			//this->Solitaire_copies[index] = S;  // over writing
			this->Solitaire_copies[index].from_game_to_copy(score,hardness_level, front_deck_array, final_4_decks, card_picker, de);
		}
		index++;
	}

};





