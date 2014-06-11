#ifndef SIMPLEX
#define SIMPLEX

#include <string>

#define M 66666
#define MAX_ZMIENNYCH 20
#define MAX_OGRANICZEN 20

class simplex{
private:

	//wspolczynniki funkcji celu
	double wspl_celu[MAX_ZMIENNYCH];

	//macierz ograniczen
	double ukl_ograniczen[MAX_OGRANICZEN][MAX_ZMIENNYCH];

	//zmienne wchodzace w sklad bazy, gdy x>0 to wchodzi
	int zmienne_bazowe[MAX_OGRANICZEN];

	//wyrazy wolne ukladu ograniczen
	double wyr_wolne[MAX_OGRANICZEN];

	double wskazniki_pomocnicze[MAX_ZMIENNYCH];
	double ilorazy[MAX_ZMIENNYCH];
	double kryt_simplex[MAX_ZMIENNYCH];
	double wartosc_funkcji_celu;

	//ilosc zmiennych
	int il_zmiennych;

	//ilosc ograniczen (rownan)
	int il_ograniczen;

	//zmienna ktora wjedzie do nowego rozwiazania bazowego
	int nowa_zmienna;
	//zmienna ktora zostanie usunieta z rozwiazania bazowego
	int stara_zmienna;

	//warotsc wspolczynnika na przecieciu
	double wartosc_na_przecieciu;

	std::string linie_transportowe[MAX_ZMIENNYCH];
public:

	void wczytaj(std::string nazwa_pliku);
	void wypisz();
	void wypisz_tablice();
	void wynik();

	void oblicz();
	bool czy_optymalne_rozwiaznie();

	int stworz_tablice();
};


#endif
