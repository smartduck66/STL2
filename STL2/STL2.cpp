//
// Book : chapitre 21 de l'ouvrage
// "Programming -- Principles and Practice Using C++" de Bjarne Stroustrup (2ème édition : 2014)
// Commit initial : 13/08/2018 
// Commit en cours : 13/08/2018 - Drill de la page 798
// Caractères spéciaux : [ ]   '\n'   {  }   ||   ~   _     @

// Librairie indispensable sous Windows et VC++
#include "stdafx.h"

#include<iostream>
#include<iomanip>
#include<fstream>
#include<sstream>
#include<vector>
#include<string>
#include<array>
#include<list>
#include<algorithm>		// pour find
#include<iterator>
#include<numeric>
#include<unordered_map>
#include "MyHeader.h"

using namespace std;

struct Item {  
	string name;
	int iid;
	double value;
	
};

void print_item (vector<Item>&v)
{  
	for (auto x : v)
		cout << x.name << '-' << x.iid << '-' << x.value << endl;
	cout << "------------------------------------------------------------------" << endl;

}

void print_map(map <string, int>&m)
{
	for (auto x : m)
		cout << x.first << '-' << x.second <<  endl;
	cout << "------------------------------------------------------------------" << endl;

}

int accu_map(map <string, int>&m)
{
	int sum = 0;
	for (auto x : m)
		sum += x.second;
	return sum;

}

double weighted_value(const pair<string,double>& a, const pair<string,double>& b)
{  
	return a.second*b.second;
}


int main()
{
	try
	{
		// A mettre dans la 1ère ligne du "main" pour afficher les caractères accentués sous la console Windows
		std::locale::global(std::locale("fr-FR"));

		// Drill page 798 - partie vecteur I (list identique)
		vector<Item> vi{};
		string filename = "values.txt";
		ifstream ist{ filename };
		ist.exceptions(ist.exceptions() | ios_base::badbit);
		if (!ist)error("Impossible d'ouvrir le fichier ", filename);

		string s{};
		string name{};
		int iid = 0;
		double value = 0.0;
		while (!ist.eof()) {					// Tant que la fin du fichier n'est pas atteinte, on continue à extraire de nouveaux enregistrements
			getline(ist, s);					// On lit une ligne terminée par un NEWLINE
			stringstream ss(s);
			ss >> name >> iid >> value;
			vi.push_back(Item{name,iid,value});
		}

		ist.close();							// Fermeture du fichier
		
		vi.insert(vi.end(), Item{ "Horse shoe", 99, 12.34 });
		vi.insert(vi.end(), Item{ "Canon S400", 9911, 1500.65 });
		sort(vi.begin(), vi.end(), [](const Item& a, const Item&b) {return a.name < b.name; });		// Tri sur name
		print_item(vi);
		sort(vi.begin(), vi.end(), [](const Item& a, const Item&b) {return a.iid < b.iid; });		// Tri sur iid
		print_item(vi);
		sort(vi.begin(), vi.end(), [](const Item& a, const Item&b) {return a.value > b.value; });	// Tri sur value mais en decreasing
		print_item(vi);

		auto p = find_if(vi.begin(),vi.end(), [](const Item& a) {return a.name == "Panasonic1"; });	// Erase en partant d'un nom
		vi.erase(p); print_item(vi);

		p = find_if(vi.begin(), vi.end(), [](const Item& a) {return a.iid == 99; });				// Erase en partant d'une référence
		vi.erase(p); print_item(vi);

		// Drill page 798 - partie map
		map <string,int> msi{};
		msi.insert(make_pair("André", 52));	
		msi.insert(make_pair("Marc", 40));
		msi.insert(make_pair("Paul", 63));
		msi.insert(make_pair("Vivien", 19));
		msi.insert(make_pair("Pauline", 18));
		msi.insert(make_pair("Sylvie", 50));
		msi.insert(make_pair("Lionel", 41));
		msi.insert(make_pair("Everett", 26));
		msi.insert(make_pair("Jules", 80));
		msi.insert(make_pair("Alice", 78));
		print_map(msi);

		auto p1 = msi.find("André");																// Erase en partant d'un nom
		msi.erase(p1); print_map(msi);

		/*
		cout << "Saisir 3 couples de valeurs :" << endl;											// Insertion de valeurs saisies sur cin : bizarre -> Map non triée...
		string prénom{};
		int age = 0;
		for (int i = 0; i < 3; ++i)
			cin >> prénom >> age; msi.insert(make_pair(prénom, age));
		print_map(msi);
		*/
		cout << "La somme des âges est égale à " << accu_map(msi) << endl;							// Création d'une fonction car je n'arrive pas à utiliser accumulate sur une map

		map <string, int> mis{};																	// Copie
		mis = msi;
		print_map(mis);


		// Drill page 798 - partie vecteur II
		vector<double> vd{};
		string filename1 = "floating_values.txt";
		ifstream ist1{ filename1 };
		ist1.exceptions(ist1.exceptions() | ios_base::badbit);
		if (!ist1)error("Impossible d'ouvrir le fichier ", filename1);

		string sd{};
		while (!ist1.eof()) {					// Tant que la fin du fichier n'est pas atteinte, on continue à extraire de nouveaux enregistrements
			getline(ist1, sd);					// On lit une ligne terminée par un NEWLINE
			vd.push_back(stod(sd));
		}

		ist1.close();							// Fermeture du fichier
		
		vector<int> vi1{};
		for (auto x : vd)
			vi1.push_back(static_cast<int>(x));
		
		for (int unsigned i=0;i<size(vi1);++i)
			cout << vd[i] <<"      "<<vi1[i]<< endl;

		double sum = accumulate(vd.begin(), vd.end(), 0.0);
		double val_moyenne = sum / size(vd);
		int sum1= accumulate(vi1.begin(), vi1.end(), 0);
		cout << "La somme des valeurs du vecteur vd est égale à " << to_string(sum) << " et la valeur moyenne est "<<to_string(val_moyenne)<<endl;
		cout << "La somme différence entre vd et vi1 est égale à une perte de précision de " << to_string(sum-sum1) << endl;
		
		reverse(vd.begin(),vd.end());
		for (auto x : vd)
			cout << x<<endl;
		cout << "-------------------------------" << endl;

		vector<double>vd2{};
		for (auto x : vd) {
			if (x < val_moyenne) {
				vd2.push_back(x); 
				cout << x << endl;
			}
		
		}
		
		// Try this page 785 : map exemple w/ market values
		unordered_map <string, double>dow_price = {
			{"MMM",81.86},
			{"AA",34.69},
			{ "MO",54.45 }
				
		};

		unordered_map <string, double>dow_weight = {
			{ "MMM",5.8549 },
			{ "AA",2.4808 },
			{ "MO",3.8940 }

		};

		unordered_map <string, string>dow_name = {
			{ "MMM","3M Co" },
			{ "AA","Alcoa" },
			{ "MO","Altria Group" }

		};

		double alcoa_price = dow_price["AA"];
		for (const auto& p:dow_price) {
			const string& symbol = p.first;
			cout << symbol << '\t' << p.second << '\t' << dow_name[symbol] << endl;

		}

		double dji_index = inner_product(dow_price.begin(), dow_price.end(), dow_weight.begin(), 0.0, plus<double>(), weighted_value);
		cout << "Dow index = " << dji_index << endl;

		// Test stream iterators - page 791
		string from = "Using static Analysis_ACM_fev2010.txt";
		string to = "Using static Analysis_ACM_fev2010_copy.txt";
		
		ifstream is{from};
		ofstream os{to};

		istream_iterator<string> ii{is};
		istream_iterator<string> eos;
		ostream_iterator<string> oo{os,"\n"};

		vector<string>b{ii,eos};
		sort(b.begin(), b.end());
		copy(b.begin(),b.end(),oo);
		

		keep_window_open();

		return 0;
	}


	catch (runtime_error& e)
	{	// this code is to produce error messages
		cout << e.what() << '\n';
		keep_window_open("q");	// For some Windows(tm) setups
	}

	
}


