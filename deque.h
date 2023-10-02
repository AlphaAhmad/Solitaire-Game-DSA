#pragma once
#include <iostream>
#include <vector>
#include <stdexcept>
#include <algorithm>
#include <stdexcept>
#include "Card.h"
#include "Stack.h"
#include <deque>
using namespace std;

//template <typename T>
//class Dequeue {
//	T* Arr;
//	int size;
//	int capacity;
//	int front, back, middle;
//public:
//	Dequeue() {
//		this->Arr = new T[10];  // default capacity set to 5
//		this->capacity = 10;
//		this->size = 0;
//		this->front = -1;
//		this->back = 0;
//		this->middle = (this->front + int(this->size / 2));
//	}
//	void pushFront(T val) // pushing on the front
//	{
//		if (this->front == -1)
//		{
//			this->Arr[0] = val;
//			this->front = 0;
//			this->back++;
//			this->middle = (this->front + int((this->size + 1) / 2));
//		}
//		else
//		{
//			// if the back index is just below the front index and we are pushing front then we have to alocate 
//		// new memory because our size is now equal to our capacity
//			if (((this->front - 1) <= this->back || ((this->front + this->size) - 1) == this->back) && this->size >= this->capacity)
//			{
//				T* decoy = new T[2 * this->capacity];
//				// keeping the 1st index empty for the new value
//				for (int i = this->front, j = 1; i != this->back && j <= this->size; i = ((i + 1) % this->capacity), j++)
//				{
//					decoy[j] = this->Arr[i];
//				}
//				delete[] this->Arr;
//				this->Arr = decoy;
//				this->front = 0;
//				this->back = this->size;
//
//				this->capacity = 2 * this->capacity;
//				decoy = nullptr;
//				this->Arr[this->front] = val;
//			} // in simple condition
//			else
//			{
//				this->front--;
//				if (this->front < 0)
//				{
//					this->front += this->capacity;
//				}
//				this->Arr[this->front] = val;
//			}
//			this->middle = abs(this->capacity - this->front) + int(this->size / 2);
//		}
//		
//		this->size++;
//		
//    }
//
//    int Size()
//    {
//        return this->size;
//    }
//
//     
//    void pushBack(T val) // pushing on the last
//	{   // 1st checking if the capacity is about to get full because of this insertion then we need to make a new array
//		if (this->size == 0)
//		{
//			this->pushFront(val);
//		}
//		else
//		{
//			if (((this->back + 1) % this->capacity) >= this->front && (this->size + 1) >= this->capacity)
//			{
//				T* decoy = new T[2 * this->capacity];
//				for (int i = this->front, j = 0; i != this->back && j < this->size; i = ((i + 1) % this->capacity), j++)
//				{
//					decoy[j] = this->Arr[i];
//				}
//				delete[] this->Arr;
//				this->Arr = decoy;
//				this->front = 0;
//				this->back = this->size;
//
//				this->capacity = 2 * this->capacity;
//				decoy = nullptr;
//			}
//			this->Arr[this->back] = val;
//			this->back = (this->back + 1) % this->capacity;
//			this->size++;
//			this->middle = (this->front + int(this->size / 2));
//		}
//		
//    }
//
//    T popFront() {
//
//		if (this->size > 0)
//		{
//			T val = this->Arr[this->front];
//			this->front = (this->front+1)%this->capacity;
//			this->size--;
//			return T(val);
//		}
//		else
//		{
//			throw exception("The deque is of size 0 cannot pop");
//		}
//    }
//
//    
//
//    T popBack() 
//	{
//		if (this->size > 0)
//		{
//			int val = 0;
//			if ((this->back - 1) < 0)
//			{
//				this->back = (this->back - 1) + this->capacity;
//
//			}
//			else
//			{
//				this->back--;
//			}
//			return T(this->Arr[this->back]);
//		}
//		else
//		{
//			throw exception("The deque is of size 0 cannot pop");
//		}
//    }
//    T operator[](int index)
//    {
//		if((this->front + index) % this->capacity<this->back)
//		{
//			return this->Arr[(this->front + index) % this->capacity];
//		}
//		else
//		{
//			cout << "\nThe value 0 is only threshold because the required index is out of bound\n";
//			
//			return T(0);
//		}
//		
//		
//    }
//
//};


class card_holder_dequeue 
{
	deque<card> Vs;
public:
	card_holder_dequeue()
	{
		
	}

	int get_size()
	{
		return this->Vs.size();
	}

	void Push_back(card k)
	{
		this->Vs.push_back(k);
	}

	void push_stack_back(Stack &S)
	{
		while (!S.empty())
		{
			//this->Vs->pushFront(S.top());
			this->Vs.push_front(S.top());
			S.pop();
		}
	}
	// this function will also delete the values from the dequeue that are being retrieved from the stack 
	void retrieve_stack(int hardness_level,Stack &S)
	{
		for (int i = 1; i <= hardness_level && i<this->Vs.size(); i++)
		{
			//S.push(this->Vs.popBack());
			S.push(this->Vs.back());
			this->Vs.pop_back();
		}
	}

	bool is_empty()
	{
		return this->Vs.empty();
	}

	void clear_size_and_capacity()
	{
		this->Vs.clear();
		this->Vs.shrink_to_fit();
	}

	card operator[] (int x)const
	{
		if (x < this->Vs.size())
		{
			return this->Vs.at(x);
		}
		else
		{
			// dummy card
			return card(1,'s');
		}
	}

};

