#pragma once
#include <iostream>

template<typename T>
class Array
{
private:
	T* array;
	size_t size;
	size_t capacity;

	void expand();//Strong exception guarantee

public:
	Array();
	Array(const Array<T>& other);
	Array(Array<T>&& other)noexcept;
	Array<T>& operator=(const Array<T>& other);
	Array<T>& operator=(Array<T>&& other)noexcept;
	~Array();
	void push_back(const T& element);//Strong exception guarantee
	void pop_back()noexcept;
	void remove(const size_t& index);//remove changes current elements order
	const T& at(const size_t& i)const;
	T& at(const size_t& i);
	T& operator[](const size_t& i);
	const T& operator[](const size_t& i)const;
	bool empty()const noexcept;
	void clear()noexcept;
	size_t getSize()const noexcept;
};


template<typename T>
Array<T>::Array() :array(nullptr), size(0), capacity(0){}

template<typename T>
Array<T>::Array(const Array<T>& other) :array(nullptr), size(other.size), capacity(other.capacity)
{
	if (other.capacity > 0)
	{
		this->array = (T*) operator new(other.capacity*sizeof(T));
		for (size_t i = 0; i < other.size;i++)
			new(array + i) T(other.array[i]);
	}
}

template<typename T>
Array<T>::Array(Array<T>&& other)noexcept :array(other.array), size(other.size), capacity(other.capacity)
{
	other.size = 0;
	other.capacity = 0;
	other.array = nullptr;
}


template<typename T>
Array<T>& Array<T>::operator=(const Array<T>& other)
{
	if (this != &other)
	{
		if (other.capacity > 0)
		{
			T* temp = (T*) operator new(other.capacity * sizeof(T));
			for (size_t i = 0; i < other.size;i++)
				new(temp + i) T(other.array[i]);

			delete[]this->array;
			this->array = temp;
			this->size = other.size;
			this->capacity = other.capacity;
		}
		else
		{
			this->clear();
		}
	}
	return *this;
}

template<typename T>
Array<T>& Array<T>::operator=(Array<T>&& other)noexcept
{
	if (this != &other)
	{
		delete[] this->array;
		this->array = other.array;
		this->size = other.size;
		this->capacity = other.capacity;

		other.capacity = 0;
		other.size = 0;
		other.array = nullptr;
	}
	return *this;
}

template<typename T>
Array<T>::~Array()
{
	delete[] this->array;
}

template<typename T>
void Array<T>::expand()
{
	if (this->array != nullptr)
	{
		T* temp = (T*) operator new(capacity*2* sizeof(T));
		for (size_t i = 0;i < this->size;i++)
			new(temp + i) T(this->array[i]);

		delete[] this->array;
		this->array = temp;
		this->capacity *= 2;
	}
	else
	{
		T* temp = (T*) operator new(16*sizeof(T));
		this->array = temp;
		this->capacity = 16;
		this->size = 0;
	}
}

template<typename T>
void Array<T>::push_back(const T& element)
{
	//Strong exception guarantee
	if (size >= capacity) this->expand();//expand supports Strong exception guarantee
	new(array + size) T(element);
	size++;
}

template<typename T>
void Array<T>::pop_back()noexcept
{
	if (!size) throw std::logic_error("nothing to remove");
	size--;
}

template<typename T>
void Array<T>::remove(const size_t& index)
{
	if (index >= this->size) throw std::out_of_range("too large index");
	this->array[index] = std::move(this->array[size - 1]);
	size--;
}

template<typename T>
const T& Array<T>::at(const size_t& i)const
{
	if (i >= this->size) throw std::out_of_range("too large index");
	return this->array[i];
}

template<typename T>
T& Array<T>::at(const size_t& i)
{
	if (i >= this->size) throw std::out_of_range("too large index");
	return this->array[i];
}

template<typename T>
T& Array<T>::operator[](const size_t& i)
{	//assume that this function will be called correctly
	return this->array[i];
}

template<typename T>
const T& Array<T>::operator[](const size_t& i)const
{
	//assume that this function will be called correctly
	return this->array[i];
}

template<typename T>
bool Array<T>::empty()const noexcept
{
	return size==0;
}

template<typename T>
void Array<T>::clear()noexcept
{
	delete[] this->array;
	this->array = nullptr;
	this->size = 0;
	this->capacity = 0;
}

template<typename T>
size_t Array<T>::getSize()const noexcept
{
	return this->size;
}
