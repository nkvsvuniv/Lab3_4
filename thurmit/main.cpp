
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

#include <iostream>
#include <fstream>

const int windowSize = 1200;
const int squareSize = 8;
const int quantity = windowSize / squareSize;

struct threeD{
    int color;
    int rotate;
    int state;
    void set(int c, int r, int s){
        color = c;
        rotate = r;
        state = s;
    }
    bool operator==(threeD & object){
        if (color == object.color && state == object.state) return true;
        return false;
    }
};

struct thurmit{
    sf::Vector2i pos;
    signed int direct;
    int state;
    thurmit(sf::Vector2i p = sf::Vector2i(windowSize / 2, windowSize / 2), int d = 1, int s = 1) : pos(p), direct(d), state(s){}
    void move(sf::RectangleShape square[quantity][quantity], threeD ** table, int colors, int states, sf::Color * palitra){
        int colorNum;
        sf::Color color = square[pos.x / squareSize][pos.y / squareSize].getFillColor();
        for (int i = 0; i < 6; i++) {
            if (color == palitra[i]) colorNum = i;
        }
        square[pos.x / squareSize][pos.y / squareSize].setFillColor(palitra[table[state-1][colorNum].color - 1]);
        direct = (4 + direct + table[state-1][colorNum].rotate) % 4;
        state = table[state-1][colorNum].state;
        direct % 2 == 0 ? pos.x += (direct - 1) * squareSize : pos.y += (direct - 2) * squareSize;
    }
    bool end(){
        if (pos.x < 0 || pos.x >= windowSize || pos.y < 0 || pos.y >= windowSize) return true;
        return false;
    }
};

int main()
{
    sf::RenderWindow window(sf::VideoMode(windowSize, windowSize), "SFML window");
    window.setPosition(sf::Vector2i(300, 300));
    
    sf::Color palitra[6] = {sf::Color::White, sf::Color::Black, sf::Color::Red, sf::Color::Green, sf::Color::Blue, sf::Color::Yellow};
    
    std::ifstream fin("File");
    int states, colors;
    fin >> states >> colors;
    
    sf::Color * palette = new sf::Color[colors];
    for (int i = 0; i < colors; i++) {
        int temp;
        fin >> temp;
        palette[i] = palitra[temp-1];
    }
    
    sf::RectangleShape w[quantity][quantity];
    
    for (int i = 0; i < quantity; i++) {
        for (int j = 0; j < quantity; j++) {
            w[i][j].setSize(sf::Vector2f(squareSize, squareSize));
            w[i][j].setPosition(sf::Vector2f(i * squareSize, j * squareSize));
            w[i][j].setFillColor(palette[0]);
        }
    }
    
    thurmit runner;
    
    threeD ** table = new threeD*[states];
    for (int i = 0; i < colors; i++) {
        table[i] = new threeD[colors];
        for (int j = 0; j < colors; j++) {
            int temp1, temp2, temp3;
            fin >> temp1 >> temp2 >> temp3;
            table[i][j].set(temp1, temp2, temp3);
        }
    }
    
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
                window.close();
            }
        }
        if (runner.end()) {
            window.close();
        }
        
        runner.move(w, table, colors, states, palette);

        window.clear(sf::Color::Black);

        for (int i = 0; i < quantity; i++) {
            for (int j = 0; j < quantity; j++) {
                window.draw(w[i][j]);
            }
        }
        
        window.display();
    }
    
    for (int i = 0; i < colors; i++) {
        delete [] table[i];
    }
    delete [] table;

    return EXIT_SUCCESS;
}
