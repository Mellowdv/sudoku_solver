#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <string>

const int rowSize = 9;
const float fieldSize = 50;
const std::string fontName = "Iosevka Nerd Font Complete Mono.ttf";
const int fontSize = 32;
const float originPoint = 10.0f;

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
        textField.setString(std::to_string(value));
    }

    void setEmpty() {
        textField.setString("");
    }

    void setPosition(sf::Vector2f origin) {
        field.setPosition(origin);
    }

    void setTextPosition(sf::Vector2f origin) {
        textField.setPosition(origin);
    }

    void setFieldFillColor(sf::Color color) {
        field.setFillColor(color);
    }

    sf::RectangleShape getField() {
        return field;
    }

    sf::Text getTextField() {
        return textField;
    }

    Cell(sf::Font& font) {
        field = sf::RectangleShape(sf::Vector2f(fieldSize, fieldSize));
        field.setFillColor(sf::Color::White);
        field.setOutlineColor(sf::Color::Black);
        field.setOutlineThickness(2.f);

        //std::cout << "Font loaded" << std::endl;
        textField.setFont(font);
        textField.setString("");
        textField.setCharacterSize(fontSize);
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
                playingField.at(i).push_back(Cell(font));
                playingField.at(i).at(j).setPosition(sf::Vector2f(origin.x + (fieldSize * j), origin.y));
                playingField.at(i).at(j).setTextPosition(sf::Vector2f(origin.x + (fieldSize * j) + 16, origin.y + 5));
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

    void accessCell(std::vector<int> &cellAddress, sf::Color color) { 
        playingField.at(cellAddress.at(0)).at(cellAddress.at(1)).setFieldFillColor(color);
    }

    void eraseCell(std::vector<int> &cellAddress) {
        playingField.at(cellAddress.at(0)).at(cellAddress.at(1)).setEmpty();
    }

    void setCellValue(std::vector<int> &cellAddress, int value) {
        playingField.at(cellAddress.at(0)).at(cellAddress.at(1)).setValue(value);
    }
};

std::vector<int> determineCell(int& x, int& y) {
    
    std::vector<int> cellCoordinates;
    cellCoordinates.push_back(x);
    cellCoordinates.push_back(y);

    // determine column
    for (int i = 0; i < rowSize; i++) {
        if (x > (fieldSize * i) + originPoint)
            cellCoordinates.at(1) = i;
    }

    // determine row
    for (int j = 0; j < rowSize; j++) {
        if (y > (fieldSize * j) + originPoint)
            cellCoordinates.at(0) = j;
    }
    return cellCoordinates;
}

int main()
{
    sf::RenderWindow window(sf::VideoMode(800,600), "My window");
    sf::Font font;
    if (!font.loadFromFile(fontName)) {
        std::cout << "Error loading font!" << std::endl;
    }
    window.setVerticalSyncEnabled(true);

    Grid playingField = Grid(sf::Vector2f(10.f,10.f), font);
    std::vector<int> activeCell;
    activeCell.clear();


    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {

            switch (event.type) {
                case sf::Event::Closed: {
                    window.close();
                }
                case sf::Event::MouseButtonPressed: {
                    if (event.mouseButton.button == sf::Mouse::Left) {
                        int x = event.mouseButton.x;
                        int y = event.mouseButton.y;
                        int fieldBorder = (fieldSize * rowSize) + originPoint;
                        std::cout << "Mouse position: " << x << ", " << y << "." << std::endl;
                        if ((x > fieldBorder || y > fieldBorder) || (x < originPoint || y < originPoint))
                            std::cout << "Out of bounds";
                        else if (activeCell.size() == 0) {
                            activeCell = determineCell(x, y);
                            playingField.accessCell(activeCell, sf::Color::Yellow);
                        }
                        else {
                                playingField.accessCell(activeCell, sf::Color::White);
                                activeCell = determineCell(x, y);
                                playingField.accessCell(activeCell, sf::Color::Yellow);
                        }
                        std::cout << "Active cell is: [" << activeCell.at(0) << "][" << activeCell.at(1) << "]" << std::endl;
                    }
                }
                case sf::Event::KeyPressed: {
                    if (activeCell.size() == 0)
                        std::cout << "No active cell." << std::endl;
                    else {
                        switch (event.key.code) {
                            case sf::Keyboard::Backspace: {
                                playingField.eraseCell(activeCell);
                                break;
                            }
                            default: {
                                if (event.key.code >= 27 && event.key.code <= 35) {
                                    playingField.setCellValue(activeCell, event.key.code - 26);
                                    break;
                                }
                                else
                                    break;
                            }
                        }
                    }
                }
            }
        }
        window.clear(sf::Color::White);
        playingField.drawGrid(window);
        window.display();
        }
    return 0;
}