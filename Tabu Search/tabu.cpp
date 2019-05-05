#include "stdafx.h"
#include "tabu.h"
#include <iostream>
#include <fstream>
#include <limits>
#include "czas.h"
using namespace std;


tabu::tabu(int dlugosclisty, int czasnarozw)
{
	wczytajzpliku();
	dlugosclistytabu = dlugosclisty * liczbamiast;
	elementlistytabu* listatabu = new elementlistytabu[dlugosclistytabu];
	for (int i = 0; i < dlugosclistytabu - 1; i++)
	{
		listatabu[i].a = -1;
		listatabu[i].b = -1;
	}
	wyswietldane(dlugosclisty,czasnarozw);
	int * kolejnosc = new int[liczbamiast - 1];
	najkrotsza_kolejnosc = new int[liczbamiast - 1];
	znajdzIrozw();
	najkrotszadroga = obliczdroge(najkrotsza_kolejnosc);
	wyswietlwyniki();

	czas czas;
	double pomiar = 0;
	czas.StartCounter();
	while (pomiar < czasnarozw)
	{
		najkrotsza_kolejnosc = przeszukajotoczenie(najkrotsza_kolejnosc, listatabu);
		pomiar = czas.GetCounter();
	}
	najkrotszadroga = obliczdroge(najkrotsza_kolejnosc);
	wyswietlwyniki();
}

int* tabu::przeszukajotoczenie(int *a, elementlistytabu* listatabu)
{
	int * b = new int[liczbamiast - 1];
	int * b_min = new int[liczbamiast - 1];
	int temp = -1;
	for (int i = 0; i < liczbamiast - 2; i++)
	{
		for (int j = i + 1; j < liczbamiast - 1; j++)
		{	
			if (SprCzyElNalezaDoListyTabu(i, j,listatabu) == false)
			{
				memcpy(b, a, sizeof(int)*(liczbamiast - 1));
				if (temp == -1) //sprawdzenie czy b_min juz cos ma
					memcpy(b_min, b, sizeof(int)*(liczbamiast - 1));
				temp = b[i];
				b[i] = b[j];
				b[j] = temp;
				if (obliczdroge(b) < obliczdroge(b_min))
				{
					memcpy(b_min, b, sizeof(int)*(liczbamiast - 1));
					dodajdolistytabu(i, j, listatabu);
				}
			}
		}
	}
	return b_min;
}

bool tabu::SprCzyElNalezaDoListyTabu(int x1, int x2, elementlistytabu* listatabu)
{	
	for (int i = 0; i < dlugosclistytabu-1; i++)
	{
		if (listatabu[i].a == x1 && listatabu[i].b == x2)
			return true;
		if (listatabu[i].a == x2 && listatabu[i].b == x1)
			return true;
	}
	return false;
}
void tabu::dodajdolistytabu(int x1, int x2, elementlistytabu* listatabu)
{
	for (int i = dlugosclistytabu - 1; i > 0; i--)
	{
		listatabu[i].a = listatabu[i - 1].a;
		listatabu[i].b = listatabu[i - 1].b;
	}
	listatabu[0].a = x1;
	listatabu[0].b = x2;
}
void tabu::znajdzIrozw()
{
	
	bool * odwiedzonemiasta = new bool[liczbamiast - 1];
	for (int i = 1; i < liczbamiast; i++) //ustawienie wartosci w tablicy pamietajacej odwiedzone miasta na falsz, za wyjatkiem I od ktorego zaczynamy
		odwiedzonemiasta[i] = false;
	odwiedzonemiasta[0] = true;
	int wiersz = 0; //zaczynamy od I miasta czyli zerowego wiersza
	int min;
	int nastepnemiasto = 0;
	for (int i = 0; i < liczbamiast-1; ) //i oznacza ile miast juz odwiedzilismy
	{
		min = numeric_limits<int>::max();
		for (int j = 0; j < liczbamiast; j++)
		{
			if (odwiedzonemiasta[j] == false && wiersz != j)
			{
				if (min > macierz[wiersz][j])
				{
					min = macierz[wiersz][j];
					nastepnemiasto = j;
				}
			}
		}
		najkrotsza_kolejnosc[i] = nastepnemiasto;
		odwiedzonemiasta[nastepnemiasto] = true;
		wiersz = nastepnemiasto;
		i++;
	}
}

int tabu::obliczdroge(const int kolejnosc[])
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


void tabu::wczytajzpliku()
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

void tabu::wyswietldane (int wielkosctablicy, int czas)
{
	cout << "Czas: " << czas << " sekund\n";
	cout << "Wielkosc tablicy tabu: " << wielkosctablicy << endl;
	/*for (int i = 0; i<liczbamiast; i++)
	{
		for (int j = 0; j<liczbamiast; j++)
		{
			cout << macierz[i][j] << "  ";
			if (macierz[i][j] < 10) cout << " ";
			if (macierz[i][j] < 100) cout << " ";
		}
		cout << endl;
	}*/
	cout << endl;
}

void tabu::wyswietlwyniki()
{
	cout << "Najkrotsza znaleziona droga:\n0";
	for (int i = 0; i < liczbamiast - 1; i++)
		cout << " -> " << najkrotsza_kolejnosc[i];
	cout << " -> 0 \n\nOdleglosc: " << najkrotszadroga << " jednostek\n\n";
}

tabu::~tabu()
{
	for (int i = 0; i < liczbamiast; ++i) {
		delete[] macierz[i];
	}
	delete[] macierz;
	delete[] najkrotsza_kolejnosc;
}
