#ifndef MENU_HPP
#define MENU_HPP

#include <SFML/Graphics.hpp>
#include "Button.hpp"

using namespace sf;

class Play;

class Menu {
public:
    Menu(RenderWindow& okno);
    ~Menu();

    bool zagruzitShrift(std::string imyaFaila);

    void obrabotatSobytie(Event sobytie);

    void risovat();

    void zakritOkno();
    void pokazatMenuVibora();
    void zapustitIgry(int razmerPolya);

private:
    void zapustit3x3();
    void zapustit5x5();
    void nazadVMenu();

    RenderWindow& okno;
    Button knopkaStart;
    Button knopkaVihod;
    Font shrift;

    Button knopkaVibor1;
    Button knopkaVibor2;
    Button knopkaVibor3;

    bool vGlavnomMenu;

    Play* tekushayaIgra;
    bool vIgry;
};

#endif