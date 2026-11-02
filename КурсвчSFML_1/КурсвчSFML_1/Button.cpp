#include "Button.hpp"
#include <iostream>

using namespace sf;

Button::Button()
{
    nazhat = false;
}

bool Button::zagruzitShrift(std::string fail)
{
    bool vseNorm = shrift.loadFromFile(fail);
    return vseNorm;
}

void Button::ustanovitObrabotchik(std::function<void()> obrabotchik)
{
    obrabotchikNazhatiya = obrabotchik;
}

void Button::init(String txt, float x, float y, float w, float h)
{
    pryamougolnik.setSize(Vector2f(w, h));
    pryamougolnik.setPosition(x, y);
    pryamougolnik.setFillColor(Color(209, 179, 181));

    tekst.setString(txt);
    tekst.setFont(shrift);
    tekst.setCharacterSize(24);
    tekst.setFillColor(Color::White);

    FloatRect ramka = tekst.getLocalBounds();
    float centrX = ramka.left + ramka.width / 2.0f;
    float centrY = ramka.top + ramka.height / 2.0f;
    tekst.setOrigin(centrX, centrY);
    tekst.setPosition(x + w - 322 / 2.0f, y + h - 35 / 2.0f);
}

void Button::obrabotatSobytie(Event sobytie, RenderWindow& okno)  // Добавили &
{
    Vector2i mish = Mouse::getPosition(okno);
    float mx = (float)mish.x;
    float my = (float)mish.y;

    bool poverh = pryamougolnik.getGlobalBounds().contains(mx, my);

    if (sobytie.type == Event::MouseButtonPressed)
    {
        if (poverh == true)
        {
            nazhat = true;
            pryamougolnik.setFillColor(Color(103, 55, 58));
        }
    }

    if (sobytie.type == Event::MouseButtonReleased)
    {
        if (nazhat == true && poverh == true)
        {
            if (obrabotchikNazhatiya != NULL)
            {
                obrabotchikNazhatiya();
            }

            pryamougolnik.setFillColor(Color(209, 179, 181));
            nazhat = false;
        }
    }

    if (sobytie.type == Event::MouseMoved)
    {
        if (poverh == true && nazhat == false)
        {
            pryamougolnik.setFillColor(Color(228, 185, 233));
        }
        else
        {
            if (poverh == false && nazhat == false)
            {
                pryamougolnik.setFillColor(Color(209, 179, 181));
            }
        }
    }
}

void Button::risovat(RenderWindow& okno)
{
    okno.draw(pryamougolnik);
    okno.draw(tekst);
}