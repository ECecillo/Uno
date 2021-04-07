#include "Terrain.h"
#include <cassert>
#include <stdlib.h>
#include <stdarg.h>
#include <iostream>
#include <stdio.h>

#if _WIN32
#include <windef.h>
#include <winbase.h>
#include <wincon.h>
#include <windows.h>
#include <conio.h>
#else
#include <unistd.h>
#include <termios.h>
#include <unistd.h>
#include <ncurses.h>

#endif

#include <iostream>
using namespace std;

void termMove(int x, int y) // deplace le curseur du terminal
{
#ifdef _WIN32
    // Deplace le curseur en haut a gauche du terminal
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD origine = { (SHORT)x, (SHORT)y };
    SetConsoleCursorPosition(console, origine);
#else
    char t[16];
    sprintf(t, "\033[%d;%dH", y, x);
    printf("%s",t);
#endif

}

void termClear()  // efface le terminal
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void termInit()      // configure la saisie : ne pas afficher les caracteres tapes
{
#ifdef _WIN32
    HANDLE console = GetStdHandle(STD_INPUT_HANDLE);
    DWORD mode; GetConsoleMode(console, &mode);
    SetConsoleMode(console, mode & ~ENABLE_LINE_INPUT & ~ENABLE_ECHO_INPUT);
#else
    struct termios ttystate;
    bool state = true;

    //get the terminal state
    tcgetattr(STDIN_FILENO, &ttystate);

    if (state) {
        //turn off canonical mode
        ttystate.c_lflag &= ~ICANON;
        //minimum of number input read.
        ttystate.c_cc[VMIN] = 1;
    }
    else {
        //turn on canonical mode
        ttystate.c_lflag |= ICANON;
    }
    //set the terminal attributes.
    tcsetattr(STDIN_FILENO, TCSANOW, &ttystate);

    struct termios t;
    tcgetattr(STDIN_FILENO, &t);
    t.c_lflag |= ~ECHO;
    tcsetattr(STDIN_FILENO, TCSANOW, &t);

#endif
}

Terrain::Terrain (int dx, int dy) {
    dimx = dx;
    dimy = dy;
    ter = new char[dimx*dimy];
    clear('.');
    termInit();
    
}

void Terrain::clear (char c) {
    cout << "ok";
    for(int i=0;i<dimx;++i)
        for(int j=0;j<dimy;++j)
            print(i,j,c);
}

void Terrain::print (int x, int y, char c) {
    if (x<0) return;
    if (y<0) return;
    if (x>=dimx) return;
    if (y>=dimy) return;
    ter[y*dimx+x] = c;
    
}

/*
 void Terrain::print (int x, int y, char* c) {
    int i=0;
    while (c[i]!='\0') {print(x+i,y,c[i]);++i;}
} 
*/

void Terrain::draw (int x, int y) {
    termMove(0,0);
    for(int j=0;j<dimx;++j) {
       for(int i=0;i<dimy;++i)
       printf("%c",ter[i*dimx+j]);
       printf("\n");
    }
    termMove(0,dimy);
}

#if not defined _WIN32
int kbhit() {
    struct timeval tv;
    fd_set fds;
    tv.tv_sec = 0;
    tv.tv_usec = 0;
    FD_ZERO(&fds);
    FD_SET(STDIN_FILENO, &fds); //STDIN_FILENO is 0
    select(STDIN_FILENO+1, &fds, NULL, NULL, &tv);
    return FD_ISSET(STDIN_FILENO, &fds);
}
#endif

void Terrain::pause() {
#ifdef _WIN32
    system("pause");
#else
    printf("Appuyer sur une touche\n");
    while(!kbhit());
#endif
}

char Terrain::getCh() { // lire un caractere si une touche a ete pressee
    char touche=0;
    //c = wgetch();
#ifdef _WIN32
    if (kbhit())
    {
        DWORD mode;
        DWORD n;
        HANDLE consoleI = GetStdHandle(STD_INPUT_HANDLE);
        GetConsoleMode(consoleI, &mode);
        SetConsoleMode(consoleI, mode & ~ENABLE_LINE_INPUT & ~ENABLE_ECHO_INPUT);
        ReadConsole(consoleI, &touche, 1, &n, NULL);
    
        /* for (i = 0; i < n; i++)
        {
            switch (touche[i].EventType)
            {
            case MOUSE_EVENT:                                          // mouse input
                MOUSE_EVENT_RECORD mer = touche[i].Event.MouseEvent;   // Les infos sur le clique.
                if (mer.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED) // Le joueur a appuyé sur clique gauche.
                {
                    COORD pos = mer.dwMousePosition;
                    if ((pos.X >= (5 / 16 * dimx) && pos.Y >= (3 / 8 * dimy)) &&
                        (pos.X <= (7 / 16 * dimx) && pos.Y <= (5 / 8 * dimy)))
                    { // On clique sur la pioche.
                        piocherCarte();
                    }
                    if ((pos.X >= (13 / 16 * dimx) && pos.Y >= (dimy * 7 / 16)) &&
                        (pos.X <= 15 / 16 * dimx) && (pos.Y <= (9 / 16 * dimy)))
                    { // On clique sur passer.

                    }
                    if ((pos.X >= (1 / 32 * dimx) && pos.Y >= (dimy * 1 / 32)) &&
                        (pos.X <= (1 / 4 * dimx) && (pos.Y <= (1 / 6 * dimy))))
                    { // On clique sur Paramètres.

                    }
                   
                }
                break;
            default:
                ErrorExit("Unknown event type");
                break;
            }
        } */
    }
#else
    if (kbhit())
        touche = fgetc(stdin);
#endif
    return touche;
}
