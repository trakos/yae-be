#ifndef ARRAY_H
	#include "array.h"
#endif

#include <limits.h>
#include <stdio.h>

namespace t
{

template <typename T> array<T>::array( const T& _default_value ):default_value(_default_value)
{
	init(1);
}

template <typename T> array<T>::array( )
{
	init(1);
}


template <typename T> array<T>::array(index size, const T& _default_value):default_value(_default_value)
{
	init(size);
}

template <typename T> array<T>::array( array<T> const& src ):default_value(src.default_value)
{
	init(src.current_size);
	for( index nr=0; nr<src.used_size; nr++ )
	{
		if(src.isset(nr))
		{
			set(nr, src[nr]);
		}
	}
	used_size = src.used_size;
}

template <typename T> array<T>& array<T>::operator= (const array<T>& src)
{
	default_value = src.default_value;
	clear();
	init(src.current_size);
	for( index nr=0; nr<src.current_size; nr++ )
	{
		if(src.isset(nr))
		{
			set(nr, src[nr]);
		}
	}
	used_size = src.used_size;
}

template <typename T> void array<T>::init(const index size)
{
	data = 0;
	used_size = 0;
	current_size = 0;
	set_size(size);
}

template <typename T> array<T>::~array()
{
	clear();
}

template <typename T> const T& array<T>::get(const index nr) const
{
	if( nr >= current_size )
	{
		//printf("trying to resize array through constant method\n");
		throw 1; //todo: trying to resize array through constant method
	}
	if(!isset(nr))
	{
		//printf("trying to get unitialized value with const method (so we can't initalize it with default value\n");
		throw 2; //todo: trying to get unitialized value with const method (so we can't initalize it with default value)
	}
        return *(data[nr]);
}


template <typename T> T& array<T>::get(const index nr)
{
	adjust_size(nr+1);
	if(!isset(nr))
	{
		set(nr, default_value);
	}
	return *(data[nr]);
}

template <typename T> const T& array<T>::operator[](const index nr) const
{
	return get(nr);
}

template <typename T> T& array<T>::operator[](const index nr)
{
	return get(nr);
}

template <typename T> bool array<T>::operator== ( const array<T>& compare_to ) const
{
	if( get_size() != compare_to.get_size() )
	{
		return false;
	}
	for( index nr=0; nr<get_size(); nr++ )
	{
		if( isset(nr) != compare_to.isset(nr) )
		{
			return false;
		}
		if( isset(nr) && get(nr) != compare_to[nr] )
		{
			return false;
		}
	}
	return true;
}


inline bool operator== ( const array<char>& arr, const char*& compare_to )
{
	for( index nr=0; nr<arr.get_size(); nr++ )
	{
		if( compare_to[nr] == '\0' )
		{
			return false;
		}
		else if( !arr.isset(nr) )
		{
			return false;
		}
		else if( arr[nr] != compare_to[nr] )
		{
			return false;	
		}
	}
	return true;
}

template <typename T> void array<T>::set(const index nr, T value)
{
	adjust_size(nr+1);
	if( !isset(nr) )
	{
			data[nr] = new T;
	}
	*(data[nr]) = value;
	if ( nr >= used_size )
	{
		used_size = nr+1;
	}
}

template <typename T> void array<T>::set_size(const index size)
{
	if( size > current_size )
	{
		T** newdata;
		newdata = new T*[size];
		index nr=0;
		for(; nr<used_size; nr++)
		{
			newdata[nr] = data[nr];
		}
		for(; nr<size; nr++)
		{
			newdata[nr] = 0;
		}
		if(data)
		{
			delete[] data;
			data = 0;
		}
		data = newdata;
		current_size = size;
	}
	else
	{
		throw 69;
	}
}

template <typename T> void array<T>::adjust_size(const index size)
{
	if( current_size < size )
	{
		int c2 = current_size*2;
		set_size(c2>size?c2:size); //TODO: use max here
	}
}

template <typename T> index array<T>::get_size() const
{
	return used_size;
}

template <typename T> bool array<T>::isset(const index nr) const
{
	return ( nr >= 0 && nr < used_size && data[nr] );
}

template <typename T> void array<T>::unset(index nr)
{
	//printf("unsetting %d: %d/%d\n", nr,this->used_size,this->current_size);
	if( nr < 0 || nr >= current_size )
	{
		//printf("\nunset: %d out of bounds... used size: %d; current size :%d\n", nr, this->used_size, this->current_size);
		throw 3; //todo
	}
	if(data[nr])
	{
		delete data[nr];
		data[nr] = 0;
		//printf("used_size: %d\n", used_size);
		//printf("beforewhile nr: %d\n", nr);
		while( nr+1 < used_size && data[nr+1] )
		{
			data[nr] = data[nr+1];
			data[nr+1] = 0;
			nr++;
		}
		//printf("used_size: %d\n", used_size);
		//printf("beforefor nr: %d\n", nr);
		// podchwyt w tej petli polega na tym, ze nr to unsigned int, wiec nie moze siegnac -1 - stad ten if/else ponizej
		for( nr = used_size-1; nr > 0 && !data[nr]; nr-- )
		{
			//printf("petla: %u, nr>=0: %d\n", nr, nr>0?1:0);
		}
		if( nr == 0 && !data[nr] )
		{
			used_size=0;
		}
		else
		{
			used_size=nr+1;
		}
		//printf("petla: %u, nr>=0: %d\n", nr, nr>=0?1:0);
		//printf("afterfornr: %d\n", nr);
	}
	//printf("done, used_size/current_size: %d/%d\n", this->used_size,this->current_size);
}

template <typename T> void array<T>::clear()
{
	for(index nr=0; nr<this->current_size; nr++)
	{
		//this->unset(nr);
		if ( this->data && this->data[nr] )
		{
			delete this->data[nr];
		}
	}
	if(this->data)
	{
		delete[] this->data;
		this->data = 0;
	}
	this->used_size = 0;
	this->current_size = 0;
}

template <typename T> void array<T>::print_r(int level) const 
{
	for(int l=0; l<level; l++) printf("\t");
	printf("Array\n");
	for(int l=0; l<level; l++) printf("\t");
	printf("(\n");
	for( index nr=0; nr<current_size; nr++ )
	{
		if( isset(nr) )
		{
			for(int l=0; l<level; l++) printf("\t");
			printf("\t[%d] => ", nr);
			get(nr).print_r(level+1);
		}
	}
	for(int l=0; l<level; l++) printf("\t");
	printf(")\n");
}

template <> inline void array<int>::print_r(int level) const
{
        for(int l=0; l<level; l++) printf("\t");
        printf("Array\n");
        for(int l=0; l<level; l++) printf("\t");
        printf("(\n");
        for( index nr=0; nr<current_size; nr++ )
        {
                if( isset(nr) )
                {
                        for(int l=0; l<level; l++) printf("\t");
                        printf("\t[%d] => %d\n", nr, get(nr));
                }
        }
        for(int l=0; l<level; l++) printf("\t");
        printf(")\n");
}

template <> inline void array<char>::print_r(int level) const
{
/*        for(int l=0; l<level; l++) printf("\t");
        printf("Array\n");
        for(int l=0; l<level; l++) printf("\t");
        printf("(\n");
        for( index nr=0; nr<current_size; nr++ )
        {
                if( isset(nr) )
                {
                        for(int l=0; l<level; l++) printf("\t");
                        printf("\t[%d] => %c\n", nr, get(nr));
                }
        }
        for(int l=0; l<level; l++) printf("\t");
        printf(")\n");*/
	char* dest = new char[get_size()+1];
	dest[0] ='\0';
	int c;
        for(c=0; c<get_size(); c++ )
        {
                dest[c] = get(c);
        }
        dest[c] = '\0';
	printf("%s\n", dest);
	delete[] dest;
}



template <typename T> array<array<T> > array<T>::explode(T delim, int limit) const
{
	array<array<T> > results;
	index start=0;
	array<T> ar(1,default_value);
	index res=0;
	for( index nr=0; nr < used_size; nr++ )
	{
		if(isset(nr))
		{
			if( limit && get(nr) == delim )
			{
				if(ar.get_size())
				{
					results[res++] = ar;
					limit--;
				}
				ar.clear();
				start = nr+1;
			}
			else
			{
				ar[nr-start] = get(nr);
			}
		}
	}
	if(ar.get_size())
	{
		results[res] = ar;
	}
	return results;
}

template <typename T> void array<T>::merge(const array<T>& merge_with)
{
	index nr_here = used_size;
	for( index nr_there=0; nr_there < merge_with.get_size(); nr_there++ )
	{
		if( merge_with.isset(nr_there) )
		{
			set(nr_here, merge_with[nr_there]);
			nr_here++;
		}
	}
}

}
