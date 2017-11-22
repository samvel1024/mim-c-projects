/**
 * "Piętnastka"
 *
 * Program przykładowy do zajęć laboratoryjnych ze Wstępu do programowania.
 * Demonstruje użycie biblioteki nCurses.
 *
 * Program kompilujemy poleceniem
 *
 * gcc -std=c11 -pedantic -Wall -Wextra -Werror -lncurses pietnascie.c -o pietnascie
 *
 * Po uruchomieniu program losuje stan początkowy łamigłówki.
 * Przerywamy mu, naciskając dowolny klawisz.
 * Układamy łamigłówkę za pomocą klawiszy strzałek.
 * Kończymy pracę naciskając klawisz Escape.
 * Program na bieżąco wyświetla liczbę wykonanych ruchów i liczbę klocków na
 * właściwych polach docelowych.
 *
 * Ograniczenia:
 * - program nie reaguje na zmianę rozmiaru okna terminala.
 *
 * autor: Artur Zaroda <zaroda@mimuw.edu.pl>
 * wersja: 1.0
 * data: 21 listopada 2017
 */

#include <ncurses.h>

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/**
 * Liczba wierszy i kolumn planszy.
 */
#define WIERSZE 4
#define KOLUMNY 4

/**
 * Reprezentacja pustego pola.
 */
#define PUSTE 0

/**
 * Liczba znaków opisujących jedno pole na ekranie.
 */
#define JEDNA 3

/**
 * Liczba cyfr wyniku, czyli liczby klocków na właściwych miejscach.
 *
 * Zależy od wartości @WIERSZE i @KOLUMNY.
 */
#define CYFRY_WYNIKU 2

/**
 * Liczba przestawień wykonywanych na raz podczas mieszania łamigłówki.
 */
#define PORCJA 20

/**
 * Używane do losowania ruchów podczas mieszania łamigłówki.
 */
#define MONETA 2

/**
 * Klawisze sterujące.
 */
#define WYJSCIE 27
#define LEWO KEY_LEFT
#define PRAWO KEY_RIGHT
#define GORA KEY_UP
#define DOL KEY_DOWN

/**
 * Komunikat o konieczności naciśnięcia dowolnego klawisza.
 */
#define MSG_NACISNIJ "Nacisnij dowolny klawisz ..."

/**
 * Reprezentacja planszy.
 */
typedef int plansza[WIERSZE][KOLUMNY];

/**
 * Daje pseudolosową liczbę całkowitą z przedziału od 0 do @n - 1.
 */
int randof(int n) {
  return rand() / (RAND_MAX + 1.0) * n;
}

/**
 * Pisze @x jako zawartość pola o współrzędnych @w i @k.
 */
void pisz(int w, int k, int x) {
  int sw = (LINES - WIERSZE) / 2 + w;
  int sk = (COLS - JEDNA * KOLUMNY) / 2 + JEDNA * k;

  if (x == PUSTE) {
    mvprintw(sw, sk, "%*s", JEDNA, "");
  } else {
    mvprintw(sw, sk, "%*d", JEDNA, x);
  }
}

/**
 * Rysuje planszę @p.
 */
void rysuj(plansza p) {
  int w;

  for (w = 0; w < WIERSZE; ++w) {
    int k;

    for (k = 0; k < KOLUMNY; ++k) {
      pisz(w, k, p[w][k]);
    }
  }
}

/**
 * Sprawdza, czy @w i @k to poprawne współrzędne pola.
 */
bool poprawne(int w, int k) {
  return (0 <= w) && (w < WIERSZE) && (0 <= k) && (k < KOLUMNY);
}

/**
 * Sprawdza, czy @w i @k to właściwe współrzędne dla klocka numer @x.
 */
bool docelowa(int w, int k, int x) {
  return (x == 1 + w * KOLUMNY + k);
}

/**
 * Pisze liczbę ruchów @r.
 */
void ruchy(int r) {
  mvprintw(0, 0, "%d", r);
}

/**
 * Pisze @n jako liczbę klocków na właściwych miejscach.
 */
void wynik(int n) {
  mvprintw(0, COLS - CYFRY_WYNIKU, "%*d", CYFRY_WYNIKU, n);
}

/**
 * Czyści na ekranie wiersz @w.
 */
void czysc(int w) {
  move(w, 0);
  clrtoeol();
}

/**
 * Próbuje przesunąć w łamigłówce @p wolne pole o wektor o współrzędnych
 * @dw i @dk.
 *
 * Aktualizuje współrzędne pustego pola @pw i @pk, liczbę klocków na
 * właściwych miejscach @n oraz liczbę ruchów @r.
 * Nie zmienia stanu planszy, jeśli ruch nie jest legalny.
 */
void przesun(plansza p, int dw, int dk, int *pw, int *pk, int *n, int *r) {
  int nw = *pw + dw;
  int nk = *pk + dk;

  if (poprawne(nw, nk)) {
    int dn = 0;

    ++*r;
    if (docelowa(nw, nk, p[nw][nk])) {
      dn = -1;
    } else if (docelowa(*pw, *pk, p[nw][nk])) {
      dn = 1;
    }
    p[*pw][*pk] = p[nw][nk];
    p[nw][nk] = PUSTE;
    *n += dn;
    ruchy(*r);
    if (dn != 0) {
      wynik(*n);
    }
    pisz(*pw, *pk, p[*pw][*pk]);
    pisz(nw, nk, p[nw][nk]);
    refresh();
    *pw = nw;
    *pk = nk;
  }
}

/**
 * Zapisuje w @p ułożoną łamigłówkę z wolnym polem o współrzędnych @pw i @pk
 * oraz @n klockami na właściwych miejscach.
 */
void ulozona(plansza p, int *pw, int *pk, int *n) {
  int w;
  int x = 1;

  for (w = 0; w < WIERSZE; ++w) {
    int k;

    for (k = 0; k < KOLUMNY; ++k) {
      p[w][k] = x;
      ++x;
    }
  }
  *pw = WIERSZE - 1;
  *pk = KOLUMNY - 1;
  p[*pw][*pk] = PUSTE;
  *n = WIERSZE * KOLUMNY - 1;
}

/**
 * Losuje wektor przesunięcia klocka, zapisując współrzędne na @dw i @dk.
 */
void losuj(int *dw, int *dk) {
  int d = 2 * randof(MONETA) - 1;

  if (randof(MONETA) == 0) {
    *dw = d;
    *dk = 0;
  } else {
    *dw = 0;
    *dk = d;
  }
}

/**
 * Miesza łamigłówkę @p, zapisując na @pw i @pk współrzędne pustego pola
 * a na @n liczbę klocków na właściwych polach.
 */
void mieszaj(plansza p, int *pw, int *pk, int *n) {
  int r = 0;

  rysuj(p);
  mvprintw(LINES - 1, (COLS - strlen(MSG_NACISNIJ)) / 2, "%s", MSG_NACISNIJ);
  ruchy(r);
  wynik(*n);
  refresh();
  halfdelay(1);
  while (getch() == ERR) {
    int dw, dk, i;

    for (i = 0; i < PORCJA; ++i) {
      losuj(&dw, &dk);
      przesun(p, dw, dk, pw, pk, n, &r);
    }
  }
  cbreak();
  czysc(LINES - 1);
}

/**
 * Prowadzi rozgrywkę na planszy @p, z wolnym polem o współrzędnych @pw i @pk.
 *
 * Na planszy jest @n klocków na właściwych polach.
 */
void graj(plansza p, int *pw, int *pk, int *n) {
  int c;
  int r = 0;

  czysc(0);
  ruchy(r);
  wynik(*n);
  refresh();
  while ((c = getch()) != WYJSCIE) {
    int dw = 0;
    int dk = 0;

    switch (c) {
      case LEWO:
        dk = 1;
        break;
      case PRAWO:
        dk = -1;
        break;
      case GORA:
        dw = 1;
        break;
      case DOL:
        dw = -1;
        break;
      default:
        flash();
        continue;
    }
    przesun(p, dw, dk, pw, pk, n, &r);
  }
}

/**
 * Rozpoczyna pracę tekstowego interfejsu użytkownika.
 */
void inicjuj_tui(void) {
  initscr();
  noecho();
  keypad(stdscr, true);
  curs_set(0);
  cbreak();
}

/**
 * Kończy pracę tekstowego interfejsu użytkownika.
 */
void zamknij_tui(void) {
  endwin();
}

/**
 * Uruchamia łamigłówkę "Piętnastka".
 *
 * Pracuje w trybie tekstowym korzystając z biblioteki nCurses.
 */
int main(void) {
  plansza p;
  int pw, pk, n;

  srand((unsigned) time(NULL));
  inicjuj_tui();
  ulozona(p, &pw, &pk, &n);
  mieszaj(p, &pw, &pk, &n);
  graj(p, &pw, &pk, &n);
  zamknij_tui();
  return 0;
}
