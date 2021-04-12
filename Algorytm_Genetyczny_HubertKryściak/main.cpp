#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <fstream>

#define genes_lenght 8 //TUTAJ NALEŻY ZDEFINIOWAĆ DŁUGOŚĆ GENÓW


using namespace std;



struct Osoba
{
    char gene[genes_lenght + 1]; // liczby binarne
    //+1 bo 0  to znak koncowy
    unsigned int w; //Wartosc powyzszego ciagu
};
////////////////////////////////////////////////////////////////////////////////////////////////

void clean(string nazwaPliku) //czysci plik
{
    remove("results.txt");
}

void kopiowanie(char gen1[], const char gen2[])  //Kopiowanie tablice gen2 do gen1
{
    for(int i = 0; i < genes_lenght; i++)
    {
        if(gen2[i] == 0 || gen2[i] == '0')
            gen1[i] = '0';
        else if(gen2[i] == 1 || gen2[i] == '1')
            gen1[i] = '1';
        else
            gen1[i] = -1; //Niepoprawny
    }
}

unsigned int binNaDec(const char gene[])  //zamienia i wypisuje 0/1
{
    unsigned int w = 0;
    char genTmp[genes_lenght];
    kopiowanie(genTmp, gene);

    for(int i = genes_lenght - 1, j = 0; i >= 0; i--, j++)
    {
        if(genTmp[i] == '1')
            w += pow(2,j);
    }

    return w;
}

void ustawW(Osoba Ludzie[], const unsigned int licznosc)  //zapisywanie wszystkim Osobaom w tablicy wartosc ciagu 0/1(gene)
{
    for(int i = 0; i < licznosc; i++)
        Ludzie[i].w = binNaDec(Ludzie[i].gene);
}

void tworzenieOsoby(Osoba Ludzie[], const unsigned int licznosc)  //przydzielanie  losowych genów 0/1 na poczatku programu, dla pierwszych osobnikow
{
    srand (time(NULL));

    char gene[genes_lenght];
    for(int i = 0; i < licznosc; i++)
    {
        for(int j = 0; j < genes_lenght; j++)
        {
            gene[j] = rand() % 2;
        }
        kopiowanie(Ludzie[i].gene, gene);
        Ludzie[i].w = binNaDec(gene);
        Ludzie[i].gene[genes_lenght] = 0;
    }
}

void wypiszOsoby(const Osoba Ludzie[], const unsigned int licznosc)  //Funkcja wypisujaca na ekran wartosci wszystkich osobnikow
{
    for(int i = 0; i < licznosc; i++)
        cout << i << ": " << Ludzie[i].gene << ", wartosc: " << Ludzie[i].w << endl;
}

double kwadrat(const double a, const double b, const double c, const double x)
{
    double y = a * x * x + b * x + c;
    return y;
}

void Najlepszy(Osoba Ludzie[], const unsigned int licznosc, const double a, const double b, const double c)  //Funkcja wyznaczajaca wartosc funkcji(wsp. a, b, c) dla danego x'a.
{
    double w, maxW = kwadrat(a,b,c,Ludzie[0].w);
    double xMax = Ludzie[0].w;

    for(int i = 1; i < licznosc; i++)
    {
        w = kwadrat(a,b,c,Ludzie[i].w);
        if(w > maxW)
        {
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

void krzyzuj(char *potomek, const char rodzic1[], const char rodzic2[], const int parametr)  //Krzyzowanie dwojga rodzicow(parametr decyduje czy najpierw dziedziczymy po 1 czy 2
{
    int punktCiecia = rand() % (genes_lenght - 1) + 1;

    if(parametr == 1)
    {
        for(int i = 0; i < punktCiecia; i++)
            potomek[i] = rodzic1[i];

        for(int i = punktCiecia; i < genes_lenght; i++)
            potomek[i] = rodzic2[i];
    }
    else
    {
        for(int i = 0; i < punktCiecia; i++)
            potomek[i] = rodzic2[i];

        for(int i = punktCiecia; i < genes_lenght; i++)
            potomek[i] = rodzic1[i];
    }
}

void krzyzowanie(Osoba Ludzie[], const unsigned int licznosc, const float pdpKrzyzowania)
{
    ;//Funkcja wywo³ujaca funkcje krzyzuj ( losuje rodzicow i sprawdza pdp krzyzowania)
    bool zajety[licznosc]; //Czy osobnik zostal juz krzyzowany, czy jeszcze nie
    for(int i = 0; i < licznosc; i++) //Zerowanie tablicy
        zajety[i] = false;

    int index, random;
    char potomek1[genes_lenght + 1]; //+ 1  0 to znak koncowy napisu
    char potomek2[genes_lenght + 1];
    potomek1[genes_lenght] = 0; //Znak konczacy napis - nie element genu
    potomek2[genes_lenght] = 0;

    for(int i = 0; i < licznosc/2; i++)   //Do polowy tablicy osobnikow, dla nich losujemy osobnikow z drugiej polowy
    {
        index = rand() % (licznosc/2) + (licznosc/2);
        if(!zajety[index])
        {
            zajety[i] = true;
            zajety[index] = true;
            random = rand() % 100 + 1;
            if((float)random <= pdpKrzyzowania * 100)
            {
                krzyzuj(potomek1, Ludzie[i].gene, Ludzie[index].gene, 1);
                krzyzuj(potomek2, Ludzie[i].gene, Ludzie[index].gene, 2);
                kopiowanie(Ludzie[i].gene, potomek1);
                kopiowanie(Ludzie[index].gene, potomek2);
            }
        }
        else   //jezeli wylosowany osobnik zostal juz wczesniej krzyzowany, nalezy ponowic probe losowania
        {
            i--;
            continue;
        }
    }
}

void mutacja(char gene[], const float pdpMutacji)  //Mutowanie poszczegolne elementy(bity) genow
{
    int random;

    for(int i = 0; i < genes_lenght; i++)
    {
        random = rand() % 100 + 1;
        if((float)random <= pdpMutacji * 100)
        {
            if(gene[i] == '0')
                gene[i] = '1';
            else if(gene[i] == '1')
                gene[i] = '0';
        }
    }
}

void selekcja(Osoba Ludzie[], const unsigned int licznosc, const double a, const double b, const double c)  //Dokonywanie selekcji do nastepnej populacji
{
    double fsuma = 0;
    double f[licznosc];
    double p[licznosc];

    for(int i = 0; i < licznosc; i++)
    {
        f[i] = kwadrat(a,b,c,Ludzie[i].w);
        fsuma += f[i];
    }

    for(int i = 0; i < licznosc; i++)
    {
        p[i] = f[i]/fsuma;
    }

    //Losowanie
    Osoba osobnikiTmp[licznosc]; //Ludzie do nowej populacji, pomocniczo
    float random, tmp;

    for(int i = 0; i < licznosc; i++)   //Losujemy tyle osobnikow, ile obecnie istnieje(licznosc)
    {
        //Losujemy liczbe od 0 do 1(w zmiennej random)
        //Suma wszystkich elemetnow tablicy p wynosi 1 (selekcja)
        random = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
        tmp = 0;

        for(int j = 0; j < licznosc; j++)
        {
            tmp += p[j]; //sumujemy wklady kolejnych osobnikow...
            if(tmp >= random)   //..i sprawdzamy czy zostal wylosowany
            {
                kopiowanie(osobnikiTmp[i].gene, Ludzie[j].gene);
                osobnikiTmp[i].w = Ludzie[j].w;
                break;
            }
        }
    }

    for(int i = 0; i < licznosc; i++)
    {
        kopiowanie(Ludzie[i].gene, osobnikiTmp[i].gene); //Kopiujemy z tabeli pomocniczej do osobnikow do nastepnej populacji
        Ludzie[i].w = osobnikiTmp[i].w;
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int main()
{

    clean("results.txt");

    //TUTAJ NALEŻY PODAC WARTOŚCI WSPÓŁCZYNNIKÓW, LICZBE I LICZEBNOSC ORAZ LICZBE URUCHOMIEN PROGRAMU
    double a = 5;
    double b = 7;
    double c = 4;
    const unsigned int liczbaPopulacji = 15; //TUTAJ NALEŻY ZDEFINIOWAĆ ILOŚĆ POPULACJI
    const unsigned int liczebnoscPopulacji = 10; //TUTAJ NALEŻY ZDEFINIOWAĆ PARZYSTĄ LICZBĘ OSOBNIKÓW W POPULACJI
    unsigned int liczbaUruchomien = 5; //TUTAJ NALEŻY ZDEFINIOWAĆ LICZBĘ URUCHOMIEŃ PROGRAMU

    //TUTAJ NALEŻY ZDEFINIOWAĆ PRAWDOPODOBIEŃSTWA KRZYŻOWANIA I MUTACJI
    const float pdpKrzyzowania = 0.8;
    const float pdpMutacji = 0.1;


    if(liczebnoscPopulacji % 2 != 0) //ZABEZPIECZENIE PRZED NIEPARZYSTĄ LICZBĄ OSOBNIKÓW
    {
        cout << "Licznosc populacji musi byc parzysta!" << endl;
        return 1;
    }


    Osoba Ludzie[liczebnoscPopulacji]; //TWORZY TABLICĘ STRUKTUR
    tworzenieOsoby(Ludzie, liczebnoscPopulacji); //Losowa inicjalizacja genow

    for(int k = 0; k < liczbaUruchomien; k++)
    {
        cout << endl << endl << "Program nr: " << k + 1 << endl;
        for(int i = 0; i < liczbaPopulacji; i++)
        {
            cout << endl << "Populacja nr: " << i + 1 << endl;
            wypiszOsoby(Ludzie, liczebnoscPopulacji);
            krzyzowanie(Ludzie, liczebnoscPopulacji, pdpKrzyzowania); //Krzyzowanie osobnikow
            for(int j = 0; j < liczebnoscPopulacji; j++)
                mutacja(Ludzie[j].gene, pdpMutacji);
            selekcja(Ludzie, liczebnoscPopulacji, a, b, c);
            ustawW(Ludzie, liczebnoscPopulacji);

            if(i == liczbaPopulacji - 1) //Ostatnia populacja
                Najlepszy(Ludzie, liczebnoscPopulacji, a, b, c);
        }
    }

    return 0;
}





