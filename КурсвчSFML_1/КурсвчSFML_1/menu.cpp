#include "menu.hpp"
#include "play.hpp"
#include <functional>

using namespace sf;

Menu::Menu(RenderWindow& win) : okno(win)
{
    tekushayaIgra = nullptr;
    vIgry = false;

    float shirinaOkna = (float)okno.getSize().x;
    float visotaOkna = (float)okno.getSize().y;
    float shirinaKnopki = 250;
    float visotaKnopki = 50;
    float centrX = (shirinaOkna - shirinaKnopki) / 2.0f;
    float startY = (visotaOkna - visotaKnopki * 2 - 20) / 2.0f;

    knopkaStart.init(L"Начать", centrX, startY - 50, shirinaKnopki, visotaKnopki);
    knopkaVihod.init(L"Выход", centrX, startY + visotaKnopki + 30, shirinaKnopki, visotaKnopki);

    knopkaVihod.ustanovitObrabotchik(std::bind(&Menu::zakritOkno, this));
    knopkaStart.ustanovitObrabotchik(std::bind(&Menu::pokazatMenuVibora, this));

    float vyborY = (visotaOkna - visotaKnopki * 3 - 40) / 2.0f;
    knopkaVibor1.init(L"3x3", centrX, startY - 50, shirinaKnopki, visotaKnopki);
    knopkaVibor2.init(L"5x5", centrX, vyborY + visotaKnopki + 20, shirinaKnopki, visotaKnopki);
    knopkaVibor3.init(L"Назад", centrX, vyborY + visotaKnopki + 110, shirinaKnopki, visotaKnopki);

    knopkaVibor1.ustanovitObrabotchik(std::bind(&Menu::zapustit3x3, this));
    knopkaVibor2.ustanovitObrabotchik(std::bind(&Menu::zapustit5x5, this));
    knopkaVibor3.ustanovitObrabotchik(std::bind(&Menu::nazadVMenu, this));

    vGlavnomMenu = true;
}

Menu::~Menu()
{
    if (tekushayaIgra != nullptr)
    {
        delete tekushayaIgra;
    }
}

bool Menu::zagruzitShrift(std::string fail)
{
    bool zagruzilos = shrift.loadFromFile(fail);
    if (zagruzilos == false)
    {
        return false;
    }

    knopkaStart.zagruzitShrift(fail);
    knopkaVihod.zagruzitShrift(fail);
    knopkaVibor1.zagruzitShrift(fail);
    knopkaVibor2.zagruzitShrift(fail);
    knopkaVibor3.zagruzitShrift(fail);
    return true;
}

void Menu::obrabotatSobytie(Event sobytie)
{
    if (vIgry == true && tekushayaIgra != nullptr)
    {
        tekushayaIgra->obrabotatSobytie(sobytie);
        if (tekushayaIgra->zakonchena() == true)
        {
            delete tekushayaIgra;
            tekushayaIgra = nullptr;
            vIgry = false;
            vGlavnomMenu = true;
        }
        return;
    }

    if (vGlavnomMenu == true)
    {
        knopkaStart.obrabotatSobytie(sobytie, okno);
        knopkaVihod.obrabotatSobytie(sobytie, okno);
    }
    else
    {
        knopkaVibor1.obrabotatSobytie(sobytie, okno);
        knopkaVibor2.obrabotatSobytie(sobytie, okno);
        knopkaVibor3.obrabotatSobytie(sobytie, okno);
    }
}

void Menu::risovat()
{
    if (vIgry == true && tekushayaIgra != nullptr)
    {
        tekushayaIgra->risovat();
        return;
    }

    if (vGlavnomMenu == true)
    {
        knopkaStart.risovat(okno);
        knopkaVihod.risovat(okno);
    }
    else
    {
        knopkaVibor1.risovat(okno);
        knopkaVibor2.risovat(okno);
        knopkaVibor3.risovat(okno);
    }
}

void Menu::zakritOkno()
{
    okno.close();
}

void Menu::pokazatMenuVibora()
{
    vGlavnomMenu = false;
}

void Menu::zapustitIgry(int razmerPolya)
{
    if (tekushayaIgra != nullptr)
    {
        delete tekushayaIgra;
    }
    tekushayaIgra = new Play(okno, razmerPolya);
    vIgry = true;
}

void Menu::zapustit3x3()
{
    zapustitIgry(3);
}

void Menu::zapustit5x5()
{
    zapustitIgry(5);
}

void Menu::nazadVMenu()
{
    vGlavnomMenu = true;
}