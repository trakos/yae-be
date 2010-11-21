#include "tmysql.h"
#include <stdexcept>
#include <cstdlib>
#include <cstring>
#include <cstdio>


using std::string;
using std::runtime_error;

/* tmysql */
tmysql::tmysql( const char* host,  const char* user,  const char* passwd,  const char* db, int port )
{
	conn = mysql_init(NULL);
	if(!mysql_real_connect(conn,host,user,passwd,db,port,NULL,0))
	{
		throw runtime_error(mysql_error(conn));
	}
}

tmysql::~tmysql()
{
	mysql_close(conn);
}

int tmysql::query( const char* q )
{
	printf("QUERY: %s\n", q);
	return mysql_query(conn, q);
}

unsigned long long tmysql::lastid( )
{
	return mysql_insert_id(conn);
}

selectResult& tmysql::select( selectFields& sf,  const char* tableName,  const char* params )
{
	string q = "SELECT "+(string)sf+" FROM "+tableName+" "+params;
	if(this->query(q.c_str()))
	{
		throw runtime_error(mysql_error(conn));
	}
	MYSQL_RES* res = mysql_store_result(conn);
	unsigned int size = mysql_num_rows(res);
	char*** rows = new char**[size];
	for( int i=0; i < size; i++ )
	{
		rows[i] = mysql_fetch_row(res);
	}
	selectResult* srp = new selectResult( sf, rows, size );
	mysql_free_result(res);
	return *srp;
}

void tmysql::freeResult( selectResult& sf )
{
	delete &sf;
}

unsigned long tmysql::escape( char* from, char* to, unsigned long length )
{
	return mysql_real_escape_string(conn, to, from, length);
}

/* selectFields */
selectFields::selectFields( unsigned int size ): added(0), limit(size)
{
	array = new selectField[size];
}

selectFields::selectFields( const selectFields& sf ): added(sf.added), limit(sf.limit)
{
	array = new selectField[limit];
	for( int i=0; i<added; i++ )
	{
		array[i] = sf.array[i];
	}
}

selectFields::~selectFields( )
{
	delete[] array;
}

void selectFields::add( selectResultType type, const char* select, const char* as )
{
	if( added == limit )
	{
		throw runtime_error("Too much fields added to selectFields");
	}
	array[added].type = type;
	array[added].select = select;
	array[added].as = as;
	added++;
}

void selectFields::add( selectResultType type, const char* select )
{
	this->add( type, select, select );
}


selectField& selectFields::operator[] (unsigned int i)
{
	return array[i];
}

unsigned int selectFields::getCurrentSize()
{
	return added;
}

selectFields::operator string() const
{
	string s = string("");
	for( int i = 0; i < added; i++ )
	{
		if(i)
		{
			s+=",";
		}
		s+=array[i].select;
		if( array[i].as != "" )
		{
			s += " AS ";
			s += array[i].as;
		}
	}
	return s;
}

/* selectResult */
selectResult::selectResult( selectFields& sf, char*** rows, unsigned int _size ):size(_size),selectedFields(sf)
{
	resultsArray = new selectResultRow*[size];
	for( int i = 0; i < size; i++ )
	{
		resultsArray[i] = new selectResultRow( &sf, rows[i] );
	}
}

selectResult::~selectResult()
{
	for( int i = 0; i < size; i++ )
	{
		delete resultsArray[i];
	}
	delete[] resultsArray;
}

selectResultRow& selectResult::operator[] (unsigned int i)
{
	if( i >= size )
	{
		char s[100];
		sprintf(s, "Tried to get row %d, but only %d rows are present.", i+1, size);
		throw runtime_error(s);
	}
	return *(resultsArray[i]);
}

unsigned int selectResult::getSize()
{
	return size;
}

/* selectResultRow */
selectResultRow::selectResultRow( selectFields* sfs, char** row )
{
	selectedFields = sfs;
	size = sfs->getCurrentSize();
	fieldsArray = new void*[size];
	for ( int i = 0; i < size; i++ )
	{
		fieldsArray[i] = convertValue( (*sfs)[i].type, row[i] );
	}
}

selectResultRow::~selectResultRow()
{
	for ( int i = 0; i < size; i++ )
	{
		if( fieldsArray[i] )
		{
			switch( (*selectedFields)[i].type )
			{
				case BOOLEAN: delete (bool*)fieldsArray[i]; break;
				case INT: delete (int*)fieldsArray[i]; break; 
				case LONG: delete (long long int*)fieldsArray[i]; break;
				case UINT: delete (unsigned int*)fieldsArray[i]; break;
				case ULONG: delete (unsigned long long int*)fieldsArray[i]; break;
				case STRING: delete[] (char*) fieldsArray[i]; break;
			}
		}
	}
	delete[] fieldsArray;
}

void* selectResultRow::convertValue( selectResultType type, char* value )
{
	void* ret;
	if( value )
	{
		switch( type )
		{
			case BOOLEAN:	{bool* a = new bool; *a=(value=="0"); ret=a; break;}
			case INT:		{int* a = new int; *a=strtol(value, (char **)NULL, 10); ret=a; break; }
			case LONG: 	{long long int* a = new long long int; *a=strtoll(value, (char **)NULL, 10); ret=a; break;}
			case UINT: 	{unsigned int* a = new unsigned int; *a=strtoul(value, (char **)NULL, 10); ret=a; break;}
			case ULONG: 	{unsigned long long int* a = new unsigned long long int; *a=strtoull(value, (char **)NULL, 10); ret=a; break;}
			case STRING: 	{int d = strlen(value); char* a = new char[d+1]; strcpy(a,value); ret=a; break;}
		}
	}
	else
	{
		ret = NULL;
	}	
	return ret;
}

void* selectResultRow::operator[] (const char* what)
{
	for ( int i = 0; i < size; i++ )
	{
		if( (*selectedFields)[i].as == string(what) || ( (*selectedFields)[i].as == "" && (*selectedFields)[i].select == string(what)) )
		{
			return fieldsArray[i];
		}
	}
	char estr[100];
	sprintf(estr, "resultRow named %s not found", what);
	throw runtime_error(estr);
}

void* selectResultRow::operator[] (unsigned int which)
{
	if( which >= size )
	{
		char s[100];
		sprintf(s, "Tried to get field %d, but only %d fields are present.", which+1, size);
		throw runtime_error(s);
	}
	return fieldsArray[which];
}

