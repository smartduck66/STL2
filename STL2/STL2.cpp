//
// Book : chapitre 21 de l'ouvrage
// "Programming -- Principles and Practice Using C++" de Bjarne Stroustrup (2ème édition : 2014)
// Commit initial : 13/08/2018 
// Commit en cours : 16/08/2018 - refactoring avant vf
// Caractères spéciaux : [ ]   '\n'   {  }   ||   ~   _     @

// Librairie indispensable sous Windows et VC++
#include "stdafx.h"

#include<iostream>
#include<fstream>
#include<sstream>
#include<vector>
#include<string>
#include<array>
#include<list>
#include<algorithm>		// pour find
#include<numeric>
#include<map>
#include<unordered_map>
#include<set>
#include "MyHeader.h"

using namespace std;

struct Item {  
	string name{};
	int iid=0;
	double value=0.0;
	
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

template<typename Iterator, typename T>
int my_count(Iterator first, Iterator last, T val)
{

	int count = 0;
	for (Iterator p = first; p != last; ++p)
		if (*p==val) ++count;
	return count;

}

template<typename Iterator, typename UnaryPredicate>
int my_count_if(Iterator first, Iterator last, UnaryPredicate pred)
{

	int count = 0;
	for (Iterator p = first; p != last; ++p)
		if (pred(*p)) ++count;
	return count;

}

bool odd(int x) { return x % 2 == 0; }	// Nombre pair ?

struct Fruit {  
	string name{};
	int count=0;
	double unit_price=0.0;
	
};

struct Fruit_comparison {  
	bool operator()(const Fruit* a, const Fruit* b) const { return a->name < b->name; }
	
};

template<typename Iterator, typename T>
bool my_binary_search(Iterator first, Iterator last, const T& value)
{
	// ATTENTION : le container sur lequel est basé les itérateurs en argument doit être TRIE. 
	first = lower_bound(first, last, value);
	return (!(first == last) && !(value < *first));
}

class product {
	string product_name{};
	double unit_price = 0;
	int count = 0;

public:
	product() = default;
	product(string n, double p, int c):product_name(n), unit_price(p), count (c) {}

	int get_count() { return count; }
	string get_product() { return product_name; }
	double get_price() { return unit_price; }
};

class order {
	string customer_name{};
	string customer_address{};
	product purchase{};
		
public:
	order() = default;
	order(string n, string a, product p) :customer_name(n), customer_address(a), purchase(p) {}
	const string get_name() const { return customer_name; }
	const string get_address() const { return customer_address; }
	string get_order_article() { return purchase.get_product(); }
	int get_order_count() { return purchase.get_count(); }
	double get_order_price() { return purchase.get_price(); }

	void print_order();
};

void order::print_order()
{
	
	cout << "La société " << customer_name << " basée à " << customer_address << " a commandé " << purchase.get_count() << " "
		<< purchase.get_product() << " au prix unitaire de " << purchase.get_price() << "E, soit un total de " << purchase.get_price()*purchase.get_count() << " euros" << endl;


}

template<typename T>
void populate_from_file(string filename,T& c) {
	// Fonction Helper qui ouvre un fichier en lecture et popule le container choisi avec des commandes
	ifstream ist = open_file_read(filename);
	
	string company, town, article;
	double price;
	int quantity;
	string s{};
	while (!ist.eof()) {					// Tant que la fin du fichier n'est pas atteinte, on continue à extraire de nouveaux enregistrements
		getline(ist, s);					// On lit une ligne terminée par un NEWLINE
		stringstream ss(s);
		ss >> company >> town >> article >> price >> quantity;
		c.push_back(order{ company,town, product{ article,price,quantity } });
	}
	ist.close();							// Fermeture du fichier
}

template<typename T>
void populate_to_file(string filename, const T& c) {
	// Fonction Helper qui ouvre un fichier en écriture et le popule avec le container d'un container
	ofstream ost = open_file_write(filename);
	
	string s{};
	for (auto x:c) {
		s = x.get_name() + ' ' + x.get_address() + ' ' + x.get_order_article() + ' ' + to_string(x.get_order_price()) + ' ' + to_string(x.get_order_count());
		ost << s << endl;
	
	}
	
	ost.close();							// Fermeture du fichier
}

int main()
{
	try
	{
		// A mettre dans la 1ère ligne du "main" pour afficher les caractères accentués sous la console Windows
		std::locale::global(std::locale("fr-FR"));

		// Exos 9 à 12 page 801 (hors GUI, déjà réalisée pour le Wumpus = même principe)
		vector<order> commandes{};
		populate_from_file("commandes.txt", commandes);
		sort(commandes.begin(), commandes.end(), [](order& a, order& b) {return a.get_name() < b.get_name(); });
		
		double somme = 0;
		for (auto x : commandes) {
			x.print_order();
			somme += x.get_order_count()*x.get_order_price();
		}
		cout << "------------------------------------------------------------" << endl;
		cout << "Le total des commandes s'établit à " << somme << " euros." << endl;
		
		list<order> commandes2{};
		populate_from_file("commandes2.txt", commandes2);
		commandes2.sort([](order& a, order& b) {return a.get_address() < b.get_address(); });
		
		// On popule les fichiers en écriture et on les merge
		populate_to_file("commandes_w.txt", commandes);
		populate_to_file("commandes2_w.txt", commandes2);

		/* Merge ************************************************************************************************************************************************************
		// Dans les deux cas (mémoire ou fichier), une bug "sequence not ordered" apparaît et fait planter le programme : pas à la compilation toutefois 
		
		// Option 1 : en mémoire
		vector<order> commandes_merged{};
		merge(commandes.begin(), commandes.end(), commandes2.begin(), commandes2.end(), commandes_merged.begin(), 
			[](const order& a, const order& b) {return a.get_name() < b.get_name(); });	// OOPS !
		cout << "Vector + liste merged :" << endl;
		for (auto x : commandes_merged)
			x.print_order();
		cout << endl;

		// Option 2 : en fichier		
		// On merge les 2 fichiers dans un troisième en utilisant un stream_iterator (page 791)
		ifstream is1{ "commandes_w.txt" };
		ifstream is2{ "commandes2_w.txt" };
		ofstream os{ "merge_commandes.txt" };

		istream_iterator<string> ii1{ is1 };
		istream_iterator<string> ii2{ is2 };
		istream_iterator<string> eos;
		ostream_iterator<string> oo{ os, "\n" };
		
		vector<string>file1{ ii1,eos };
		vector<string>file2{ ii2,eos };
		merge(file1.begin(), file1.end(), file2.begin(), file2.end(), oo, [](const string& a, const string& b) {return a<b; });	OOPS !
		*/

		// Recherche d'une liste de commandes passées dans une ville saisie : on part de la list qui est déjà triée sur la ville
		cout << "Saisir une ville qui permettra d'y retrouver les commandes passées :" << endl;
		string town{}; cin >> town;
		
		// Version 1 : classique
		for (auto x : commandes2) {
			if (x.get_address() == town)
				cout << "La société " << x.get_name() << " de " << x.get_address() << " a commandé" << endl;
		}
		
		// Version 2 : l'appel à equal_range ne fonctionne pas : erreur C2664 liée à l'appel de la structure Comp -> A ETUDIER XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
		/*
		struct Comp
		{
			bool operator() (const order& a, const string& s) const {return a.get_address() == s;}
		
		};

		auto px = equal_range(commandes2.begin(), commandes2.end(), town, Comp{});
		for (auto i = px.first; i != px.second; ++i)
			cout << "La société " << i->get_name() << " de " << i->get_address() << " a commandé" << endl;
		*/
		
		
		
		// ***************************************************************************************************************************************************
		// Drill page 798 - partie vecteur I (list identique)
		vector<Item> vi{};
		ifstream ist = open_file_read("values.txt");
		
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

		// Exo 7 page 800 : binary search - ATTENTION : le container doit être trié avant l'appel de binary_search
		vector<int>bs{ 1,258,3,4,5,6,71587,8,99,987,765,765,98,435,23,9876,9999 };
		sort(bs.begin(), bs.end());
		auto pbs = my_binary_search(bs.begin(), bs.end(),987);
		if (pbs)
			cout << "La valeur a été trouvée dans le vecteur"<<endl;
		else 
			cout<<"Valeur inexistante dans le vecteur !" << endl;

		list<string>bss{ "aa","aa","aa","b","c","dd","eee","ffff","aa","aa","c" };
		bss.sort();
		pbs = my_binary_search(bss.begin(), bss.end(), "ffff");
		if (pbs)
			cout << "La valeur a été trouvée dans la list *****************************************************" << endl;
		else
			cout << "Valeur inexistante dans la list ! ********************************************************" << endl;

		// Drill page 798 - partie map
		map <string,int> msi{ {"André", 52}, {"Marc", 40},{"Paul", 63},{"Vivien", 19},{"Pauline", 18},{"Sylvie", 50},{"Lionel", 41},{"Everett", 26},{"Jules", 80},{"Alice", 78} };
		print_map(msi);

		auto p1 = msi.find("André");																// Erase en partant d'un nom
		msi.erase(p1); print_map(msi);

		
		cout << "Saisir 3 couples de valeurs (prénom, âge) :" << endl;								// Insertion de valeurs saisies sur cin : bizarre -> Map non triée...
		string prénom{};
		int age = 0;
		for (int i = 0; i < 3; ++i)
			cin >> prénom >> age; msi.insert(make_pair(prénom, age));
		print_map(msi);
		
		cout << "La somme des âges est égale à " << accu_map(msi) << endl;							// Création d'une fonction car je n'arrive pas à utiliser accumulate sur une map

		map <string, int> mis{};																	// Copie
		mis = msi;
		print_map(mis);


		// Drill page 798 - partie vecteur II
		vector<double> vd{};
		ifstream ist1=open_file_read("floating_values.txt");
		
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

		// Exos 3&4 : implémentation de cout et count_if
		vector<int>my1{ 1,2,3,3,4,5,6,6,6,6,8,9,9,8};
		cout<< "Le vecteur my1 contient " << my_count(my1.begin(), my1.end(), 6) << " fois la valeur 6" << endl;
		cout << "Le vecteur my1 contient " << my_count_if(my1.begin(), my1.end(), odd) << " nombres pairs" << endl;	//v1
		cout << "Le vecteur my1 contient " << my_count_if(my1.begin(), my1.end(), [](int i) {return i % 2 == 0; }) << " nombres pairs" << endl;	//v2 avec une lambda

		vector<string>my2{ "aa","aa","aa","b","c","d","eee","fff","aa","aa","c" };
		cout << "Le vecteur my2 contient " << my_count(my2.begin(), my2.end(), "aa") << " fois la valeur aa" << endl;
		
		// Exo 6 page 800
		set<Fruit*, Fruit_comparison>inventory;
		inventory.insert(new Fruit{"pomme",55});
		inventory.insert(new Fruit{ "abricot",10 });
		inventory.insert(new Fruit{ "poire",20 });
		
		for (auto p=inventory.begin();p!=inventory.end();++p)
		{
			// Le stockage sur la stack (new) complexifie la récupération des données, d'où la syntaxe d'indirection (*p)->name
			cout << "Stock en "<< (*p)->name <<" : "<< (*p)->count<<" unités"<<endl;
		}
		
		// Exo 8 page 800 : pas de solution évidente trouvée pour trier la map sur la valeur => Je passe donc par un vecteur string intermédiaire (w)
		map<string, int>words;
		
		cout << "Entrer des mots (CTRL Z pour stopper) :" << endl;
		for (string s; cin >> s;)
			++words[s];
		
		vector <string>w;
		
		// On remplit le vecteur w en prenant soin de le laisser trié
		for (const auto& p : words) {
			
			// On recherche la 1ère valeur immédiatement supérieure 
			auto upper = upper_bound(w.begin(), w.end(), to_string(p.second));
			
			// Si aucune valeur n'est trouvée, on insère la valeur à la fin du vecteur (vrai quand le vecteur est vide)
			if (upper == w.end())
				w.push_back(to_string(p.second)+' '+ p.first);
			else
				w.insert(upper, to_string(p.second) + ' ' + p.first);
				
		}
		
		for (const auto&p : w)
			cout << p << endl;
		
		keep_window_open();

		return 0;
	}


	catch (runtime_error& e)
	{	// this code is to produce error messages
		cout << e.what() << '\n';
		keep_window_open("q");	// For some Windows(tm) setups
	}

	
}


