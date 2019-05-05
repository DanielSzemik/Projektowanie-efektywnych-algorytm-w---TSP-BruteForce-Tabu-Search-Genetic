#include "stdafx.h"
#include "AGen.h"
#include <iostream>
#include <fstream>
#include <limits>
#include <stdio.h>      /* printf, scanf, puts, NULL */
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <random>
#include "czas.h"

using namespace std;


AGen::AGen(int czasnarozw)
{
	//***** PARAMETRY *****
	WlkPop = 20;
	LiczbaEliteow = 7;
	WspMutacji = 15; //w procentach, 0-100
	WspRuletkowy = 1.1; 
	/*WspRuletkowy >=1, wieksze wartosci oznaczaja bardziej rowne losowania,
	dla 1 nie mozliwe jest wylosowania do krzyzowania najgorszego osobnika z populacji, dla 1.1 jest wybierany okolo 60% tak czesto jak pierwszy 
	kazdy chromosom dostaje wage rowna: (ostatni chromosom * WspRuletkowy) - dlugosc drogi*/

	srand(time(NULL));
	wczytajzpliku();
	inicjalizujtablice();
	int liczbagen = 0;
	czas czas;
	double pomiar = 0;
	czas.StartCounter();
	//***** ALGORYTM ******
	WylosujPoczPop();
	sortujpopulacje();
	cout << endl;

	while (pomiar < czasnarozw*1000)
	{
		selekcjaruletkowa();
		krzyzowanie(); // + mutacja
		sortujpopulacje();
		pomiar = czas.GetCounter();
		liczbagen++;
	}
	cout<<"\n Liczba generacji: "<< liczbagen << endl;
	//wyswietlpop();
	wyswietlnajkrotszadroge();
}

void AGen::krzyzowanie()
{
	int rzutkostka;
	for (int i = 0; i < WlkPop - LiczbaEliteow; i++)
	{
		dziecko = zkrzyrzuj(malzenstwa[i][0], malzenstwa[i][1]);
		rzutkostka = rand() % 100;
		if (rzutkostka < WspMutacji)
		{
			dziecko = zmutuj(dziecko);
		}
		kopiujdroge(dziecko, populacjatemp[i]);
	}
	for (int i = LiczbaEliteow; i < WlkPop-1; i++)
	{
		kopiujdroge(populacjatemp[i - LiczbaEliteow], populacja[i]);
	}
}

int* AGen::zmutuj(int* droga)
{
	int v1 = rand() % (liczbamiast - 1);
	int v2 = v1;
	while(v2 == v1)
		v2 = rand() % (liczbamiast - 1);
	swap(droga[v1], droga[v2]);
	return droga;
}

int* AGen::zkrzyrzuj(int rodzic1, int rodzic2) //OX CROSSOVER
{
	int punkt1, punkt2;
	
	for (int j = 0; j < liczbamiast; j++) //ustawiamy wszystkie wierzcholki na nie odwiedzone
	{
		odwiedzone[j] = false;
	}
	odwiedzone[0] = true;
	punkt1 = rand() % (liczbamiast - 2) + 1; //przed ktorym wierzcholkiem jest kreska
	punkt2 = punkt1;
	while(punkt2==punkt1)
		punkt2 = rand() % (liczbamiast - 2) + 1;
	if (punkt2 < punkt1)
		swap(punkt1, punkt2);
	for (int i = punkt1; i < punkt2; i++)
	{
		dziecko[i] = populacja[rodzic1][i];
		odwiedzone[dziecko[i]] = true;
	}
	int j = punkt2;
	for (int i = punkt2;i!=punkt1; i++)
	{
		if (i > liczbamiast - 2)
			i = 0;
		
		while (odwiedzone[populacja[rodzic2][j]] == true)
		{
			j++;
			if (j > liczbamiast - 2)
					j = 0;
		}
		dziecko[i] = populacja[rodzic2][j];
		odwiedzone[populacja[rodzic2][j]] = true;
		if (i == punkt1-1)
			break;
	}
	return dziecko;
	
}

void AGen::selekcjaruletkowa()
{
	std::mt19937_64 gen(std::random_device{}());
	int sumadrog = 0;
	int *sumakum = new int[WlkPop];
	int rodzic1 = 0;
	int rodzic2 = 0;
	uint64_t  los1, los2;
	float maxf = obliczdroge(populacja[WlkPop - 1]) * WspRuletkowy;
	int max = (int)maxf;
	for (int i = 0; i < WlkPop; i++)
	{
		sumadrog += (max - obliczdroge(populacja[i]));
		sumakum[i] = sumadrog;
	}
	for (int i = 0; i < WlkPop-LiczbaEliteow; i++)
	{
		los1 = gen() % (sumadrog + 1);
		los2 = gen() % (sumadrog + 1);
		for (int j = 0; j < WlkPop; j++)
		{
			if (sumakum[j] > los1)
			{
				rodzic1 = j;
				break;
			}
		}
		for (int j = 0; j < WlkPop; j++)
		{
			if (sumakum[j] > los2)
			{
				rodzic2 = j;
				break;
			}
		}
		malzenstwa[i][0] = rodzic1;
		malzenstwa[i][1] = rodzic2;
	}
	delete sumakum;
}

void AGen::wczytajzpliku()
{
	ifstream plik;
	plik.open("dane.txt");
	plik >> liczbamiast;
	macierz = new int*[liczbamiast];
	for (int i = 0; i < liczbamiast; ++i) {
		macierz[i] = new int[liczbamiast];
	}
	for (int i = 0; i<liczbamiast; i++)
	{
		for (int j = 0; j<liczbamiast; j++)
		{
			plik >> macierz[i][j];
		}
	}
}

void AGen::sortujpopulacje()
{

	
	for (int i = 0; i < WlkPop; i++)
		ranking[i] = -1;
	

	for (int i = 0; i < WlkPop; i++) //tworzymy tablice z droga i indeksem kazdego chromosou
	{
		drogi[i][0] = obliczdroge(populacja[i]);
		drogi[i][1] = i;
	}
	for (int i = 0; i < WlkPop; i++) //insertion sort
	{
		for (int j = 0; j < WlkPop; j++)
		{
			if (ranking[j] == -1)
			{
				ranking[j] = i;
				break;
			}
			if (drogi[ranking[j]][0] > drogi[i][0])
			{
				for (int k = i; k > j; k--)
				{
					ranking[k] = ranking[k - 1];
				}
				ranking[j] = i;
				break;
			}
		}
	}
	for (int i = 0; i < WlkPop; i++) //kopiujemy populacje do populacjatemp
	{
		kopiujdroge(populacja[i], populacjatemp[i]);
	}
	for (int i = 0; i < WlkPop; i++) //porzadkujemy chromosomy wedlug rankingu
	{
		kopiujdroge(populacjatemp[ranking[i]], populacja[i]);
	}

}
void AGen::kopiujdroge(int *src, int *dst) {
	for(int i = 0; i < liczbamiast - 1; i++)
	{
		dst[i] = src[i];
	}
}

void AGen::WylosujPoczPop() 
{
	int v;
	for (int i = 0; i<WlkPop; i++)
	{
		for (int j = 0; j < liczbamiast - 1; j++) //ustawiamy wszystkie wierzcholki na nie odwiedzone
		{
			odwiedzone[j] = false;
		}
		for (int j = 0; j < liczbamiast - 1; j++) //losujemy wierzcholek, jezeli juz byl uzyty to inkrementujemy nr wierzcholka az bedzie wolny i go dodajemy
		{
			v = (rand() % (liczbamiast - 1)) + 1;
			while (odwiedzone[v-1] == true) 
			{
				v++;
				if (v >= liczbamiast)
					v = 1;
			}
			populacja[i][j] = v;
			odwiedzone[v-1] = true;
		} 

	}
}

void AGen::wyswietlpop()
{
	for (int i = 0; i < WlkPop; i++)
	{
		for (int j = 0; j < liczbamiast-1; j++)
		{
			cout << populacja[i][j] << "  ";
			if (populacja[i][j] < 10) cout << " ";
			if (populacja[i][j] < 100) cout << " ";
		}
		cout << "  Droga:" << obliczdroge(populacja[i]);
		cout << endl;
	}
}

void AGen::wyswietlnajkrotszadroge()
{
	cout << "Najkrotsza droga to: ";
	for (int j = 0; j < liczbamiast - 1; j++)
	{
		cout << populacja[0][j] << "->";
	}
	cout<<"\n\nDlugosc: "<< obliczdroge(populacja[0])<<" jednostek\n\n";
}

int AGen::obliczdroge(const int kolejnosc[])
{
	int suma = 0;
	suma += macierz[0][kolejnosc[0]];		//droga do I miasta do odwiedzenia
	for (int i = 0; i < liczbamiast - 2; i++)
	{
		suma += macierz[kolejnosc[i]][kolejnosc[i + 1]]; //dodajemy drogi do kolejnych miast
	}
	suma += macierz[kolejnosc[liczbamiast - 2]][0];	//dodajemy droge zeby wrocic do pierwszego miasta
	return suma;
}

void AGen::inicjalizujtablice()
{
	dziecko = new int[liczbamiast - 1];
	ranking = new int[WlkPop];
	odwiedzone = new bool[liczbamiast];
	populacjatemp = new int*[WlkPop];
	for (int i = 0; i < WlkPop; ++i) {
		populacjatemp[i] = new int[liczbamiast - 1];
	}
	populacja = new int*[WlkPop];
	for (int i = 0; i < WlkPop; ++i) {
		populacja[i] = new int[liczbamiast - 1];
	}
	drogi = new int*[WlkPop];
	for (int i = 0; i < WlkPop; i++)
		drogi[i] = new int[2];
	malzenstwa = new int*[WlkPop - LiczbaEliteow];
	for (int i = 0; i < WlkPop - LiczbaEliteow; i++)
		malzenstwa[i] = new int[2];
}