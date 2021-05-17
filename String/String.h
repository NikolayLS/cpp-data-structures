#pragma once
#include <iostream>


class String
{
	//strong exception guarantee
private:

	size_t size;
	size_t capacity;
	char* str;

	void expand();

public:
	String();
	String(const char*);
	String(const String&);
	String(String&&)noexcept;
	~String();
	String& operator=(const String&);
	String& operator=(String&&)noexcept;
	bool operator==(const String&)const noexcept;
	char& at(const std::size_t);
	const char& at(const std::size_t) const;
	char& operator[](const std::size_t);
	const char& operator[](const std::size_t) const;
	char& front();
	const char& front()const;
	size_t length()const noexcept;
	void push_back(const char);
	void clear()noexcept;
	friend std::ostream& operator<<(std::ostream&, const String&);
	friend std::istream& operator>>(std::istream&, String&);
	void pop_back();
	String& operator+=(char);
	String& operator+=(const String&);
	String operator+(char) const;
	String operator+(const String&) const;
	bool operator<(const String&) const;
};
