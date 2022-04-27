#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <string>

const int rowSize = 9;
const float fieldSize = 50;
const std::string fontName = "Iosevka Nerd Font Complete Mono.ttf";

class Cell {

private:
    sf::RectangleShape field;
    sf::Font font;
    sf::Text textField;
    int value;

public:
    int getValue() {
        return value;
    }

    void setValue(int newValue) {
        value = newValue;
    }

    void setPosition(sf::Vector2f origin) {
        field.setPosition(origin);
    }

    void setTextPosition(sf::Vector2f origin) {
        textField.setPosition(origin);
    }

    sf::RectangleShape getField() {
        return field;
    }

    sf::Text getTextField() {
        std::cout << "Managed to get here." << std::endl;
        return textField;
    }

    Cell(int newValue, sf::Font& font) {
        field = sf::RectangleShape(sf::Vector2f(fieldSize, fieldSize));
        field.setFillColor(sf::Color::White);
        field.setOutlineColor(sf::Color::Black);
        field.setOutlineThickness(2.f);

        std::cout << "Font loaded" << std::endl;
        value = newValue;
        textField.setFont(font);
        textField.setString(std::to_string(value));
        textField.setCharacterSize(24);
        textField.setFillColor(sf::Color::Black);
    }
};

class Grid {

private:
    std::vector<std::vector<Cell>> playingField;  

public:
    Grid(sf::Vector2f origin, sf::Font& font) {
        for (int i = 0; i < rowSize; i++) {
            std::vector<Cell> row;
            playingField.push_back(row);

            for (int j = 0; j < rowSize; j++) {
                playingField.at(i).push_back(Cell(j + 1, font));
                playingField.at(i).at(j).setPosition(sf::Vector2f(origin.x + (fieldSize * j), origin.y));
                playingField.at(i).at(j).setTextPosition(sf::Vector2f(origin.x + (fieldSize * j), origin.y));
            }
            origin = sf::Vector2f(origin.x, (origin.y + fieldSize));
        }
    }

    void drawGrid(sf::RenderWindow& window) {
        for (int m = 0; m < rowSize; m++) {
            for (int n = 0; n < rowSize; n++) {
                window.draw(playingField.at(m).at(n).getField());
                window.draw(playingField.at(m).at(n).getTextField());
            }
        }
    }
};

int main()
{
    sf::RenderWindow window(sf::VideoMode(800,600), "My window");
    sf::Font font;
    if (!font.loadFromFile(fontName)) {
        std::cout << "Error loading font!" << std::endl;
    }
    window.setVerticalSyncEnabled(true);

    Grid playingField = Grid(sf::Vector2f(10.f,10.f), font);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {

            switch (event.type) {
                case sf::Event::Closed: {
                    window.close();
                }
                case sf::Event::TextEntered: {
                    if (event.text.unicode < 128)
                        std::cout << "Character typed: " << static_cast<char>(event.text.unicode) << std::endl;
                }
            }
        }
        window.clear(sf::Color::White);
        playingField.drawGrid(window);
        window.display();
    }
    return 0;
}