#ifndef BRUTEFORCE_H_INCLUDED
#define BRUTEFORCE_H_INCLUDED

class BruteForce {
public:
	BruteForce();
	~BruteForce();
private:
	int liczbamiast;
	int **macierz;
	//int *kolejnosc;
	
	int *najkrotsza_kolejnosc;
	int najkrotszadroga;
	void permutacje(int[], int, int);
	void wczytajzpliku();
	void wyswietlmacierz();
	void wyswietlwyniki();
	int obliczdroge(const int[]);
};



#endif // BRUTEFORCE_H_INCLUDED
