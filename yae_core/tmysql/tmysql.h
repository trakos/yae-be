#include <mysql/mysql.h>
#include <string>

using std::string;

enum selectResultType { BOOLEAN, INT, LONG, UINT, ULONG, STRING };

class tmysql;
class selectFields;
class selectResult;
class selectResultRow;

struct selectField
{
	selectResultType type;
	string select;
	string as;
};

class tmysql
{
	public:
	 tmysql( const char* host, const char* user, const char* passwd, const char* db="", int port=0 );
	 ~tmysql();
	 int query( const char* q );
	 selectResult& select( selectFields& sf, const char* tableName, const char* params );
	 void freeResult( selectResult& sr );
	 unsigned long escape( char* from, char* to, unsigned long length ); // to musi byc zaalokowane na length*2+1 !
	 unsigned long long lastid( );
	protected:
	 tmysql( tmysql& m ) {}; //trzeba by bylo nowe polaczenie otworzyc, a nie zapisuje host/user/passwd itp, wiec daj znac jakby by³o potrzebne
	 MYSQL* conn;
};

class selectFields
{
	public:
	 selectFields( unsigned int size );
	 selectFields( const selectFields& sf );
	 ~selectFields();
	 void add( selectResultType type, const char* select, const char* as );
	 void add( selectResultType type, const char* select );
	 selectField& operator[] (unsigned int i);
	 unsigned int getCurrentSize();
	 operator string() const;
	protected:
	 int added;
	 int limit;
	 selectField* array; 
};

class selectResult
{
	public:
	 selectResult( selectFields& sf, char*** rows, unsigned int _size );
	 ~selectResult();
	 selectResultRow& operator[] (unsigned int i);
	 unsigned int getSize();
	protected:
	 selectResult( const selectResult& sr ):selectedFields(0) {}; // kopiowanie raczej nie by³oby op³acalne - chociaz w razie czego mozna dodac
	 selectResultRow** resultsArray;
	 unsigned int size;
	 selectFields selectedFields;
};

class selectResultRow
{
	public:
	 selectResultRow( selectFields* sfs, char** row ); 
	 ~selectResultRow();
	 void* operator[] (const char* what);
	 void* operator[] (unsigned int which);
	protected:
	 selectResultRow(  const selectResultRow& srr ) {}; // nieintucyjne by³oby kopiowanie tego obiektu
	 void* convertValue( selectResultType type, char* value );
	 void** fieldsArray;
	 unsigned int size;
	 selectFields* selectedFields;
};





