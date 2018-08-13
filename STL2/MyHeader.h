/*
MyHeader.h
*/

/*
first own header

Revised July 21, 2018: initial release
Revised August 3, 2018: function prototypes added

*/

#ifndef MY_HEADER_GUARD	// Pour �viter que mon header soit compil� deux fois
#define MY_HEADER_GUARD

#include<string>
#include<iostream>
#include<vector>
#include<map>	// pour b�n�ficier du container multimap

using namespace std;

inline void keep_window_open() // From "std_lib_facilities.h" of BS
{
	cin.clear();
	cout << "Please enter a character to exit\n";
	char ch;
	cin >> ch;
	return;
}

inline void keep_window_open(string s)
{
	if (s == "") return;
	cin.clear();
	cin.ignore(120, '\n');
	for (;;) {
		cout << "Please enter " << s << " to exit\n";
		string ss;
		while (cin >> ss && ss != s)
			cout << "Please enter " << s << " to exit\n";
		return;
	}
}

struct Exit : runtime_error {
	Exit() : runtime_error("Exit") {}
};

// error() simply disguises throws:
inline void error(const string& s)
{
	throw runtime_error(s);
}

inline void error(const string& s, const string& s2)
{
	error(s + s2);
}

int nb_aleatoire(const int min, const int max)
{
	// Cette fonction helper g�re le tirage de nombres al�atoires entre deux bornes pass�es en arguments
	// On pr�f�re la fonction C rand() � randint(min, max) incluse dans std_lib_facilities.h
	// Le seeding doit �tre r�alis� dans le main() et permet une g�n�ration pseudo-al�atoire (meilleurs outils � partir de C++ 11)
	// srand((int)time(0));	// Seeding du moteur de g�n�ration al�atoire en partant de l'heure
	int a = rand() % max + min;
	return a;
}


#endif