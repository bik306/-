#pragma once

#include <SFML/Graphics.hpp>
#include <string>

using namespace sf;
using namespace std;

class Label : public Transformable, public Drawable {
private:
    Font shrift;
    Text tekst;
    void draw(RenderTarget& target, RenderStates sostoyanie) const;
public:
    Label(string tekst, string putKShriftu = "Roboto-VariableFont_wdth,wght.ttf");
    void ustanovitStroku(string novayaStroka);
    void tsentrirovatStroku(Vector2u razmerOkna);
    Vector2f poluchitRazmer();
};