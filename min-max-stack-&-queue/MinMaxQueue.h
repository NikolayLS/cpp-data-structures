#pragma once
#include "MinMaxStack.h"

template<typename T>
class MinMaxQueue
{
	MinMaxStack<T> m_qSemantic;
	MinMaxStack<T> m_stack;
	//space complexity is still O(n)

	void oneToAnother(); //transfer all elements from first stack to second
public:
	void push(const T& element);//O(1)
	const T& front(); //amortized analysised time complexity O(1)
	void pop(); //amortized analysised time complexity O(1)
	bool empty();
	const T& min();//O(1)
	const T& max();//O(1)
};

template<typename T>
void MinMaxQueue<T>::oneToAnother()
{
	if (m_stack.empty()) throw std::length_error("Empty queue");
	while (!m_stack.empty())
	{
		m_qSemantic.push(m_stack.top());
		m_stack.pop();
	}
}

template<typename T>
void MinMaxQueue<T>::push(const T& element)
{
	m_stack.push(element);
}

template<typename T>
const T& MinMaxQueue<T>::front()
{
	if (m_qSemantic.empty()) oneToAnother(); //transfer all elements from first stack to second
	return m_qSemantic.top();
}


template<typename T>
void  MinMaxQueue<T>::pop()
{
	if (m_qSemantic.empty()) oneToAnother(); //transfer all elements from first stack to second
	return m_qSemantic.pop();
}

template<typename T>
bool  MinMaxQueue<T>::empty()
{
	return m_stack.empty() && m_qSemantic.empty();
}

template<typename T>
const T& MinMaxQueue<T>::min()
{
	if (m_stack.empty() && m_qSemantic.empty()) throw std::length_error("Empty queue");
	if (m_stack.empty()) return m_qSemantic.min();
	if (m_qSemantic.empty()) return m_stack.min();
	return std::min(m_stack.min(), m_qSemantic.min());
}

template<typename T>
const T& MinMaxQueue<T>::max()
{
	if (m_stack.empty() && m_qSemantic.empty()) throw std::length_error("Empty queue");
	if (m_stack.empty()) return m_qSemantic.max();
	if (m_qSemantic.empty()) return m_stack.max();
	return std::max(m_stack.max(), m_qSemantic.max());
}
