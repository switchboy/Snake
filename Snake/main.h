#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <array>
#include <vector>

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
    bool hasEnemy;
    bool hasSnakeBodyPart;
    bool hasFood;
};

enum class Orientation { Up, Down, Left, Right };
enum class MapObjectTypes { Food, Enemy, BodyPart};

class Map {

public:
    Map(bool gameMap[10][10]);
    void updateGameMap(tileProperties gameMap[10][10]);
    tileProperties getMapPropertiesAtPostion(Cordinates cords);
    void drawWorldMap(Sprites& gameSprites, sf::RenderWindow& window);
    void addObject(Cordinates cords, MapObjectTypes object);
    void removeObject(Cordinates cords, MapObjectTypes object);

private:
    tileProperties gameMap[10][10];

};

class Bodypart {

public:
    Bodypart(Cordinates cords, Orientation orientation, int timeInMs, int frameNumber, int maxLifetimeInMs);
    bool getCanBeDeleted();
    void draw(sf::RenderWindow& window, Sprites& gameSprites);
    void update(sf::Time& gameTime, Map& gameMap);

private:
    Cordinates cords;
    Orientation orientation;
    int msPassedWhenSpawned;
    int frameNumber;
    int maxLifetimeInMs;
    int lifetimeInMs;
    int msSinceLastFrameChange;
};

class Player {

public:
    Player();
    bool isAlive();
    void setOrientation(Orientation ort);
    void draw(Sprites& gameSprites, sf::RenderWindow& window);
    void update(sf::Time& gameTime, Map& gameMap, std::vector<Bodypart>& bodyParts);

private:
    Cordinates position;
    Orientation orientation;
    int msSinceLastMove;
    int msSinceLastFrameChange;
    int frameNumber;
    int lastUpdate;
    int applesEaten;
    bool alive;
    bool firstKeyPressed;

};






