#pragma once
#include <vector>
#include "Card.h"
#include <stdexcept>
using namespace std;


// creating our own stack because we need more functionalities which the buitin STL stack cannot provide

class Stack
{
private:
	vector<card> Vs;   
protected:

	

public:
	Stack()
	{

	}
	Stack(int size)
	{
		this->Vs = vector<card>(size);
	}
	// this function will give us the vector working begind this satck by reference so we can change it how we can
	// this function is going to be used in derived class
	vector<card>& get_whole_stack()
	{
		return Vs;
	}

	void push(card c)
	{
		Vs.push_back(c);
	}
	
	card& top()
	{
		if (!this->empty())
		{
			return Vs[Vs.size() - 1];
		}
		else
		{
			throw exception("stack is empty cannot return top value");
		}
	}

	void pop()
	{
		Vs.pop_back();
	}

	int return_Size()
	{
		return this->Vs.size();
	}
	bool empty()
	{
		return this->Vs.size() == 0;
	}
	void Clear()
	{
		this->Vs.clear();
	}

};



		// inherited publically from stack

// these are the front card decks in which we are going to do 
class front_decks : public Stack
{
	int cards_to_hide;

public:

	front_decks()
	:Stack(1)
	{
		cards_to_hide = 0;
	}

	int& how_much_cards_to_hide()
	{
		return this->cards_to_hide;
	}

													// v-- number of cards that are going to be loaded in the deck initially
	front_decks(int size,const int  deck_number,vector<pair<int,char>> cards_to_load)
	{
		this->cards_to_hide = deck_number; // the number of the deck from 0-6 is also the number of cards the deck will hide initially
		
		// loading cards
		for (int i{}; i < cards_to_load.size(); i++)
		{
			card c(cards_to_load[i].first, cards_to_load[i].second);
			this->push(c); // pushing into the deck
		}
		
	}

	
	/*vector<card> operator[] (int x) 
	{
		if (x < this->return_Size())
		{
			return this->get_whole_stack()[x];
		}
		return ;
	}*/


			// Checking for thiese functions will happen in the game class


	// this is going to retrieve all the cards onward from the index we are going to chose 
	// this function is going to be used in shifting the cards from one deck to another
	void card_deck_retriever(const int &index, vector<card> &cards_to_be_shifted)
	{
		
		for (int i{ index }; i < this->return_Size(); i++)
		{
			cards_to_be_shifted.push_back(this->get_whole_stack()[i]);
		}
	}

	// when a deck of cards is removed from a column then this function will delete 
	// this function is going to be used in shifting the cards from one deck to another
	void cards_deck_deleter(const int &index)
	{  // deleting from that index to the end
		this->get_whole_stack().erase(get_whole_stack().begin() + index, get_whole_stack().end());
	}

	// this function is going to be used for placing the cards onto the new deck
	// this function is going to be used in shifting the cards from one deck to another
	void card_deck_placer(vector<card> &cards)
	{
		int size = cards.size();
		for (int i{}; i < size; i++)
		{   // placing cards 
			this->push(cards[i]);
		}
	}
};














