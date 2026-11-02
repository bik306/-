#include "GameMenu.h"
using namespace sf;

void game::GameMenu::setInitText(Text& text, String str, float xpos, float ypos) {
    text.setFont(font);
    text.setFillColor(menu_text_color);
    text.setString(str);
    text.setCharacterSize(size_font);
    text.setPosition(xpos, ypos);
    text.setOutlineThickness(3);
    text.setOutlineColor(border_color);
}

void game::GameMenu::AlignMenu(int posx) {
    float nullx = 0;

    for (int i = 0; i < max_menu; i++) {
        if (posx == 0) {
            nullx = 0;
        }
        else if (posx == 1) {
            nullx = mainMenu[i].getLocalBounds().width;
        }
        else if (posx == 2) {
            nullx = mainMenu[i].getLocalBounds().width / 2;
        }
        else {
            nullx = 0;
        }
        mainMenu[i].setPosition(mainMenu[i].getPosition().x - nullx, mainMenu[i].getPosition().y);
    }
}

game::GameMenu::GameMenu(RenderWindow& window, float menux, float menuy, int index, String name[], int sizeFont, int step)
    : mywindow(window), menu_X(menux), menu_Y(menuy), size_font(sizeFont), menu_Step(step)
{
    if (!font.loadFromFile("arial.ttf")) {
        // шрифт не загрузился
        int a = 32;
        exit(a);
    }

    max_menu = index;
    mainMenu = new Text[max_menu];

    int ypos = menu_Y;
    for (int i = 0; i < max_menu; i++) {
        setInitText(mainMenu[i], name[i], menu_X, ypos);
        ypos = ypos + menu_Step;
    }

    mainMenuSelected = 0;
    mainMenu[mainMenuSelected].setFillColor(chose_text_color);
}

void game::GameMenu::MoveUp()
{
    mainMenuSelected--;
    if (mainMenuSelected >= 0) {
        mainMenu[mainMenuSelected].setFillColor(chose_text_color);
        mainMenu[mainMenuSelected + 1].setFillColor(menu_text_color);
    }
    else
    {
        mainMenu[0].setFillColor(menu_text_color);
        mainMenuSelected = max_menu - 1;
        mainMenu[mainMenuSelected].setFillColor(chose_text_color);
    }
}

void game::GameMenu::MoveDown()
{
    mainMenuSelected++;
    if (mainMenuSelected < max_menu) {
        mainMenu[mainMenuSelected - 1].setFillColor(menu_text_color);
        mainMenu[mainMenuSelected].setFillColor(chose_text_color);
    }
    else
    {
        mainMenu[max_menu - 1].setFillColor(menu_text_color);
        mainMenuSelected = 0;
        mainMenu[mainMenuSelected].setFillColor(chose_text_color);
    }
}

void game::GameMenu::draw()
{
    for (int i = 0; i < max_menu; i++) {
        mywindow.draw(mainMenu[i]);
    }
}

void game::GameMenu::setColorTextMenu(Color menColor, Color Chocolor, Color BordColor)
{
    menu_text_color = menColor;
    chose_text_color = Chocolor;
    border_color = BordColor;

    for (int i = 0; i < max_menu; i++) {
        mainMenu[i].setFillColor(menu_text_color);
        mainMenu[i].setOutlineColor(border_color);
    }

    mainMenu[mainMenuSelected].setFillColor(chose_text_color);
}

void game::GameMenu::CheckMouseHover(Vector2i mousePos) {
    for (int i = 0; i < max_menu; i++) {
        FloatRect bounds = mainMenu[i].getGlobalBounds();

        if (bounds.contains(mousePos.x, mousePos.y)) {
            if (mainMenuSelected != i) {
                mainMenu[mainMenuSelected].setFillColor(menu_text_color);
                mainMenuSelected = i;
                mainMenu[mainMenuSelected].setFillColor(chose_text_color);
            }
            break;
        }
    }
}

int game::GameMenu::getMouseSelected(Vector2i mousePos) {
    for (int i = 0; i < max_menu; i++) {
        FloatRect bounds = mainMenu[i].getGlobalBounds();
        if (bounds.contains(mousePos.x, mousePos.y)) {
            return i;
        }
    }
    return -1;
}