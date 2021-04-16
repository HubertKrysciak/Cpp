#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <fstream>

#define genes_lenght 8 //TUTAJ NALEŻY ZDEFINIOWAĆ DŁUGOŚĆ GENÓW


using namespace std;



struct Osoba
{
    char gene[genes_lenght + 1]; //CIAG GENÓW
    //+1 (ZNAK KOŃCOWY)
    unsigned int w; //WARTOSC CIĄGU
};
////////////////////////////////////////////////////////////////////////////////////////////////

void clean(string nazwaPliku) //UFNKCJIA DO CZYSZCZENIA PLIKU Z WYNIKAMI
{
    remove("results.txt");
}

void kopiowanie(char gen1[], const char gen2[])  //KOPIOWANIE TABLICY GEN1 DO GEN2
{
    for(int i = 0; i < genes_lenght; i++)
    {
        if(gen2[i] == 0 || gen2[i] == '0')
            gen1[i] = '0';
        else if(gen2[i] == 1 || gen2[i] == '1')
            gen1[i] = '1';
        else
            gen1[i] = -1; //NIEPOPRAWNY
    }
}

unsigned int binNaDec(const char gene[])  //KONWERTER BINNADEC
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

void ustawWartosc(Osoba Ludzie[], const unsigned int licznosc)  //USTAWIANIE WARTOŚCI WSZYSTKIM OSOBOM W TABLICY (0/1)
{
    for(int i = 0; i < licznosc; i++)
        Ludzie[i].w = binNaDec(Ludzie[i].gene);
}

void tworzenieOsoby(Osoba Ludzie[], const unsigned int licznosc)  //PRZYDZIELANIE LOSOWYCH GENÓW DLA PIERWSZYCH OSOBNIKÓW
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

//void wypiszOsoby(const Osoba Ludzie[], const unsigned int licznosc)  //Funkcja wypisujaca na ekran wartosci wszystkich osobnikow(napisane dla testów w trakcie żeby było łatwiej)
//{
//    for(int i = 0; i < licznosc; i++)
//        cout << i << ": " << Ludzie[i].gene << ", wartosc: " << Ludzie[i].w << endl;
//}

double kwadrat(const double a, const double b, const double c, const double x)
{
    double y = a * x * x + b * x + c;
    return y;
}

void Najlepszy(Osoba Ludzie[], const unsigned int licznosc, const double a, const double b, const double c)  //WYZNACZANIE WARTOŚCI FUNKCJI KWADRATOWEJ DLA DANEGO X
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

    //ZAPIS DO PLIKU(WYNIK)
    ofstream plik;
    plik.open("results.txt", ios::app);

    if (!plik)
        cout << "Nie moge otworzyc pliku!\n";
    plik << xMax << "     " << maxW << endl;
    plik.close();
}

void krzyzuj(char *potomek, const char rodzic1[], const char rodzic2[], const int parametr)  //KRYŻOWANIE RODZICÓW. parametr DECYDUJE, CZY NAJPIERW DZIEDZICZONE JEST PO PIERWSZYM CZY PO DRUGIM RODZICU
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

void krzyzowanie(Osoba Ludzie[], const unsigned int licznosc, const float pdpK)//LOSOWANIE RODZICÓW I SPRAWDZANIE PRAWDOPODOBIEŃSTWO KRZYŻOWANIA
{
    bool zajety[licznosc]; //SPRAWDZENIE CZY OSOBNIK BYŁ JUŻ KRZYŻOWANY
    for(int i = 0; i < licznosc; i++) //ZEROWANIE
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
            if((float)random <= pdpK * 100)
            {
                krzyzuj(potomek1, Ludzie[i].gene, Ludzie[index].gene, 1);
                krzyzuj(potomek2, Ludzie[i].gene, Ludzie[index].gene, 2);
                kopiowanie(Ludzie[i].gene, potomek1);
                kopiowanie(Ludzie[index].gene, potomek2);
            }
        }
        else   //PONOWIENIE LOSOWANIA JEŻELI OSOBNIK ZOSTAŁ JUŻ WCZEŚNIEJ WYLOSOWANY
        {
            i--;
            continue;
        }
    }
}

void mutacja(char gene[], const float pdpM)  //MUTOWANIE POSZCZEGÓLNYCH GENÓW
{
    int random;

    for(int i = 0; i < genes_lenght; i++)
    {
        random = rand() % 100 + 1;
        if((float)random <= pdpM * 100)
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

    //LOSOWANIE
    Osoba osobnikiTmp[licznosc]; //TABELA POMOCNICZA, LUDZIE DO NOWEJ POPULACJI
    float random, tmp;

    for(int i = 0; i < licznosc; i++)   //LOSUJEMY OSOBNIKÓW W LICZBIE TAKIEJ SAMEJ JAK ZMIENNA LICZNOSC
    {
        //LOSOWANIE 0 1
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
        kopiowanie(Ludzie[i].gene, osobnikiTmp[i].gene); //KOPIOWANIE OSOBNIKÓW DO NASTĘPNEJ POPULACJI
        Ludzie[i].w = osobnikiTmp[i].w;
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int main()
{

    clean("results.txt");

    //TUTAJ NALEŻY PODAC WARTOŚCI WSPÓŁCZYNNIKÓW, LICZBE I LICZEBNOSC ORAZ LICZBE PRZEJŚĆ ALGORYTMU
    double a = 5;
    double b = 7;
    double c = 4;
    const unsigned int liczbaPop = 15; //TUTAJ NALEŻY ZDEFINIOWAĆ ILOŚĆ POPULACJI
    const unsigned int liczebnoscPop = 10; //TUTAJ NALEŻY ZDEFINIOWAĆ PARZYSTĄ LICZBĘ OSOBNIKÓW W POPULACJI
    unsigned int liczbaUruchomien = 5; //TUTAJ NALEŻY ZDEFINIOWAĆ LICZBĘ PRZEJŚĆ ALGORYTMU

    //TUTAJ NALEŻY ZDEFINIOWAĆ PRAWDOPODOBIEŃSTWA KRZYŻOWANIA I MUTACJI
    const float pdpK = 0.8;
    const float pdpM = 0.1;


    if(liczebnoscPop % 2 != 0) //ZABEZPIECZENIE PRZED NIEPARZYSTĄ LICZBĄ OSOBNIKÓW
    {
        cout << "Licznosc populacji musi byc parzysta!" << endl;
        return 1;
    }


    Osoba Ludzie[liczebnoscPop]; //TWORZY TABLICĘ STRUKTUR
    tworzenieOsoby(Ludzie, liczebnoscPop); //Losowa inicjalizacja genow

    for(int k = 0; k < liczbaUruchomien; k++)
    {
        cout << endl << endl << "Przejscie: " << k + 1 << endl;
        for(int i = 0; i < liczbaPop; i++)
        {
            //cout << endl << "Populacja nr: " << i + 1 << endl;
            //wypiszOsoby(Ludzie, liczebnoscPop);
            krzyzowanie(Ludzie, liczebnoscPop, pdpK); //WYWOŁANIE KRZYŻOWANIA
            for(int j = 0; j < liczebnoscPop; j++)
                mutacja(Ludzie[j].gene, pdpM);
            selekcja(Ludzie, liczebnoscPop, a, b, c);
            ustawWartosc(Ludzie, liczebnoscPop);

            if(i == liczbaPop - 1) //NAJLEPSZY OSOBNIK Z POPULACJI
                Najlepszy(Ludzie, liczebnoscPop, a, b, c);
        }
    }

    return 0;
}
