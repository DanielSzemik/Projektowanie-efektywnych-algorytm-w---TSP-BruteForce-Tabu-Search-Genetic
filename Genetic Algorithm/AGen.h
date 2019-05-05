#pragma once

class AGen {
public:
	AGen(int);
	//~AGen();
private:
	int WlkPop;
	int LiczbaEliteow;
	float WspRuletkowy;
	float WspMutacji;
	int liczbamiast;
	int **macierz;
	int **populacja;
	int **populacjatemp;
	int **drogi;
	int **malzenstwa;
	bool *odwiedzone;
	int *ranking;
	int *dziecko;
	void inicjalizujtablice();
	int obliczdroge(const int[]);
	void kopiujdroge(int *src, int *dst);
	void sortujpopulacje();
	void wczytajzpliku();
	void WylosujPoczPop();
	void wyswietlpop();
	void wyswietlnajkrotszadroge();
	void selekcjaruletkowa();
	void krzyzowanie();
	int* zkrzyrzuj(int rodzic1, int rodzic2);
	int* zmutuj(int* droga);
};