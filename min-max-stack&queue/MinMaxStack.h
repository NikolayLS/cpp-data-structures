#pragma once
#include <iostream>
#include<stack>

template<typename T>
class MinMaxStack
{
	std::stack<T> m_stack;
	std::stack<T> m_currMin;
	std::stack<T> m_currMax;
	//space complexity is still O(n)
public:
	void push(const T& element); //O(1)
	void pop(); //O(1)
	const T& top()const; //O(1)
	bool empty()const;
	const T& min()const; //O(1)
	const T& max()const; //O(1)
};

template<typename T>
void MinMaxStack<T>::push(const T& element)
{
	if (m_stack.empty())
	{
		m_stack.push(element);
		m_currMax.push(element);
		m_currMin.push(element);
	}
	else
	{
		m_stack.push(element);
		m_currMin.push(std::min(element, m_currMin.top()));
		m_currMax.push(std::max(element, m_currMax.top()));
	}
}

template<typename T>
void MinMaxStack<T>::pop()
{
	if (m_stack.empty()) throw std::length_error("Nothing to pop");
	m_stack.pop();
	m_currMin.pop();
	m_currMax.pop();
}

template<typename T>
const T& MinMaxStack<T>::top()const
{
	if (m_stack.empty()) throw std::length_error("Nothing on top");
	return m_stack.top();
}

template<typename T>
const T& MinMaxStack<T>::min()const
{
	if (m_stack.empty()) throw std::length_error("Empty stack");
	return m_currMin.top();
}

template<typename T>
const T& MinMaxStack<T>::max()const
{
	if (m_stack.empty()) throw std::length_error("Empty stack");
	return m_currMax.top();
}

template<typename T>
bool MinMaxStack<T>::empty()const
{
	return m_stack.empty();
}
