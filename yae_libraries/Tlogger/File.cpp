#include <stdexcept>
#include <fcntl.h>
#include <sys/stat.h>
#include <cstring>
#ifdef __unix__
	#include <errno.h>
#endif

#include <Tlogger/File.h>

using namespace std;

Tlogger_File::Tlogger_File(string file, string dir)
{
	if (!setfile(file, dir))
	{
		throw(runtime_error("Nie udało się rozpocząć logowania!"));
	}
}

bool Tlogger_File::setfile(string file, string dir)
{
	string srcfile = dir + "/" + file;
	if (access(srcfile.c_str(), F_OK) == 0)
	{
		if (access(dir.c_str(), W_OK) == 0)
		{
			cout << "setfile(): Plik istnieje, rozpoczynamy przesuwanie plików." << endl;
			// szukamy pierwszego pliku którego nie ma
			int i;
			for (i = 0; i < 10; i++)
			{
				string testfile = dir + "/" + file + "." + i;
				if (access(testfile.c_str(), F_OK) != 0)
					break;
			}
			// jezeli wszystkie sa to 9 po prostu usuwamy
			if (i == 10)
			{
				string delfile = dir + "/" + file + ".9";
				if (remove(delfile.c_str()) != 0)
				{
					cerr << "setfile(): Błąd przy usuwaniu " << delfile << "!" << endl;
					cout << "setfile(): Zrzut errno: " << strerror(errno) << endl;
					return false;
				}
				i--;
			}
			// przesuwamy pliki po kolei od tylu
			for (i--; i >= 0; i--)
			{
				// przesun plik.i do plik.(i+1)
				string oldpath = srcfile + "." + i;
				string newpath = (srcfile + ".") + (i + 1);
				if (rename(oldpath.c_str(), newpath.c_str()) != 0)
				{
					cerr << "setfile(): Błąd przy przesuwaniu " << oldpath << " do " << newpath << "!" << endl;
					cout << "setfile(): Zrzut errno: " << strerror(errno) << endl;
					return false;
				}
			}
			// przesun plik do plik.0 - {} żeby nie trzymać potem oldpath i newpath
			{
				string oldpath = srcfile;
				string newpath = srcfile + ".0";
				if (rename(oldpath.c_str(), newpath.c_str()) != 0)
				{
					cerr << "setfile(): Błąd przy przesuwaniu " << oldpath << "do " << newpath << "!" << endl;
					cout << "setfile(): Zrzut errno: " << strerror(errno) << endl;
					return false;
				}
			}
			cout << "setfile(): Stare pliki poprzesuwane, tworzenie nowego..." << endl;
			//if (creat(srcfile.c_str(), S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH) == -1)
			if (creat(srcfile.c_str(), O_WRONLY | O_CREAT | O_TRUNC) == -1)
			{
				cerr << "setfile(): Błąd przy tworzeniu " << srcfile << "!" << endl;
				cout << "setfile(): Zrzut errno: " << strerror(errno) << endl;
				return false;
			}
			logfile.open(srcfile.c_str());
			cout << "setfile(): Plik loga otwarty." << endl;
		}
		else // nie ma praw zapisu do folderu
		{
			cerr << "setfile(): Niezdolny do zapisu do folderu " << dir << " !" << endl;
			cout << "setfile(): Zrzut errno: " << strerror(errno) << endl;
			if (access(srcfile.c_str(), W_OK) == 0)
			{
				cout << "setfile(): Otwieram już istniejący plik " << file << " !" << endl;
				logfile.open(srcfile.c_str());
				cout << "setfile(): Plik loga otwarty." << endl;
			}
			else
			{
				cout << "setfile():Niezdolny do zapisu do " << file << "!" << endl;
				cout << "setfile(): Zrzut errno: " << strerror(errno) << endl;
				return false;
			}
		}
	}
	else // nie istnieje plik
	{
		if (access(dir.c_str(), W_OK) == 0)
		{
			cout << "setfile(): Tworzenie pliku logowania..." << endl;
			//if (creat(srcfile.c_str(), S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH) == -1)
			if (creat(srcfile.c_str(), O_WRONLY | O_CREAT | O_TRUNC) == -1)
			{
				cerr << "setfile(): Błąd przy tworzeniu " << srcfile << "!"
						<< endl;
				cout << "exec(): Zrzut errno: " << strerror(errno) << endl;
				return false;
			}
			logfile.open(srcfile.c_str());
			cout << "setfile(): Plik loga otwarty." << endl;
		}
		else // nie ma praw zapisu do folderu
		{
			cout << "setfile(): Niezdolny do zapisu do " << dir << "!" << endl;
			cout << "exec(): Zrzut errno: " << strerror(errno) << endl;
			return false;
		}
	}
	// sprawdzenie czy plik się otworzył
	if (logfile.is_open())
	{
		return true;
	}
	else
	{
		cerr << "ERROR: żaden logfile nie jest otwarty!" << endl;
		return false;
	}
}

Tlogger_File::~Tlogger_File()
{
	close();
}

void Tlogger_File::close()
{
	if (logfile.is_open())
	{
		logfile.close();
	}
}

void Tlogger_File::operator()(string msg)
{
	if (logfile.is_open())
	{
		logfile << msg << endl;
	}
}
