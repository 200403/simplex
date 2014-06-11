#include "simplex.hh"

#include <iostream>
#include <fstream>
#include <iomanip>

using namespace std;


/* Funkcja wczytuje dane wejsciowe z pliku
Format danych:
il_magazynow il_fabryk

macierz kosztow:
x11 x12 ...
x21
.
.
.

produkcja fabryk:
S1 S2 S3 ...

zapotrzebowanie magazynow:
D1 D2 D3 ...

*/
void simplex::wczytaj(string nazwa_pliku){
	ifstream plik;
	plik.open(nazwa_pliku.c_str(), ios::out);

	//wczytuje ilosc zmiennych i ilosc oraniczen
	int il_fabryk, il_magazynow;
	plik >> il_fabryk >> il_magazynow;
	il_zmiennych = il_fabryk * il_magazynow;
	il_ograniczen = il_fabryk + il_magazynow;

	//wczytuje wspolczynniki funkcji celu

	/*for (int i=0; i<il_zmiennych; i++){
		double zm_celu;
		plik >> zm_celu;
		wspl_celu[i] = 0 - zm_celu;
	}*/

	//nazwy magazynow
	string magazyny = "ABCDEFGHIJKLMN";
	string fabryki = "0123456789";

	for(int i=0; i<il_fabryk; i++){
		for(int j=0; j<il_magazynow; j++){
			double zm_celu;
			plik >> zm_celu;
			wspl_celu[i*il_fabryk + j] = 0 - zm_celu;
			string  a,b;
			a="Fabryka ";
			a+=fabryki[i];
			b="Magazyn";
			b+=magazyny[j];

			linie_transportowe[i*il_fabryk + j] = a + " -> " +b;
		}
	}

	//wczytuje uklad ograniczen i wektor wyrazow wolnych
	for (int i=0; i <il_ograniczen; i++){
		plik >> wyr_wolne[i];
	}
	for(int i=0; i<il_zmiennych+il_ograniczen; i++)
	{
		for(int j=0; j<il_ograniczen; j++){
			ukl_ograniczen[j][i]=0;
		}
	}

	for(int k=0; k<il_magazynow; k++){
		for(int l=0; l<il_fabryk; l++){
			int temp=l+il_magazynow*k;
			ukl_ograniczen[k][temp]=1;
		}
	}
	int x = 0;
	for(int i=il_fabryk; i <= il_ograniczen; i++){

		for(int j=0; j<il_fabryk; j++){
			int temp = j*il_magazynow + x;
			ukl_ograniczen[i][temp]=1;
		}
		x++;
	}

	//dodawanie zmiennych sztucznych
	/*for(int i=il_zmiennych; i<il_zmiennych+il_ograniczen; i++){
		wspl_celu[i]=-999999;
	}
	for(int i=0; i<il_ograniczen; i++){
		ukl_ograniczen[i][i+il_zmiennych]=1;
		zmienne_bazowe[i]=i+il_zmiennych;
	}

	il_zmiennych=il_zmiennych + il_ograniczen;*/
	plik.close();
}

void simplex::wypisz(){
	cout << endl << endl<< "ilosc zmiennych: " << il_zmiennych << endl;
	cout << "ilosc ograniczen: " << il_ograniczen << endl;
	cout << "Uklad ograniczen:" << endl;
	for( int i=0; i< il_ograniczen; i++){
		for(int j=0; j<il_zmiennych; j++){
			cout << setw(3) <<  ukl_ograniczen[i][j];
		}
		cout << " = " << wyr_wolne[i] << endl;
	}
	cout << "funkcja celu:" << endl;
	for(int i=0; i < il_zmiennych; i++){
		if(wspl_celu[i]>=M)
			cout << setw(3) << 'm';
		else if(wspl_celu[i]<=-M)
			cout << setw(3) << "-m";
		else
			cout << setw(3) << wspl_celu[i];
	}
	cout << endl;
}


void simplex::wypisz_tablice(){
	int sz=10;
	std::cout.precision(2);
	std::cout.setf( std::ios::fixed);

	cout << endl;
		for(int k=0; k<12+il_zmiennych*6+12; k++){
			cout << "-";
		}
	cout << endl <<setw(sz*2+1)<<"|";
	for(int i=0; i<il_zmiennych; i++){
		if(wspl_celu[i]>=M)
			cout << setw(sz) << 'm'<<"|";
		else if(wspl_celu[i]<=-M)
			cout << setw(sz) << "-m"<<"|";
		else
			cout << setw(sz) << wspl_celu[i]<<"|";
	}

	cout <<endl << setw(sz*2+1)<<"|";
	for(int i=0; i<il_zmiennych;i++){
		cout <<setw(sz-1) <<  "x"<< i <<"|";
		}
	cout << endl;
		for(int k=0; k<12+il_zmiennych*6+12; k++){
			cout << "-";
		}
		cout << endl;
	for( int i=0; i< il_ograniczen; i++){
		cout <<setw(sz-1)<<wspl_celu[zmienne_bazowe[i]]<<"|"<< setw(sz-1)<<"x"<<setw(2) << zmienne_bazowe[i];
		for(int j=0; j<il_zmiennych; j++){
			cout <<"|"<< setw(sz) <<  ukl_ograniczen[i][j];
		}
		cout << "|"<<setw(sz) << wyr_wolne[i] <<"|"<<setw(sz)<<ilorazy[i]<< endl;

	}
	for(int k=0; k<12+il_zmiennych*6+12; k++){
				cout << "-";
			}
	cout <<endl << setw(sz*2+1)<<"|";
		for(int i=0; i<il_zmiennych; i++){
			cout << setw(sz) <<  wskazniki_pomocnicze[i]<<"|";
		}

	cout << endl <<setw(sz*2+1)<<"|";
		for(int i=0; i<il_zmiennych; i++){
			cout << setw(sz) <<  kryt_simplex[i]<<"|";
		}
	cout <<setw(sz)<< wartosc_funkcji_celu <<"|"<< endl;

		for(int k=0; k<12+il_zmiennych*6+12; k++){
			cout << "-";
		}
		cout << endl;
}

int simplex::stworz_tablice(){


	//wyznaczenie wektora wskaznikow pomocniczych, iloraz skalarny
	for(int i=0; i<il_zmiennych; i++){
		wskazniki_pomocnicze[i]=0;
		for(int j=0; j<il_ograniczen; j++){
			wskazniki_pomocnicze[i]+=wspl_celu[ zmienne_bazowe[j] ] * ukl_ograniczen[j][i];
		}
	}

	//wyznaczanie kryterium simplex dla danego wektora bazowego
	for(int i=0; i<il_zmiennych; i++){
		kryt_simplex[i] = wspl_celu[i]-wskazniki_pomocnicze[i];
	}

	//wyznaczanie wartosci funkcji celu dla danego rozwiazania bazowego
	wartosc_funkcji_celu=0;
	for(int i=0; i<il_ograniczen; i++){
		if(wyr_wolne[i]>0)
			wartosc_funkcji_celu+=wspl_celu[ zmienne_bazowe[i] ]*wyr_wolne[i];
	}



	//#############################
	//BRAK SPRAWDZANIA WARUNKU CZY ROZWIAZANIE JEST OPTYMALNE
	//#############################
	if(czy_optymalne_rozwiaznie()){
		cout << "KONIEC" << endl;
		return 0;
		//wypisz_tablice();
	}


	//wyznaczanie zmiennej ktora wejdzie do nowego rozwiazania bazowego
	//szukamy maksymalnego wspolczynnika celu
	//ponadto pomijamy zmienne ktore juz sa w roziwazaniu bazowym
	nowa_zmienna=0;
	double wartosc_nowej_zmiennej=-9999999999;
	for(int i=0; i<il_zmiennych; i++){
		if(kryt_simplex[i]>wartosc_nowej_zmiennej and kryt_simplex[i]>0){
			/*bool czy_w_bazie=false;
			for(int j=0; j<il_ograniczen; j++){
				if(zmienne_bazowe[j] == i){
					czy_w_bazie=true;
				}
			}
			if(!czy_w_bazie){
				nowa_zmienna=i;
				wartosc_nowej_zmiennej=kryt_simplex[nowa_zmienna];
			}*/
			nowa_zmienna=i;
							wartosc_nowej_zmiennej=kryt_simplex[nowa_zmienna];
		}
	}


	//wzyznaczenie kryterium wyjscia, ilorazu wyrazow wolnych i wspolczynnikow odpowiedniej kolumny
	for(int i=0; i<il_ograniczen; i++){
		if(ukl_ograniczen[i][nowa_zmienna] == 0)
			ilorazy[i]=-1;
		else
			ilorazy[i]=wyr_wolne[i]/ukl_ograniczen[i][nowa_zmienna];

	}

	wypisz_tablice();
	//wyznaczenie ktora zmienna usuniemy z rozwiazania bazowego
	//znaleziona stara_zmienna informuje nas ktory element z zmiennej zmienne_bazowe usunac

	double temp=999999999; //dla maksymalizcji szukamy najmniejszego ilorazu

	for(int i=0; i<il_ograniczen; i++){
		//pomijamy te elementy, dla ktorych nie liczylismy ilorazu
		if(ilorazy[i] > 0){
			if(ilorazy[i] < temp){
				temp=ilorazy[i];
				stara_zmienna=i;
			}
		}
	 }

	cout << "nowa:" << nowa_zmienna << endl;
	cout << "stara: " << stara_zmienna << endl;
	//wypisz_tablice();
	 //wprowadzamy nowa zmienna bazowa do rozwiazania
	 zmienne_bazowe[stara_zmienna]=nowa_zmienna;

	 //aktualizacja wspolczynnikow wiersza dla ktorego znalezlismy kryterium wyjscia
	 //wiersz o indeksie "stara_zmienna"

	 wartosc_na_przecieciu=ukl_ograniczen[stara_zmienna][nowa_zmienna];
	 for(int i=0; i<il_zmiennych; i++){
		 ukl_ograniczen[stara_zmienna][i]/=wartosc_na_przecieciu;
	 }
	 wyr_wolne[stara_zmienna]/=wartosc_na_przecieciu;

	 //aktualizacja reszty wierzszy ukladu ograniczen
	 for(int i=0; i<il_ograniczen; i++){
		 //pomijamy wiersz o indeksie stara_zmienna
		 if(i != stara_zmienna){
			 wartosc_na_przecieciu=ukl_ograniczen[i][nowa_zmienna];
			 for(int j=0; j<il_zmiennych; j++){
				 ukl_ograniczen[i][j]-=ukl_ograniczen[stara_zmienna][j]*wartosc_na_przecieciu;
			 }
			 wyr_wolne[i]-=wyr_wolne[stara_zmienna]*wartosc_na_przecieciu;
		 }
	 }

}

bool simplex::czy_optymalne_rozwiaznie(){
	//MAKSYMALIZACJA
	for(int i=0; i<il_zmiennych; i++){
		if(kryt_simplex[i]>0)
			return false;
	}
	return true;
}

void simplex::oblicz(){
	//wstepne rozwiazanie bazowe
		for(int i=0; i<il_ograniczen; i++){
			zmienne_bazowe[i]=i;
		}
		/*int wyraz=0;
		for(int i=0; i<il_zmiennych; i++){
			if(wspl_celu[i] == 0){
				zmienne_bazowe[wyraz++]=i;
			}
		}*/

		while(!czy_optymalne_rozwiaznie()){
			stworz_tablice();
		}
		wypisz_tablice();
		wynik();
}

void simplex::wynik(){
	cout << endl << endl;
	cout << "WYNIK" << endl;
	cout << "Aby zminimalizowac koszt transportu nalezy przetransportowac: " << endl;

	cout << setw(10) << "Ilosc" << setw(20) << "Trasa" << endl;
	for(int i=0; i<il_ograniczen; i++){
		if(wyr_wolne[i] != 0)
			cout <<setw(10) << wyr_wolne[i] << setw(27) << linie_transportowe[zmienne_bazowe[i]] << endl;
	}
}
