#ifndef BUTTON_HPP
#define BUTTON_HPP

#include <SFML/Graphics.hpp>
#include <functional>
#include <string>

using namespace sf;

class Button {
public:
    Button();

    bool zagruzitShrift(std::string fail);

    void init(String txt, float x, float y, float w, float h);

    void obrabotatSobytie(Event sobytie, RenderWindow& okno);  // Добавили &

    void risovat(RenderWindow& okno);

    void ustanovitObrabotchik(std::function<void()> obrabotchik);

private:
    RectangleShape pryamougolnik;
    Text tekst;
    Font shrift;
    bool nazhat;
    std::function<void()> obrabotchikNazhatiya;
};

#endif