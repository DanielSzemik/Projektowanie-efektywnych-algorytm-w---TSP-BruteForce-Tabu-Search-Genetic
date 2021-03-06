// tabu.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include "tabu.h"
#include <fstream>
#include <stdio.h>      /* printf, scanf, puts, NULL */
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

using namespace std;

void wygenerujplik(int);

int main()
{
	char wybor;
	int sekundy, wielkosctablicy;
	int iloscmiast;
	cout << "Wygenerowac dane tekstowe? y/n \n";
	cin >> wybor;
	if (wybor == 'y')
	{
		cout << "Prosze podac ilosc miast \n";
		cin >> iloscmiast;
		wygenerujplik(iloscmiast);
	}
	cout << "Prosze podac ile czasu w sekundach ma sie algorytm wykonywac\n";
	cin >> sekundy;
	cout << "Prosze podac wielkosc tablicy tabu \n";
	cin >> wielkosctablicy;
	tabu tabu(wielkosctablicy,sekundy);
	system("pause");
    return 0;
}

void wygenerujplik(int liczbamiast)
{
	srand(time(NULL));
	int liczba;
	ofstream plik;
	plik.open("dane.txt");

	plik << liczbamiast << endl;
	for (int i = 0; i<liczbamiast; i++)
	{
		for (int j = 0; j < liczbamiast; j++)
		{
			if (i == j)
				plik << "0   ";
			else
			{
				liczba = rand() % 999 + 1;
				plik << liczba << " ";
				if (liczba < 10)
					plik << " ";
				if (liczba < 100)
					plik << " ";
			}
		}
		plik << endl;
	}
	plik.close();

}