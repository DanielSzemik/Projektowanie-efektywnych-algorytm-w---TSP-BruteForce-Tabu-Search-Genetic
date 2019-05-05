#include "stdafx.h"
#include "BruteForce.h"
#include <iostream>
#include <fstream>
#include <limits>
#include "czas.h"

using namespace std;


BruteForce::BruteForce()
{
	wczytajzpliku();
	wyswietlmacierz();
	czas czas;
	double pomiar;
	czas.StartCounter();
	int * kolejnosc = new int[liczbamiast - 1];
	najkrotsza_kolejnosc = new int[liczbamiast - 1];
	for (int i = 0; i < liczbamiast - 1; i++) //ustawienie pierwszej permutacji na 0->1->2->...
		kolejnosc[i] = i + 1;
	najkrotszadroga = numeric_limits<int>::max();
	
	permutacje(kolejnosc, liczbamiast - 1, (liczbamiast / sizeof(int))); //przejscie przez wszystkie permutacje, algorytm Heape'a
	pomiar = czas.GetCounter();
	wyswietlwyniki();
	cout<< "Czas: " << pomiar << " milisekund" << endl<<endl;
}

void BruteForce::permutacje(int a[], int size, int n) //algorytm Heape'a
{
	if (size == 1)
	{
		if (obliczdroge(a) < najkrotszadroga)
		{
			najkrotszadroga = obliczdroge(a);
			memcpy(najkrotsza_kolejnosc, a, sizeof(int)*(liczbamiast - 1));
		}
		return;
	}
	for (int i = 0; i < size; i++)
	{
		permutacje(a, size - 1, n);
		if (size % 2 == 1)
			swap(a[0], a[size - 1]);
		else
			swap(a[i], a[size - 1]);
	}
}

int BruteForce::obliczdroge(const int kolejnosc[])
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


void BruteForce::wczytajzpliku()
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

void BruteForce::wyswietlmacierz()
{
	for (int i = 0; i<liczbamiast; i++)
	{
		for (int j = 0; j<liczbamiast; j++)
		{
			cout << macierz[i][j]<<"  ";
		}
		cout << endl;
	}
	cout << endl;
}

void BruteForce::wyswietlwyniki()
{
	cout << "Najkrotsza droga:\n0";
	for (int i = 0; i < liczbamiast - 1; i++)
		cout << " -> " << najkrotsza_kolejnosc[i];
	cout << " -> 0 \n\nOdleglosc: " << najkrotszadroga << " jednostek\n\n";
}

BruteForce::~BruteForce()
{
	for (int i = 0; i < liczbamiast; ++i) {
		delete[] macierz[i];
	}
	delete[] macierz;
	delete[] najkrotsza_kolejnosc;
}
