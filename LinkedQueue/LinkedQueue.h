#pragma once
#include<iostream>

template <typename T>
struct Node
{
	T data;
	Node* next;

	Node(T data) :data(data), next(nullptr)
	{}
};

template<typename T>
class LinkedQueue
{
private:
	Node<T>* firstAdded;
	Node<T>* lastAdded;

public:
	LinkedQueue();
	LinkedQueue(const LinkedQueue& other);
	LinkedQueue(LinkedQueue&& other)noexcept;
	LinkedQueue& operator=(const LinkedQueue& other);
	LinkedQueue& operator=(LinkedQueue&& other)noexcept;
	~LinkedQueue();
	void push(const T& element);
	void pop();
	T& front();
	const T& front()const;
	void clear();
	bool empty();
};


template<typename T>
LinkedQueue<T>::LinkedQueue() :firstAdded(nullptr), lastAdded(nullptr)
{}

template<typename T>
LinkedQueue<T>::LinkedQueue(const LinkedQueue<T>& other) : firstAdded(nullptr), lastAdded(nullptr)
{
	if (other.firstAdded != nullptr)
	{
		Node<T>* tempFst = new Node<T>(other.firstAdded->data);
		Node<T>* tempLast = tempFst;

		Node<T>* iterOther = other.firstAdded->next;
		while (iterOther != nullptr)
		{
			tempLast->next = new Node<T>(iterOther->data);
			tempLast = tempLast->next;
			iterOther = iterOther->next;
		}
		this->firstAdded = tempFst;
		this->lastAdded = tempLast;
	}
}

template<typename T>
LinkedQueue<T>::LinkedQueue(LinkedQueue<T>&& other)noexcept : firstAdded(other.first), lastAdded(other.last)
{
	other.first = nullptr;
	other.last = nullptr;
}

template<typename T>
LinkedQueue<T>& LinkedQueue<T>::operator=(const LinkedQueue<T>& other)
{
	if (this != &other)
	{
		if (other.firstAdded != nullptr)
		{
			Node<T>* tempFst = new Node<T>(other.firstAdded->data);
			Node<T>* tempLast = tempFst;

			Node<T>* iterOther = other.firstAdded->next;
			while (iterOther != nullptr)
			{
				tempLast->next = new Node<T>(iterOther->data);
				tempLast = tempLast->next;
				iterOther = iterOther->next;
			}

			this->clear();
			this->firstAdded = tempFst;
			this->lastAdded = tempLast;
		}
		else
		{
			this->clear();
			this->firstAdded = nullptr;
			this->lastAdded = nullptr;
		}
	}
	return *this;
}
template<typename T>
LinkedQueue<T>& LinkedQueue<T>::operator=(LinkedQueue<T>&& other) noexcept
{
	if (this != &other)
	{
		this->clear();
		this->firstAdded = other.firstAdded;
		this->lastAdded = other.lastAdded;

		other.firstAdded = nullptr;
		other.lastAdded = nullptr;
	}
	return *this;
}

template<typename T>
LinkedQueue<T>::~LinkedQueue()
{
	clear();
}

template<typename T>
void LinkedQueue<T>::push(const T& element)
{
	Node<T>* temp = new Node<T>(element);
	
	if(this->firstAdded == nullptr)
	{
		this->firstAdded = temp;
		this->lastAdded = temp;
	}
	else
	{
		this->lastAdded->next = temp;
		this->lastAdded = this->lastAdded->next;
	}
}

template<typename T>
void LinkedQueue<T>::pop()
{
	if (this->firstAdded == this->lastAdded)
	{
		delete this->firstAdded;
		this->firstAdded = nullptr;
		this->lastAdded = nullptr;
	}
	else
	{
		Node<T>* forDelete = this->firstAdded;
		this->firstAdded = this->firstAdded->next;
		delete forDelete;
	}
}

template<typename T>
T& LinkedQueue<T>::front()
{
	return this->firstAdded->data;
}

template<typename T>
const  T& LinkedQueue<T>::front()const
{
	return this->firstAdded->data;
}

template<typename T>
void LinkedQueue<T>::clear()
{
	while(this->firstAdded != nullptr)
	{
		pop();
	}
}

template<typename T>
bool  LinkedQueue<T>::empty()
{
	return this->firstAdded == nullptr;
}