#pragma once

struct elementlistytabu {

public:
	int a;
	int b;
};

class tabu {
public:
	tabu(int, int);
	~tabu();
	elementlistytabu * listatabu;
private:
	int liczbamiast;
	int **macierz;
	int dlugosclistytabu;
	//int *kolejnosc;

	int *najkrotsza_kolejnosc;
	int najkrotszadroga;
	void wczytajzpliku();
	void wyswietldane(int,int);
	void wyswietlwyniki();
	void znajdzIrozw();
	int* przeszukajotoczenie(int*, elementlistytabu*);
	int obliczdroge(const int[]);
	bool SprCzyElNalezaDoListyTabu(int, int, elementlistytabu*);
	void dodajdolistytabu(int, int, elementlistytabu*);
	//std::vector<pair<int, int>>  listatabu;
	
};