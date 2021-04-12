#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <fstream>

#define dl_genu 8 //Dlugosc genow(domyslnie = 8)


using namespace std;



struct Osoba {
    char gen[dl_genu + 1]; // liczby binarne
	//+1 bo 0  to znak koncowy
    unsigned int w; //Wartosc powyzszego ciagu
};
//________________________________________________________________________________________________________________________________________________________________________________________//

void kopiowanie(char gen1[], const char gen2[]);//Kopiowanie tablice gen2 do gen1

unsigned int binNaDec(const char gen[]);//zamienia i wypisuje 0/1

void ustawW(Osoba Ludzie[], const unsigned int licznosc) ;//zapisywanie wszystkim Osobaom w tablicy wartosc ciagu 0/1(gen)

void tworzenieOsoby(Osoba Ludzie[], const unsigned int licznosc) ;//przydzielanie  losowych genów 0/1 na poczatku programu, dla pierwszych osobnikow

void wypiszOsoby(const Osoba Ludzie[], const unsigned int licznosc);//Funkcja wypisujaca na ekran wartosci wszystkich osobnikow

double kwadrat(const double a, const double b, const double c, const double x);//Funkcja wyznaczajaca wartosc funkcji(wsp. a, b, c) dla danego x'a.

void clean(string nazwaPliku){//czysci plik
    remove("results.txt");
}
void Najlepszy(Osoba Ludzie[], const unsigned int licznosc, const double a, const double b, const double c);//Wypisuje parametry najlepszego osobnika

void krzyzuj(char *potomek, const char rodzic1[], const char rodzic2[], const int parametr);//Krzyzowanie dwojga rodzicow(parametr decyduje czy najpierw dziedziczymy po 1 czy 2

void krzyzowanie(Osoba Ludzie[], const unsigned int licznosc, const float pdpKrzyzowania);//Funkcja wywo³ujaca funkcje krzyzuj ( losuje rodzicow i sprawdza pdp krzyzowania)

void mutacja(char gen[], const float pdpMutacji);//Mutowanie poszczegolne elementy(bity) genow

void selekcja(Osoba Ludzie[], const unsigned int licznosc, const double a, const double b, const double c);//Dokonywanie selekcji do nastepnej populacji(opisane w specyfikacji zadania)

//_______________________________________________________M__A__I__N_________________________________________________________________________________________________________________________//
int main() {

    clean("results.txt");
    //f(x)=ax^2+bx+c

    //TUTAJ NALEŻY PODAC WARTOŚCI WSPÓŁCZYNNIKÓW, LICZBE I LICZEBNOSC ORAZ LICZBE URUCHOMIEN PROGRAMU
    double a = 5;
    double b = 7;
    double c = 4;
    const unsigned int liczbaPopulacji = 15;//ilosc populacji
    const unsigned int liczebnoscPopulacji = 10; //Musi byc liczba parzysta! ilosc osobników w populacji
    unsigned int liczbaUruchomien = 5;

    //TUTAJ NALEŻY ZDEFINIOWAĆ PRAWDOPODOBIEŃSTWA
    const float pdpKrzyzowania = 0.8;
    const float pdpMutacji = 0.1;









    if(liczebnoscPopulacji % 2 != 0) {
        cout << "Licznosc populacji musi byc parzysta!" << endl;
        return 1;
    }


    Osoba Ludzie[liczebnoscPopulacji];
    tworzenieOsoby(Ludzie, liczebnoscPopulacji); //Losowa inicjalizacja genow

    for(int k = 0; k < liczbaUruchomien; k++) {
        cout << endl << endl << "Program nr: " << k + 1 << endl;
        for(int i = 0; i < liczbaPopulacji; i++) {
            cout << endl << "Populacja nr: " << i + 1 << endl;
            wypiszOsoby(Ludzie, liczebnoscPopulacji);
            krzyzowanie(Ludzie, liczebnoscPopulacji, pdpKrzyzowania); //Krzyzowanie osobnikow
            for(int j = 0; j < liczebnoscPopulacji; j++)
                mutacja(Ludzie[j].gen, pdpMutacji);
            selekcja(Ludzie, liczebnoscPopulacji, a, b, c);
            ustawW(Ludzie, liczebnoscPopulacji);

            if(i == liczbaPopulacji - 1) //Ostatnia populacja
                Najlepszy(Ludzie, liczebnoscPopulacji, a, b, c);
        }
    }

    return 0;
}





//____________________________________________F__U__N__K__C__J__E__________________________________________________________________________________________________________________________//
void kopiowanie(char gen1[], const char gen2[]) {//Kopiowanie tablice gen2 do gen1
    for(int i = 0; i < dl_genu; i++) {
        if(gen2[i] == 0 || gen2[i] == '0')
            gen1[i] = '0';
        else if(gen2[i] == 1 || gen2[i] == '1')
            gen1[i] = '1';
        else
            gen1[i] = -1; //Niepoprawny
    }
}




unsigned int binNaDec(const char gen[]) {//zamienia i wypisuje 0/1
    unsigned int w = 0;
    char genTmp[dl_genu];
    kopiowanie(genTmp, gen);

    for(int i = dl_genu - 1, j = 0; i >= 0; i--, j++) {
        if(genTmp[i] == '1')
            w += pow(2,j);
    }

    return w;
}




void ustawW(Osoba Ludzie[], const unsigned int licznosc) {//zapisywanie wszystkim Osobaom w tablicy wartosc ciagu 0/1(gen)
    for(int i = 0; i < licznosc; i++)
        Ludzie[i].w = binNaDec(Ludzie[i].gen);
}



void tworzenieOsoby(Osoba Ludzie[], const unsigned int licznosc) {//przydzielanie  losowych genów 0/1 na poczatku programu, dla pierwszych osobnikow
    srand (time(NULL));

    char gen[dl_genu];
    for(int i = 0; i < licznosc; i++) {
        for(int j = 0; j < dl_genu; j++) {
            gen[j] = rand() % 2;
        }
        kopiowanie(Ludzie[i].gen, gen);
        Ludzie[i].w = binNaDec(gen);
        Ludzie[i].gen[dl_genu] = 0;
    }
}



void wypiszOsoby(const Osoba Ludzie[], const unsigned int licznosc) {//Funkcja wypisujaca na ekran wartosci wszystkich osobnikow
    for(int i = 0; i < licznosc; i++)
        cout << i << ": " << Ludzie[i].gen << ", wartosc: " << Ludzie[i].w << endl;
}


double kwadrat(const double a, const double b, const double c, const double x) {
    double y = a * x * x + b * x + c;
    return y;
}



void Najlepszy(Osoba Ludzie[], const unsigned int licznosc, const double a, const double b, const double c) {//Funkcja wyznaczajaca wartosc funkcji(wsp. a, b, c) dla danego x'a.
    double w, maxW = kwadrat(a,b,c,Ludzie[0].w);
    double xMax = Ludzie[0].w;

    for(int i = 1; i < licznosc; i++) {
        w = kwadrat(a,b,c,Ludzie[i].w);
        if(w > maxW) {
            maxW = w;
            xMax = Ludzie[i].w;
        }
    }
    cout << "Wartosc najlepszego osobnika wynosi: " << maxW << ", dla x = " << xMax << endl;

    //Zapis wyniku do pliku:
    ofstream plik;
    plik.open("results.txt", ios::app);

	if (!plik)
		cout << "Nie moge otworzyc pliku!\n";
    plik << xMax << "     " << maxW << endl;
	plik.close();
}


void krzyzuj(char *potomek, const char rodzic1[], const char rodzic2[], const int parametr) {//Krzyzowanie dwojga rodzicow(parametr decyduje czy najpierw dziedziczymy po 1 czy 2
    int punktCiecia = rand() % (dl_genu - 1) + 1;

    if(parametr == 1) {
        for(int i = 0; i < punktCiecia; i++)
            potomek[i] = rodzic1[i];

        for(int i = punktCiecia; i < dl_genu; i++)
            potomek[i] = rodzic2[i];
    }
    else {
        for(int i = 0; i < punktCiecia; i++)
            potomek[i] = rodzic2[i];

        for(int i = punktCiecia; i < dl_genu; i++)
            potomek[i] = rodzic1[i];
    }
}



void krzyzowanie(Osoba Ludzie[], const unsigned int licznosc, const float pdpKrzyzowania) {;//Funkcja wywo³ujaca funkcje krzyzuj ( losuje rodzicow i sprawdza pdp krzyzowania)
    bool zajety[licznosc]; //Czy osobnik zostal juz krzyzowany, czy jeszcze nie
    for(int i = 0; i < licznosc; i++) //Zerowanie tablicy
        zajety[i] = false;

    int index, random;
    char potomek1[dl_genu + 1]; //+ 1  0 to znak koncowy napisu
    char potomek2[dl_genu + 1];
    potomek1[dl_genu] = 0; //Znak konczacy napis - nie element genu
    potomek2[dl_genu] = 0;

    for(int i = 0; i < licznosc/2; i++) { //Do polowy tablicy osobnikow, dla nich losujemy osobnikow z drugiej polowy
        index = rand() % (licznosc/2) + (licznosc/2);
        if(!zajety[index]) {
            zajety[i] = true;
            zajety[index] = true;
            random = rand() % 100 + 1;
            if((float)random <= pdpKrzyzowania * 100) {
                krzyzuj(potomek1, Ludzie[i].gen, Ludzie[index].gen, 1);
                krzyzuj(potomek2, Ludzie[i].gen, Ludzie[index].gen, 2);
                kopiowanie(Ludzie[i].gen, potomek1);
                kopiowanie(Ludzie[index].gen, potomek2);
            }
        }
        else { //jezeli wylosowany osobnik zostal juz wczesniej krzyzowany, nalezy ponowic probe losowania
            i--;
            continue;
        }
    }
}


void mutacja(char gen[], const float pdpMutacji) {//Mutowanie poszczegolne elementy(bity) genow
    int random;

    for(int i = 0; i < dl_genu; i++) {
        random = rand() % 100 + 1;
        if((float)random <= pdpMutacji * 100) {
            if(gen[i] == '0')
                gen[i] = '1';
            else if(gen[i] == '1')
                gen[i] = '0';
        }
    }
}



void selekcja(Osoba Ludzie[], const unsigned int licznosc, const double a, const double b, const double c) {//Dokonywanie selekcji do nastepnej populacji
    double fsuma = 0;
    double f[licznosc];
    double p[licznosc];

    for(int i = 0; i < licznosc; i++) {
        f[i] = kwadrat(a,b,c,Ludzie[i].w);
        fsuma += f[i];
    }

    for(int i = 0; i < licznosc; i++) {
        p[i] = f[i]/fsuma;
    }

    //Losowanie
    Osoba osobnikiTmp[licznosc]; //Ludzie do nowej populacji, pomocniczo
    float random, tmp;

    for(int i = 0; i < licznosc; i++) { //Losujemy tyle osobnikow, ile obecnie istnieje(licznosc)
        //Losujemy liczbe od 0 do 1(w zmiennej random)
        //Suma wszystkich elemetnow tablicy p wynosi 1 (selekcja)
        random = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
        tmp = 0;

        for(int j = 0; j < licznosc; j++) {
            tmp += p[j]; //sumujemy wklady kolejnych osobnikow...
            if(tmp >= random) { //..i sprawdzamy czy zostal wylosowany
                kopiowanie(osobnikiTmp[i].gen, Ludzie[j].gen);
                osobnikiTmp[i].w = Ludzie[j].w;
                break;
            }
        }
    }

    for(int i = 0; i < licznosc; i++) {
        kopiowanie(Ludzie[i].gen, osobnikiTmp[i].gen); //Kopiujemy z tabeli pomocniczej do osobnikow do nastepnej populacji
        Ludzie[i].w = osobnikiTmp[i].w;
    }
}
