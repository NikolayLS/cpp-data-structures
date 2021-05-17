#include "String.h"

String::String() :size(0), capacity(0), str(nullptr)
{}

String::String(const char* other) : size(0), capacity(0), str(nullptr)
{
	if (other != nullptr)
	{
		size_t otherLen = 0;
		size_t otherCap = 16;

		while (other[otherLen] != '\0')
		{
			otherLen++;
			if (otherLen > otherCap) otherCap *= 2;
		}

		if (otherLen != 0)
		{
			this->str = new char[otherCap];
			this->size = otherLen;
			this->capacity = otherCap;
			for (size_t i = 0;i < this->size;i++)
				this->str[i] = other[i];
		}
	}
}

void String::expand()
{
	//strong exception guarantee
	if (str == nullptr)
	{
		char* temp = new char[16];
		str = temp;
		this->size = 0;
		this->capacity = 16;
	}
	else
	{
		char* temp = new char[capacity * 2];

		for (size_t i = 0;i < size;i++)
			temp[i] = this->str[i];

		delete[] this->str;
		this->str = temp;
		capacity *= 2;
	}
}

String::String(const String& other) : size(0), capacity(0), str(nullptr)
{
	if (other.size > 0)
	{
		this->str = new char[other.capacity];
		for (size_t i = 0;i < other.capacity && i < other.size; i++)
			this->str[i] = other.str[i];

		this->size = other.size;
		this->capacity = other.capacity;
	}
}

String::String(String&& other)noexcept :size(0), capacity(0), str(other.str)
{
	other.capacity = 0;
	other.size = 0;
	other.str = nullptr;
}
String::~String()
{
	delete[] str;
}

String& String::operator=(const String& other)
{
	//strong exception guarantee
	if (this != &other)
	{
		if (other.size == 0)
		{
			delete[] this->str;
			this->str = nullptr;
			this->size = 0;
			this->capacity = 0;
		}
		else
		{
			char* temp = new char[other.capacity];
			for (size_t i = 0; i < other.capacity && i < other.size;i++)
			{
				temp[i] = other.str[i];
			}

			delete[] this->str;
			this->str = temp;
			this->size = 0;
			this->capacity = 0;
		}
	}
	return *this;
}

String& String::operator=(String&& other)noexcept
{
	if (this != &other)
	{
		delete[] this->str;
		this->str = other.str;
		this->size = other.size;
		this->capacity = other.capacity;

		other.str = nullptr;
		other.size = 0;
		other.capacity = 0;
	}
	return *this;
}

bool String::operator==(const String& other)const noexcept
{
	if (this->size == 0 && other.size == 0) return true;
	if (this->size != other.size) return false;

	for (size_t i = 0;i < this->size;i++)
	{
		if (this->str[i] != other.str[i])return false;
	}
	return true;
}
char& String::at(const std::size_t index)
{
	if (index >= size) throw std::out_of_range("wrong index");
	return this->str[index];
}

const char& String::at(const std::size_t index) const
{
	if (index >= size) throw std::out_of_range("wrong index");
	return this->str[index];
}

char& String::operator[](const std::size_t index)
{
	//assume that this operator will be called correctly
	return this->str[index];
}

const char& String::operator[](const std::size_t index) const
{
	//assume that this operator will be called correctly
	return this->str[index];
}

char& String::front()
{
	//assume that this operator will be called correctly
	return this->str[0];
}
const char& String::front()const
{
	//assume that this operator will be called correctly
	return this->str[0];
}

size_t String::length()const noexcept
{
	return this->size;
}

void String::push_back(const char element)
{
	//strong exception guarantee
	if (this->size >= this->capacity) this->expand();
	this->str[this->size] = element;
	this->size++;
}

void String::clear()noexcept
{
	delete[] this->str;
	this->str = nullptr;
	this->size = 0;
	this->capacity = 0;
}

std::ostream& operator<<(std::ostream& os, const String& str)
{
	for (size_t i = 0;i < str.size;i++)
		os << str[i];
	return os;
}

std::istream& operator>>(std::istream& is, String& thisString)
{
	const std::istream::sentry tempSentry(is);
	if (!tempSentry) return is;
	thisString.clear();

	try
	{
		int ch = is.peek();
		while (ch != EOF && !isspace(static_cast<unsigned char>(ch)))
		{   //this is check for setw(int) function for input streams
			if (thisString.size >= is.width() && is.width()) break;

			thisString.push_back(is.get());
			ch = is.peek();
		}
	}
	//only our push_back can throw exception - we need to catch only bad_alloc
	//we need to throw the thrown exception to the use if he wants
	catch (std::bad_alloc& ourExcept)
	{
		try
		{
			is.setstate(std::ios_base::badbit);
		}
		catch (...)
		{
			// must set back width to 0 - this is the way it must work
			// setw(int) must works only for the first stream after changing it
			is.width(0);
			throw ourExcept;
		}
	}
	// must set back width to 0 - this is the way it must work
	// setw(int) must works only for the first stream after changing it
	is.width(0);
	return is;
}

void String::pop_back()
{
	//assert that the String is not empty;
	this->size--;
}

String& String::operator+=(char c)
{
	//strong exception guarantee
	this->push_back(c);
	return *this;
}

String& String::operator+=(const String& other)
{
	if (other.str != nullptr)
	{
		for (size_t i = 0;i < other.size;i++)
			this->push_back(other[i]);
	}
	return *this;
}

String String::operator+(char ch) const
{
	String temp(*this);
	temp += ch;

	return temp;//move constructor called
}

String String::operator+(const String& other) const
{
	String temp(*this);
	temp += other;

	return temp;//move constructor called
}

bool String::operator<(const String& other) const
{
	for (size_t i = 0; i < this->size && i < other.size;i++)
	{
		if (this->operator[](i) < other[i]) return true;
		if (this->operator[](i) > other[i]) return false;
	}

	if (this->size >= other.size) return false;
	return true;
}