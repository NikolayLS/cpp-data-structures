#pragma once
#include <iostream>

template <typename T>
struct Node
{
	T data;
	Node* next;

	Node(const T& data) :data(data), next(nullptr)
	{}
};

template <typename T>
class LinkedStack
{
private:
	Node<T>* head;
public:
	LinkedStack();
	LinkedStack(const LinkedStack& other);
	LinkedStack(LinkedStack&& other)noexcept;
	LinkedStack& operator=(const LinkedStack& other); //strong exception guarantee
	LinkedStack& operator=(LinkedStack&& other)noexcept;
	~LinkedStack()noexcept;
	T& top();
	const T& top()const;
	void pop();
	void push(const T& element); //strong exception guarantee
	void clear()noexcept;
	bool empty()const noexcept;
};


template <typename T>
LinkedStack<T>::LinkedStack() : head(nullptr) {}

template <typename T>
LinkedStack<T>::LinkedStack(const LinkedStack<T>& other) : head(nullptr)
{
	if (other.head != nullptr)
	{
		auto temp = new Node<T>(other.head->data);
		Node<T>* iteratorOth = other.head->next;
		Node<T>* iteratorTemp = temp;

		while (iteratorOth != nullptr)
		{
			iteratorTemp->next = new Node<T>(iteratorOth->data);
			iteratorTemp = iteratorTemp->next;
			iteratorOth = iteratorOth->next;
		}
		this->head = temp;
	}
}

template <typename T>
LinkedStack<T>::LinkedStack(LinkedStack<T>&& other)noexcept : head(other.head)
{
	other.head = nullptr;
}

template<typename T>
LinkedStack<T>& LinkedStack<T>::operator=(const LinkedStack<T>& other)
{
	if (this != &other)
	{
		if (other.head != nullptr)
		{
			auto temp = new Node<T>(other.head->data);
			Node<T>* iteratorOth = other.head->next;
			Node<T>* iteratorTemp = temp;

			while (iteratorOth != nullptr)
			{
				iteratorTemp->next = new Node<T>(iteratorOth->data);
				iteratorTemp = iteratorTemp->next;
				iteratorOth = iteratorOth->next;
			}
			clear();
			this->head = temp;
		}
		else
		{
			clear();
			this->head = nullptr;
		}
	}
	return *this;
}

template<typename T>
LinkedStack<T>& LinkedStack<T>::operator=(LinkedStack&& other)noexcept
{
	if (this != &other)
	{
		this->clear();
		this->head = other.head;
		other.head = nullptr;
	}
	return *this;
}

template<typename T>
LinkedStack<T>::~LinkedStack()noexcept
{
	this->clear();
}

template<typename T>
T& LinkedStack<T>::top()
{
	if (this->head == nullptr) throw std::out_of_range("nothing on top");
	return this->head->data;
}

template<typename T>
const T& LinkedStack<T>::top()const
{
	if (this->head == nullptr) throw std::out_of_range("nothing on top");
	return this->head->data;
}

template <typename T>
void LinkedStack<T>::pop()
{
	Node<T>* forDelete = this->head;
	this->head = this->head->next;
	delete forDelete;
}

template<typename T>
void LinkedStack<T>::push(const T& element)
{
	auto temp = new Node<T>(element);
	temp->next = this->head;
	this->head = temp;
}

template <typename T>
void LinkedStack<T>::clear()noexcept
{
	while (head != nullptr)
		this->pop();
}

template<typename T>
bool LinkedStack<T>::empty()const noexcept
{
	return head == nullptr;
}