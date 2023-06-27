#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>

struct Textures {
    sf::Texture playerHead;
    sf::Texture playerBody;
    sf::Texture wall;
    sf::Texture ground;
};

struct Sprites {
    sf::Sprite playerHead;
    sf::Sprite playerBody;
    sf::Sprite wall;
    sf::Sprite ground;
};

struct Cordinates {
    int x;
    int y;
};

struct tileProperties {
    bool isWall;
    bool hasSnakeBodyPart;
    int enemyWithIndex;
    int foodWithIndex;
};

enum class Orientation { Up, Down, Left, Right };

class Map {

public:
    Map(bool gameMap[10][10]) {
        for (int i = 0; i < 10; i++) {
            for (int j = 0; j < 10; j++) {
                this->gameMap[i][j].isWall = gameMap[i][j];
                this->gameMap[i][j].enemyWithIndex = -1;
                this->gameMap[i][j].foodWithIndex = -1;
                this->gameMap[i][j].hasSnakeBodyPart = false;
            }
        }
    }
    void updateGameMap(tileProperties gameMap[10][10]);
    tileProperties getMapPropertiesAtPostion(Cordinates cords);
    void drawWorldMap(Sprites& gameSprites, sf::RenderWindow& window);


private:
    tileProperties gameMap[10][10];

};

class Player {

public:
    Player() {
        this->position = { 5,5 };
        this->orientation = Orientation::Down;
        this->frameNumber = 0;
        this->msSinceLastMove = 0;
        this->msSinceLastFrameChange = 0;
        this->lastUpdate = 0;
        this->alive = true;
        this->firstKeyPressed = false;
    }

    void draw(Sprites& gameSprites, sf::RenderWindow& window);

    void update(sf::Time& gameTime, Map& gameMap);

    bool isAlive() {
        return this->alive;
    }

    void setOrientation(Orientation ort) {
        if (this->alive) {
            this->orientation = ort;
            this->firstKeyPressed = true;
        }
    }

private:
    Cordinates position;
    Orientation orientation;
    int msSinceLastMove;
    int msSinceLastFrameChange;
    int frameNumber;
    int lastUpdate;
    bool alive;
    bool firstKeyPressed;
};


class Bodypart {

public:
    Bodypart(Cordinates cords, Orientation orientation, int timeInMs, int frameNumber) {
        this->cords = cords;
        this->orientation = orientation;
        this->msPassedWhenSpawned = timeInMs;
        this->canBeDeleted = false;
        this->frameNumber = frameNumber;
    }

    void draw();

    void update();

    bool getCanBeDeleted();

private:
    Cordinates cords;
    Orientation orientation;
    int msPassedWhenSpawned;
    int frameNumber;
    bool canBeDeleted;
};



