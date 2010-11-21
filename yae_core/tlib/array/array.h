#pragma once

//todo: pthread-safe (mainly setting size)
#include <limits.h>

namespace t
{
	typedef unsigned int index;

template <typename T>
class array
{
	public:
		array();
		array(const T& default_value);
		array(const index size);
		array(const index size, const T& default_value);
		array( array<T> const& src );
		array<T>& operator= (const array<T>& src);
		~array();
		T& get(const index nr);
		const T& get(const index nr) const;
		void set(const index nr, const T value);
		const T& operator[](const index nr) const;
		T& operator[](const index nr);
		friend bool operator== ( const array<char>& arr, const char*& compare_to );
		bool operator== ( const array<T>& compare_to ) const;
		void set_size(const index size);
		void adjust_size(const index size);
		index get_size() const;
		bool isset(const index nr) const;
		void unset(index nr);
		void clear();

		array< array<T> > explode(T delim, int limit=INT_MAX) const;
		void merge(const array<T>& merge_with);
		
		void print_r(int level=0) const;

		T default_value;
	protected:
		void init(const index size);
		index current_size;
		index used_size;
		T** data;
};


}

#define ARRAY_H
#include "array.cpp"

