#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>

class Player {

public:
    Player() {
        this->position = { 1,1 };
        this->orientation = Orientation::Down;
        this->frameNumber = 0;
        this->msSinceLastMove = 0;
        int msSinceLastFrameChange; = 0; 
    }

    void draw(Sprites& gameSprites, sf::RenderWindow& window);

    void update(sf::Time& gameTime);

    void setOrientation(Orientation ort) {
        this->orientation = ort;
    }

private:
    Cordinates position;
    Orientation orientation;
    int msSinceLastMove;
    int msSinceLastFrameChange;
    int frameNumber;
};

