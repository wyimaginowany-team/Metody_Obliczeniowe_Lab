/*********************************/
/*Autorzy: 
 * Mateusz Miotk
 * Sylwia Kaczmarczyk
 * Michał Kulesz
 *Opis:
 * Program po wczytaniu n wezlow od x_0 do x_n oraz podania wartosci
 * funkcji w tych punktach oraz pochodnych wyprowadza wielomian interpolacyjny Hermite'a.
 * Wynik przedstawia w postaci Newtona oraz w postaci ogolnej. 
*********************************/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
/************************************
 * Nazwa funkcji:pokaz_tablice_int
 * Opis wejscia:tablica int x[],N_MAX jako rozmiar tablicy *prompt jako komunikat do wyswietlenia
 * Opis wyjscia:wydrukowana tablica z odpowiednim komunikatem
*************************************/
void pokaz_tablice_int(int x[], unsigned int N_MAX, char *prompt)
{
    unsigned int i;
    printf("%s\n", prompt);
    printf("---------\n");
    for (i = 0; i < N_MAX; i++) {
	printf("|x_%d==%d|\n", i, x[i]);
    }
    printf("---------\n");
}

/************************************
 * Nazwa funkcji:pokaz_tablice_double
 * Opis wejscia:tablica double x[],N_MAX jako rozmiar tablicy *prompt jako komunikat do wyswietlenia
 * prompt1 jako parametr przy wyswietlaniu tablicy:jaka tablica czy A czy B czy D
 * Opis wyjscia:wydrukowana tablica z odpowiednim komunikatem
*************************************/
void pokaz_tablice_double(double x[], unsigned int N_MAX, char *prompt,
			  char *prompt1)
{
    unsigned int i;
    printf("%s\n", prompt);
    printf("------------\n");
    for (i = 0; i < N_MAX; i++) {
	printf("|%s", prompt1);
	printf("%d==%.03lf|\n", i, x[i]);
    }
    printf("------------\n");
}

/************************************
 * Nazwa funkcji:przeszukaj_tablice
 * Opis wejscia:tablica double tab[],N_MAX jako rozmiar tablicy var jako wartosc do sprawdzenia
 * Opis funkcji:Funkcja sprawdza czy wartosc var nie istnieje czasem juz w tablicy tab[]
 * wyrzuca 0 jesli element nie wystepuje lub -1 gdy jest.
 * Opis wyjscia:-1 gdy element wystepuje 0 gdy go nie ma.
*************************************/
int przeszukaj_tablice(double tab[], unsigned int N_MAX, double var)
{
    int i, wynik;
    wynik = 0;
    for (i = 0; i < N_MAX; i++) {
	if (tab[i] == var) {
	    wynik = -1;
	    break;
	}
    }
    return wynik;
}

/************************************
 * Nazwa funkcji:przeszukaj_tablice_int
 * Opis wejscia:tablica int tab[],N_MAX jako rozmiar tablicy var jako wartosc do sprawdzenia
 * Opis funkcji:Funkcja sprawdza czy wartosc var nie istnieje czasem juz w tablicy tab[]
 * wyrzuca 0 jesli element nie wystepuje lub -1 gdy jest.
 * Opis wyjscia:-1 gdy element wystepuje 0 gdy go nie ma.
*************************************/
int przeszukaj_tablice_int(int tab[], unsigned int N_MAX, int var)
{
    int i, wynik;
    wynik = 0;
    for (i = 0; i < N_MAX; i++) {
	if (tab[i] == var) {
	    wynik = -1;
	}
    }
    return wynik;
}

/************************************
 * Nazwa funkcji:wczytaj_x
 * Opis wejscia:tablica int x[],N_MAX jako rozmiar tablicy 
 * Opis funkcji:Funkcja wczytuje do tablicy x wartosci.
 * Funkcja pilnuje aby byly to rozne wartosci
 * Opis wyjscia:BRAK. Otrzymujemy wypelniona tablice x[].
*************************************/
void wczytaj_x(int x[], unsigned int N_MAX)
{
    unsigned int i = 0;
    int temp;
    while (i < N_MAX) {
	printf("x_%d==", i);
	scanf("%d", &temp);
	x[i] = INFINITY;
	if (przeszukaj_tablice_int(x, i, temp) == 0) {
	    x[i] = temp;
	    i++;
	} else {
	    printf("Ta wartosc juz istnieje! Podaj inna! \n");
	}
    }
}

/************************************
 * Nazwa funkcji:wczytaj_A
 * Opis wejscia:tablica x[],tablica double A[],N_MAX jako rozmiar tablicy 
 * Opis funkcji:Funkcja wczytuje do tablicy A wartosci funkcji.
 * Tablica x[] sluzy do wyswietlania jakiemu wezlowi chcemy przypisac wartosc.
 * W tablicy A wartosci sa powielane.
 * Opis wyjscia:BRAK. Otrzymujemy wypelniona tablice A[].
*************************************/
void wczytaj_A(int x[], double A[], unsigned int N_MAX)
{
    unsigned int i = 0, j = 0;
    double temp;
    while (i < 2 * N_MAX) {
	printf("f(%d)==", x[j]);
	scanf("%lf", &temp);
	A[i] = temp;
	A[i + 1] = temp;
	i += 2;
	j++;
    }
}

 /************************************
 * Nazwa funkcji:wczytaj_B
 * Opis wejscia:tablica int x[],tablica double B[],N_MAX jako rozmiar tablicy 
 * Opis funkcji:Funkcja wczytuje do tablicy B wartosci pochodnych.
 * Tablica x[] sluzy do wyswietlania jakiemu wezlowi chcemy przypisac wartosc.
 * Opis wyjscia:BRAK. Otrzymujemy wypelniona tablice B[].
*************************************/
void wczytaj_B(int x[], double B[], unsigned int N_MAX)
{
    unsigned int i = 0;
    double temp;
    while (i < N_MAX) {
	printf("f'(%d)==", x[i]);
	scanf("%lf", &temp);


	B[i] = temp;
	i++;
    }

}

/************************************
 * Nazwa funkcji:wczytaj_D
 * Opis wejscia:tablica double D[] do ktorej beda liczone wartosci,tablica double A[] z wczytanymi wartosciami wezlow z tablicy x[]
 * tablica int x[] z wezlami,tablica double B[] z wczytanymi wartosciami pochodnych wezlow z tablicy x[],N_MAX jako rozmiar tablicy 
 * Opis funkcji:Funkcja wczytuje do tablicy D[] "roznice dzielone" wedlug algorytmu:
 * 1.Na poczatku wczytujemy wartosci do D[] z tablicy A[].
 * 2.Pozniej wykonujemy liczenie roznic dzielonych zgodnie z ich definicja.
 * 2a)Jezeli wychodzi 0/0 to w miejsce tego dzialania(0/0) wpisujemy wartosc pochodnej.
 * 2b)W przeciwnym wypadku wpisujemy uzyskana wartosc.
 * Opis wyjscia:BRAK. Otrzymujemy wypelniona tablice D[].
*************************************/
void wczytaj_D(double D[], double A[], int X[], double B[],
	       unsigned int N_MAX)
{
    unsigned int i, j;
    for (i = 0; i < N_MAX; i++) {
	D[i] = A[i];
    }
    for (j = 1; j < N_MAX; j++) {
	for (i = N_MAX - 1; i >= j; i--) {
	    D[i] = (D[i] - D[i - 1]) / (X[i / 2] - X[(i - j) / 2]);
	    if (D[i] != D[i]) {	// To znaczy że liczba nie jest liczbą
		D[i] = B[i / 2];
	    }
	}
    }
}

/************************************
 * Nazwa funkcji:pobierz_int
 * Opis wejscia:liczba a oraz prompt jako komunikat
 * Opis funkcji:Funkcja wczytuje wartosc int i zapisuje go do *a.
 * Opis wyjscia:BRAK.W *a mamy wczytana wartosc.
*************************************/
void pobierz_int(unsigned int *a, unsigned int K_MAX, char *prompt)
{
    printf("%s", prompt);
    scanf("%d", a);
    while (*a < 0 || *a > K_MAX) {
	printf("ZLA WARTOSC! PODAJ INNA: ");
	scanf("%d", a);
    }


}

/************************************
 * Nazwa funkcji:wypisz_newton
 * Opis wejscia:tablica double D[] z wartosciami wielomianu,tablica x[] z wezlami,N_MAX jako rozmiar tablicy
 * Opis funkcji:Funkcja wypisuje wielomian w postaci Newtona.
 * Opis wyjscia:Wydrukowany wielomian w postaci Newtona.
*************************************/
void wypisz_newton(double D[], int x[], unsigned int N_MAX)
{
    int i, j;
    printf("POSTAC NEWTONA: \n");
    printf("W(x)==");
    for (i = 0; i < N_MAX; i++) {

	if (D[i] != 0 && D[i] != 1) {

	    printf("%.03lf", D[i]);

	}

	for (j = 0; j <= i - 1 && D[i] != 0; j++) {
	    if (x[j / 2] > 0)
		printf("(x-%d)", x[j / 2]);
	    else
		printf("(x+%d)", -1 * x[j / 2]);
	}
	if ((i < N_MAX - 1 && D[i + 1] > 0 && D[i] != 0) || D[i + 1] > 0) {
	    putchar('+');
	}
    }
    putchar('\n');

}

/************************************
 * Nazwa funkcji:wypisz_normalnie
 * Opis wejscia:tablica double D[] z wartosciami wielomianu,tablica x[] z wezlami,N_MAX jako rozmiar tablicy
 * Opis funkcji:Funkcja wypisuje wielomian w postaci normalnej.
 * Opis wyjscia:Wydrukowany wielomian w postaci normalnej.
*************************************/
void wypisz_normalnie(double D[], int x[], unsigned int N_MAX)
{
    double *poly = D;
    int i, j;
    printf("POSTAĆ NORMALNA: \n");
    printf("W(x)==");
    /*Ponizsze dwie petle licza wspolczynniki wielomianu ogolnej postaci z postaci Newtona */
    for (j = N_MAX - 1; j >= 0; j--)
	for (i = j; i < N_MAX - 1; i++)
	    poly[i] = poly[i] - poly[i + 1] * x[j / 2];
    /*Wypisanie wielomianu */
    for (i = 0; i < N_MAX; i++) {
	if (poly[i] != 0 && i > 1) {
	    if (poly[i] != 1)
		printf("%.03lfx^%d", poly[i], i);
	    else
		printf("x^%d", i);
	} else if (i == 1) {
	    if (poly[i] != 1 && poly[i] != 0)
		printf("%.03lfx", poly[i]);
	    else if (poly[i] != 0)
		printf("x");
	} else if (poly[i] != 0) {
	    printf("%.03lf", poly[i]);
	}
	if (i + 1 < N_MAX && D[i + 1] > 0 && poly[i] != 0)
	    putchar('+');
    }
    putchar('\n');
}

int main()
{
    unsigned const int K_MAX = 5;
    unsigned int N_MAX;
    pobierz_int(&N_MAX, K_MAX, "Podaj ilosc wezlow: \n");
    int *x = malloc(N_MAX * sizeof(int));
    double *A = (double *) malloc(2 * N_MAX * sizeof(double));
    double *B = (double *) malloc(N_MAX * sizeof(double));
    double *D = (double *) malloc(2 * N_MAX * sizeof(double));
    wczytaj_x(x, N_MAX);
    //pokaz_tablice_int(x,N_MAX,"Tablica wezlow: ");
    wczytaj_A(x, A, N_MAX);
    //pokaz_tablice_double(A,2*N_MAX,"Tabela wartosci A[]","A_");
    wczytaj_B(x, B, N_MAX);
    //pokaz_tablice_double(B,N_MAX,"Tabela wartosci B[]","B_");
    wczytaj_D(D, A, x, B, 2 * N_MAX);
    //pokaz_tablice_double(D,2*N_MAX,"Tabela wartosci roznic dzielonych: ","D_");
    wypisz_newton(D, x, 2 * N_MAX);
    wypisz_normalnie(D, x, 2 * N_MAX);
    free(x);
    free(A);
    free(B);
    free(D);
    return EXIT_SUCCESS;
}
