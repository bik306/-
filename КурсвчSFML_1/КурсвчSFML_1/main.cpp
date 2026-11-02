#include <SFML/Graphics.hpp>
#include <Windows.h>
#include "GameMenu.h"
#include <cstdlib>
#include <ctime>
#include <string>

using namespace sf;

void InitText(Text& mtext, float xpos, float ypos, String str, int size_font = 60, Color menu_text_color = Color::White, int bord = 0, Color border_color = Color(206, 152, 151)) {
	mtext.setCharacterSize(size_font);
	mtext.setPosition(xpos, ypos);
	mtext.setString(str);
	mtext.setFillColor(menu_text_color);
	mtext.setOutlineThickness(bord);
	mtext.setOutlineColor(border_color);
}

int razmer_polya = 3;
char mass[5][5];
int vibor_razmera = 0;
bool igra_zapushena = false;
bool pobeda = false;

Texture texture_vkl;
Texture texture_vikl;

void vivodim(RenderWindow& window, Font& font, char mass[5][5], int razmer) {
	float startX = 550;
	float startY = 100;
	float cellSize = 140;
	float cellSize5 = 100;
	float cellW = 120;
	float cellW5 = 85;

	for (int i = 0; i < razmer; i++) {
		for (int j = 0; j < razmer; j++) {
			RectangleShape cell(Vector2f(razmer == 3 ? cellW : cellW5, razmer == 3 ? cellW : cellW5));
			cell.setPosition(startX + j * (razmer == 3 ? cellSize : cellSize5), startY + i * (razmer == 3 ? cellSize : cellSize5));
			cell.setFillColor(Color(50, 40, 60));
			cell.setOutlineThickness(2);
			cell.setOutlineColor(Color(193, 128, 108));
			window.draw(cell);

			Sprite switchSprite;
			if (mass[i][j] == '|') {
				switchSprite.setTexture(texture_vkl);
			}
			else {
				switchSprite.setTexture(texture_vikl);
			}

			float scaleX = (razmer == 3 ? cellW : cellW5) / texture_vkl.getSize().x;
			float scaleY = (razmer == 3 ? cellW : cellW5) / texture_vkl.getSize().y;
			switchSprite.setScale(scaleX, scaleY);
			switchSprite.setPosition(startX + j * (razmer == 3 ? cellSize : cellSize5), startY + i * (razmer == 3 ? cellSize : cellSize5));
			window.draw(switchSprite);
		}
	}
}

int proverka(char mass[5][5], int razmer) {
	int proverim = 0;
	for (int i = 0; i < razmer; i++) {
		for (int j = 0; j < razmer; j++) {
			if (mass[i][j] == '|') {
				proverim++;
			}
		}
	}
	if (proverim == (razmer * razmer)) {
		return 1;
	}
	return 0;
}

void menaem(char mass[5][5], int stroka, int stolb, int razmer) {
	if (mass[stroka][stolb] == '-') mass[stroka][stolb] = '|';
	else if (mass[stroka][stolb] == '|') mass[stroka][stolb] = '-';

	if (stroka > 0) {
		if (mass[stroka - 1][stolb] == '-') mass[stroka - 1][stolb] = '|';
		else if (mass[stroka - 1][stolb] == '|') mass[stroka - 1][stolb] = '-';
	}

	if (stolb > 0) {
		if (mass[stroka][stolb - 1] == '-') mass[stroka][stolb - 1] = '|';
		else if (mass[stroka][stolb - 1] == '|') mass[stroka][stolb - 1] = '-';
	}

	if (stroka < razmer - 1) {
		if (mass[stroka + 1][stolb] == '-') mass[stroka + 1][stolb] = '|';
		else if (mass[stroka + 1][stolb] == '|') mass[stroka + 1][stolb] = '-';
	}

	if (stolb < razmer - 1) {
		if (mass[stroka][stolb + 1] == '-') mass[stroka][stolb + 1] = '|';
		else if (mass[stroka][stolb + 1] == '|') mass[stroka][stolb + 1] = '-';
	}
}

int prprpr(char mass[5][5], int chislo_perecluch) {
	int  razmer = 5;

	int n = razmer * razmer;

	int A[25][25] = { 0 };
	int b[25] = { 0 };

	for (int i = 0; i < razmer; i++) {
		for (int j = 0; j < razmer; j++) {
			int index = i * razmer + j;
			A[index][index] = 1;
			if (i > 0) A[index][(i - 1) * razmer + j] = 1;
			if (i < razmer - 1) A[index][(i + 1) * razmer + j] = 1;
			if (j > 0) A[index][i * razmer + (j - 1)] = 1;
			if (j < razmer - 1) A[index][i * razmer + (j + 1)] = 1;
		}
	}

	for (int i = 0; i < razmer; i++) {
		for (int j = 0; j < razmer; j++) {
			int index = i * razmer + j;
			if (mass[i][j] == '|') b[index] = 1;
			else b[index] = 0;
		}
	}

	for (int k = 0; k < n; k++) {
		int pivot = -1;
		for (int i = k; i < n; i++) {
			if (A[i][k] == 1) {
				pivot = i;
				break;
			}
		}
		if (pivot == -1) continue;
		if (pivot != k) {
			for (int j = 0; j < n; j++) {
				int temp = A[k][j];
				A[k][j] = A[pivot][j];
				A[pivot][j] = temp;
			}
			int temp_b = b[k];
			b[k] = b[pivot];
			b[pivot] = temp_b;
		}
		for (int i = k + 1; i < n; i++) {
			if (A[i][k] == 1) {
				for (int j = k; j < n; j++) {
					A[i][j] = A[i][j] ^ A[k][j];
				}
				b[i] = b[i] ^ b[k];
			}
		}
	}

	for (int i = 0; i < n; i++) {
		int all_zero = 1;
		for (int j = 0; j < n; j++) {
			if (A[i][j] != 0) {
				all_zero = 0;
				break;
			}
		}
		if (all_zero == 1 && b[i] == 1) {
			return 0;
		}
	}
	return 1;
}

int pole(int chislo_perecluch, char mass[5][5]) {
	int razmer;
	if (chislo_perecluch == 1) razmer = 3;
	else razmer = 5;

	int help = 0;
	int popitka = 0;

	while (help == 0) {
		popitka++;
		for (int i = 0; i < razmer; i++) {
			for (int j = 0; j < razmer; j++) {
				int perecluch = rand() % 2;
				if (perecluch == 1) mass[i][j] = '|';
				else mass[i][j] = '-';
			}
		}
		if (chislo_perecluch == 1) {
			help = 1;
		}
		else {
			help = prprpr(mass, chislo_perecluch);
		}
	}
	return 1;
}

void ChooseSize(RenderWindow& win, Font& font) {
	// Вместо создания нового окна, рисуем в переданном win
	RectangleShape background_choose(Vector2f(1280, 720));
	Texture texture_choose;
	if (!texture_choose.loadFromFile("image/menu.png")) exit(1);
	background_choose.setTexture(&texture_choose);

	Text title;
	title.setFont(font);
	InitText(title, 170, 20, L"Выберите размер поля", 90, Color(234, 181, 164), 3);

	String size_menu[] = { L"3 x 3", L"5 x 5", L"Назад" };
	game::GameMenu sizemenu(win, 640, 250, 3, size_menu, 80, 100);
	sizemenu.setColorTextMenu(Color(211, 146, 126), Color(234, 181, 164), Color(193, 128, 108));
	sizemenu.AlignMenu(2);

	bool choose_open = true;
	while (choose_open && win.isOpen()) {
		Event event_choose;
		while (win.pollEvent(event_choose)) {
			if (event_choose.type == Event::Closed) {
				win.close();
				choose_open = false;
			}

			if (event_choose.type == Event::KeyReleased) {
				if (event_choose.key.code == Keyboard::Up) { sizemenu.MoveUp(); }
				if (event_choose.key.code == Keyboard::Down) { sizemenu.MoveDown(); }
				if (event_choose.key.code == Keyboard::Return) {
					switch (sizemenu.getSelectedMenuNumber()) {
					case 0:
						razmer_polya = 3;
						vibor_razmera = 1;
						pole(vibor_razmera, mass);
						igra_zapushena = true;
						choose_open = false;
						break;
					case 1:
						razmer_polya = 5;
						vibor_razmera = 2;
						pole(vibor_razmera, mass);
						igra_zapushena = true;
						choose_open = false;
						break;
					case 2:
						choose_open = false;
						break;
					}
				}
			}

			if (event_choose.type == Event::MouseMoved) {
				Vector2i mousePos = Mouse::getPosition(win);
				sizemenu.CheckMouseHover(mousePos);
			}

			if (event_choose.type == Event::MouseButtonReleased) {
				if (event_choose.mouseButton.button == Mouse::Left) {
					Vector2i mousePos = Mouse::getPosition(win);
					int selected = sizemenu.getMouseSelected(mousePos);
					if (selected != -1) {
						switch (selected) {
						case 0:
							razmer_polya = 3;
							vibor_razmera = 1;
							pole(vibor_razmera, mass);
							igra_zapushena = true;
							choose_open = false;
							break;
						case 1:
							razmer_polya = 5;
							vibor_razmera = 2;
							pole(vibor_razmera, mass);
							igra_zapushena = true;
							choose_open = false;
							break;
						case 2:
							choose_open = false;
							break;
						}
					}
				}
			}
		}
		win.clear();
		win.draw(background_choose);
		win.draw(title);
		sizemenu.draw();
		win.display();
	}
}

void podskazka(char mass[5][5], int razmer) {
	int perem = 0, strok = 0, stolb = 0;

	for (int i = 0; i < razmer - 1; i++) {
		for (int j = 0; j < razmer; j++) {
			if (mass[i][j] == '-') {
				perem = 1;
				strok = i;
				stolb = j;
				break;
			}
		}
		if (perem == 1) break;
	}

	if (perem == 1) {
		wchar_t buf[200];
		swprintf_s(buf, L"Нажмите на переключатель:\nСтолбец: %d\nСтрока: %d", stolb + 1, strok + 2);
		MessageBoxW(NULL, buf, L"ПОДСКАЗКА", MB_OK);
		return;
	}

	if (razmer == 3) {
		wchar_t info[800] = L"";
		wchar_t temp[100];

		for (int j = 0; j < 3; j++) {
			if (mass[2][j] == '-') {
				swprintf_s(temp, L"", j + 1);
			}
			else {
				swprintf_s(temp, L"", j + 1);
			}
			wcscat_s(info, temp);
		}

		wcscat_s(info, L"\nКАКИЕ КЛЕТКИ НАЖАТЬ В 1-Й СТРОКЕ:\n");

		if (mass[2][0] == '-' && mass[2][1] == '-' && mass[2][2] == '-') {
			wcscat_s(info, L"Нажмите на 2-й переключатель\n");
		}
		else if (mass[2][0] == '-' && mass[2][1] == '-' && mass[2][2] == '|') {
			wcscat_s(info, L"Нажмите на 1-й и 2-й переключатели\n");
		}
		else if (mass[2][0] == '-' && mass[2][1] == '|' && mass[2][2] == '-') {
			wcscat_s(info, L"Нажмите на 1-й и 3-й переключатели\n");
		}
		else if (mass[2][0] == '-' && mass[2][1] == '|' && mass[2][2] == '|') {
			wcscat_s(info, L"Нажмите на 1-й переключатель\n");
		}
		else if (mass[2][0] == '|' && mass[2][1] == '-' && mass[2][2] == '-') {
			wcscat_s(info, L"Нажмите на 2-й и 3-й переключатели\n");
		}
		else if (mass[2][0] == '|' && mass[2][1] == '-' && mass[2][2] == '|') {
			wcscat_s(info, L"Нажмите на 3-й переключатель\n");
		}
		else if (mass[2][0] == '|' && mass[2][1] == '|' && mass[2][2] == '-') {
			wcscat_s(info, L"Нажмите на 1-й и 3-й переключатели\n");
		}
		else {
			wcscat_s(info, L"Попробуйте перебирать переключатели в 1-й строке\n");
		}

		MessageBoxW(NULL, info, L"ПОДСКАЗКА", MB_OK);
	}
	else if (razmer == 5) {
		wchar_t info[1500] = L"";
		wchar_t temp[100];

		for (int j = 0; j < 5; j++) {
			if (mass[4][j] == '-') {
				swprintf_s(temp, L"", j + 1);
			}
			else {
				swprintf_s(temp, L"", j + 1);
			}
			wcscat_s(info, temp);
		}

		wcscat_s(info, L"\nКАКИЕ КЛЕТКИ НАЖАТЬ В 1-Й СТРОКЕ:\n");

		if (mass[4][0] == '-' && mass[4][1] == '-' && mass[4][2] == '-' && mass[4][3] == '|' && mass[4][4] == '|') {
			wcscat_s(info, L"Нажмите на 2-й переключатель\n");
		}
		else if (mass[4][0] == '-' && mass[4][1] == '-' && mass[4][2] == '|' && mass[4][3] == '-' && mass[4][4] == '-') {
			wcscat_s(info, L"Нажмите на 3-й переключатель\n");
		}
		else if (mass[4][0] == '-' && mass[4][1] == '-' && mass[4][2] == '|' && mass[4][3] == '|' && mass[4][4] == '|') {
			wcscat_s(info, L"Нажмите на 1-й и 2-й переключатели\n");
		}
		else if (mass[4][0] == '-' && mass[4][1] == '|' && mass[4][2] == '-' && mass[4][3] == '-' && mass[4][4] == '|') {
			wcscat_s(info, L"Нажмите на 5-й переключатель\n");
		}
		else if (mass[4][0] == '-' && mass[4][1] == '|' && mass[4][2] == '-' && mass[4][3] == '|' && mass[4][4] == '-') {
			wcscat_s(info, L"Нажмите на 1-й и 4-й переключатели\n");
		}
		else if (mass[4][0] == '-' && mass[4][1] == '|' && mass[4][2] == '|' && mass[4][3] == '|' && mass[4][4] == '-') {
			wcscat_s(info, L"Нажмите на 1-й и 2-й переключатели\n");
		}
		else if (mass[4][0] == '|' && mass[4][1] == '-' && mass[4][2] == '-' && mass[4][3] == '|' && mass[4][4] == '-') {
			wcscat_s(info, L"Нажмите на 1-й переключатель\n");
		}
		else if (mass[4][0] == '|' && mass[4][1] == '-' && mass[4][2] == '-' && mass[4][3] == '|' && mass[4][4] == '|') {
			wcscat_s(info, L"Нажмите на 4-й переключатель\n");
		}
		else if (mass[4][0] == '|' && mass[4][1] == '-' && mass[4][2] == '|' && mass[4][3] == '-' && mass[4][4] == '|') {
			wcscat_s(info, L"Нажмите на 1-й и 4-й переключатели\n");
		}
		else if (mass[4][0] == '|' && mass[4][1] == '-' && mass[4][2] == '|' && mass[4][3] == '|' && mass[4][4] == '-') {
			wcscat_s(info, L"Нажмите на 3-й и 5-й переключатели\n");
		}
		else if (mass[4][0] == '|' && mass[4][1] == '|' && mass[4][2] == '-' && mass[4][3] == '-' && mass[4][4] == '-') {
			wcscat_s(info, L"Нажмите на 4-й переключатель\n");
		}
		else if (mass[4][0] == '|' && mass[4][1] == '|' && mass[4][2] == '-' && mass[4][3] == '-' && mass[4][4] == '|') {
			wcscat_s(info, L"Нажмите на 3-й и 5-й переключатели\n");
		}
		else if (mass[4][0] == '|' && mass[4][1] == '|' && mass[4][2] == '|' && mass[4][3] == '-' && mass[4][4] == '-') {
			wcscat_s(info, L"Нажмите на 1-й и 2-й переключатели\n");
		}
		else {
			wcscat_s(info, L"Попробуйте перебирать переключатели в 1-й строке\n");
		}

		MessageBoxW(NULL, info, L"ПОДСКАЗКА", MB_OK);
	}
}

void GamePlay(RenderWindow& win, Font& font) {
	float startX = 550;
	float startY = 100;

	RectangleShape backBtn(Vector2f(250, 70));
	backBtn.setPosition(50, 100);
	backBtn.setFillColor(Color(193, 128, 108));
	backBtn.setOutlineThickness(2);
	backBtn.setOutlineColor(Color(193, 128, 108));

	Text backText;
	backText.setFont(font);
	InitText(backText, 175, 115, L"Назад", 55, Color(234, 181, 164), 2);
	FloatRect backBounds = backText.getLocalBounds();
	backText.setPosition(50 + 125 - backBounds.width / 2, 100 + 35 - backBounds.height / 2);

	RectangleShape hintBtn(Vector2f(250, 70));
	hintBtn.setPosition(50, 190);
	hintBtn.setFillColor(Color(193, 128, 108));
	hintBtn.setOutlineThickness(2);
	hintBtn.setOutlineColor(Color(193, 128, 108));

	Text hintText;
	hintText.setFont(font);
	InitText(hintText, 175, 205, L"Подсказка", 55, Color(234, 181, 164), 2);
	FloatRect hintBounds = hintText.getLocalBounds();
	hintText.setPosition(50 + 125 - hintBounds.width / 2, 190 + 35 - hintBounds.height / 2);

	Text sizeText;
	sizeText.setFont(font);
	if (razmer_polya == 3) InitText(sizeText, 50, 290, L"Поле 3x3", 40, Color(200, 200, 200), 1);
	else InitText(sizeText, 50, 290, L"Поле 5x5", 40, Color(200, 200, 200), 1);

	while (igra_zapushena) {
		Event event;
		while (win.pollEvent(event)) {
			if (event.type == Event::Closed) {
				win.close();
				igra_zapushena = false;
			}

			if (event.type == Event::KeyPressed) {
				if (event.key.code == Keyboard::Escape) {
					igra_zapushena = false;
					return;
				}
			}

			if (event.type == Event::MouseButtonReleased) {
				if (event.mouseButton.button == Mouse::Left) {
					Vector2i mousePos = Mouse::getPosition(win);

					if (backBtn.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
						igra_zapushena = false;
						return;
					}

					if (hintBtn.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
						podskazka(mass, razmer_polya);
					}

					for (int i = 0; i < razmer_polya; i++) {
						for (int j = 0; j < razmer_polya; j++) {
							float cellW = (razmer_polya == 3) ? 120 : 85;
							float cellS = (razmer_polya == 3) ? 140 : 100;
							FloatRect cellRect(startX + j * cellS, startY + i * cellS, cellW, cellW);
							if (cellRect.contains(mousePos.x, mousePos.y)) {
								menaem(mass, i, j, razmer_polya);
								if (proverka(mass, razmer_polya) == 1) {
									pobeda = true;
									igra_zapushena = false;
								}
							}
						}
					}
				}
			}

			if (event.type == Event::MouseMoved) {
				Vector2i mousePos = Mouse::getPosition(win);
				if (backBtn.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
					backBtn.setFillColor(Color(213, 148, 128));
					backText.setFillColor(Color(255, 200, 180));
				}
				else {
					backBtn.setFillColor(Color(193, 128, 108));
					backText.setFillColor(Color(234, 181, 164));
				}

				if (hintBtn.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
					hintBtn.setFillColor(Color(213, 148, 128));
					hintText.setFillColor(Color(255, 200, 180));
				}
				else {
					hintBtn.setFillColor(Color(193, 128, 108));
					hintText.setFillColor(Color(234, 181, 164));
				}
			}
		}

		win.clear();

		RectangleShape background(Vector2f(1280, 720));
		Texture texture_win;
		if (texture_win.loadFromFile("image/menu.png")) {
			background.setTexture(&texture_win);
		}
		win.draw(background);

		win.draw(backBtn);
		win.draw(backText);
		win.draw(hintBtn);
		win.draw(hintText);
		win.draw(sizeText);

		vivodim(win, font, mass, razmer_polya);

		win.display();
	}
}

void WinScreen(RenderWindow& win, Font& font) {
	// Вместо создания нового окна, рисуем в переданном win
	RectangleShape background(Vector2f(1280, 720));
	Texture texture_win;
	if (!texture_win.loadFromFile("image/menu.png")) exit(1);
	background.setTexture(&texture_win);

	Text winText;
	winText.setFont(font);
	InitText(winText, 250, 260, L"ГОЛОВОЛОМКА РЕШЕНА!", 60, Color(234, 181, 164), 3);

	Text hintText;
	hintText.setFont(font);
	InitText(hintText, 400, 500, L"Нажмите любую клавишу", 40, Color(200, 200, 200), 2);

	bool win_open = true;
	while (win_open && win.isOpen()) {
		Event event;
		while (win.pollEvent(event)) {
			if (event.type == Event::Closed) {
				win.close();
				win_open = false;
				pobeda = false;
			}
			if (event.type == Event::KeyPressed) {
				win_open = false;
				pobeda = false;
			}
			if (event.type == Event::MouseButtonReleased) {
				win_open = false;
				pobeda = false;
			}
		}
		win.clear();
		win.draw(background);
		win.draw(winText);
		win.draw(hintText);
		win.display();
	}
}

void About_Game(RenderWindow& win, Font& font) {
	// Вместо создания нового окна, рисуем в переданном win
	RectangleShape background_ab(Vector2f(1280, 720));
	Texture texture_ab;
	if (!texture_ab.loadFromFile("image/menu.png")) exit(3);
	background_ab.setTexture(&texture_ab);

	Text titleText;
	titleText.setFont(font);
	InitText(titleText, 400, 20, L"ОБ ИГРЕ", 100, Color(234, 181, 164), 3);

	Text descText;
	descText.setFont(font);
	descText.setCharacterSize(35);
	descText.setFillColor(Color(234, 181, 164));
	descText.setString(L"Связные переключатели - головоломка\n\n"
		L"Правила:\n"
		L"При нажатии на переключатель,\n"
		L"он и все соседние в строке и столбце\n"
		L"меняют своё состояние.\n"
		L"Цель: все переключатели во включенное состояние\n\n"
		L"Управление:\n"
		L"- Клик мышью по переключателю\n"
		L"- Кнопка 'Назад' для возврата\n"
		L"- Кнопка 'Подсказка' для помощи");
	descText.setPosition(240, 150);
	descText.setOutlineThickness(2);
	descText.setOutlineColor(Color(193, 128, 108));

	Text hintText;
	hintText.setFont(font);
	InitText(hintText, 450, 650, L"Нажмите ESC для возврата", 40, Color(200, 200, 200), 2);

	bool about_open = true;
	while (about_open && win.isOpen()) {
		Event event_play;
		while (win.pollEvent(event_play)) {
			if (event_play.type == Event::Closed) {
				win.close();
				about_open = false;
			}
			if (event_play.type == Event::KeyPressed) {
				if (event_play.key.code == Keyboard::Escape) about_open = false;
			}
		}
		win.clear();
		win.draw(background_ab);
		win.draw(titleText);
		win.draw(descText);
		win.draw(hintText);
		win.display();
	}
}

int main() {
	srand(time(NULL));

	if (!texture_vkl.loadFromFile("image/vkl.png")) {
		return 6;
	}
	if (!texture_vikl.loadFromFile("image/vikl.png")) {
		return 7;
	}

	RenderWindow win(VideoMode(1280, 720), L"Связные переключатели");

	float width = VideoMode::getDesktopMode().width;
	float height = VideoMode::getDesktopMode().height;

	RectangleShape background(Vector2f(width, height));
	Texture texture_win;
	if (!texture_win.loadFromFile("image/menu.png")) return 4;
	background.setTexture(&texture_win);

	Font font;
	if (!font.loadFromFile("arial.ttf")) {
		return 5;
	}

	Text Titul;
	Titul.setFont(font);
	InitText(Titul, 70, 15, L"Связные переключатели", 100, Color(234, 181, 164), 3);

	String name_menu[] = { L"Старт", L"Об игре", L"Выход" };
	game::GameMenu mymenu(win, 650, 200, 3, name_menu, 80, 120);
	mymenu.setColorTextMenu(Color(211, 146, 126), Color(211, 146, 126), Color(193, 128, 108));
	mymenu.AlignMenu(2);

	while (win.isOpen()) {
		Event event;
		while (win.pollEvent(event)) {
			if (event.type == Event::Closed)
				win.close();

			if (event.type == Event::KeyReleased) {
				if (event.key.code == Keyboard::Up) { mymenu.MoveUp(); }
				if (event.key.code == Keyboard::Down) { mymenu.MoveDown(); }
				if (event.key.code == Keyboard::Return) {
					switch (mymenu.getSelectedMenuNumber()) {
					case 0:
						ChooseSize(win, font);
						if (igra_zapushena) {
							GamePlay(win, font);
							if (pobeda) {
								WinScreen(win, font);
							}
							igra_zapushena = false;
							pobeda = false;
						}
						break;
					case 1: About_Game(win, font); break;
					case 2: win.close(); break;
					}
				}
			}

			if (event.type == Event::MouseMoved) {
				Vector2i mousePos = Mouse::getPosition(win);
				mymenu.CheckMouseHover(mousePos);
			}

			if (event.type == Event::MouseButtonReleased) {
				if (event.mouseButton.button == Mouse::Left) {
					Vector2i mousePos = Mouse::getPosition(win);
					int selected = mymenu.getMouseSelected(mousePos);
					if (selected != -1) {
						switch (selected) {
						case 0:
							ChooseSize(win, font);
							if (igra_zapushena) {
								GamePlay(win, font);
								if (pobeda) {
									WinScreen(win, font);
								}
								igra_zapushena = false;
								pobeda = false;
							}
							break;
						case 1: About_Game(win, font); break;
						case 2: win.close(); break;
						}
					}
				}
			}
		}
		win.clear();
		win.draw(background);
		win.draw(Titul);
		mymenu.draw();
		win.display();
	}
	return 0;
}