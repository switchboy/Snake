#include "main.h"



void loadAndApplyTextures(Textures &gameTextures, Sprites &gameSprites) {
    if (!gameTextures.playerHead.loadFromFile("textures/snakehead.png")) {
        std::cout << "Error loading texture for playerHead! \n";
    }
    if (!gameTextures.playerBody.loadFromFile("textures/snakeBodyPart.png")) {
        std::cout << "Error loading texture for playerBody! \n";
    }
    if (!gameTextures.wall.loadFromFile("textures/wall.png")) {
        std::cout << "Error loading texture for wall! \n";
    }
    if (!gameTextures.ground.loadFromFile("textures/ground.png")) {
        std::cout << "Error loading texture for ground! \n";
    }

    gameSprites.playerHead.setTexture(gameTextures.playerHead);
    gameSprites.playerBody.setTexture(gameTextures.playerBody);
    gameSprites.wall.setTexture(gameTextures.wall);
    gameSprites.ground.setTexture(gameTextures.ground);
}

Map::Map(bool gameMap[10][10]) {
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            this->gameMap[i][j].isWall = gameMap[i][j];
            this->gameMap[i][j].hasEnemy = false;
            this->gameMap[i][j].hasFood = false;
            this->gameMap[i][j].hasSnakeBodyPart = false;
        }
    }
}


void Map::updateGameMap(tileProperties gameMap[10][10]) {
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            this->gameMap[i][j] = gameMap[i][j];
        }
    }
}

tileProperties Map::getMapPropertiesAtPostion(Cordinates cords) {
    return this->gameMap[cords.x][cords.y];
}

void Map::drawWorldMap(Sprites& gameSprites, sf::RenderWindow& window) {
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            if (this->gameMap[i][j].isWall) {
                gameSprites.wall.setPosition(sf::Vector2f(float(i * 64),float(j * 64)));
                window.draw(gameSprites.wall);
            }
            else {
                gameSprites.ground.setPosition(sf::Vector2f(float(i * 64), float(j * 64)));
                window.draw(gameSprites.ground);
            }
        }
    }
}

void Map::addObject(Cordinates cords, MapObjectTypes object) {
    switch (object){
        
    case MapObjectTypes::Food:
        this->gameMap[cords.x][cords.y].hasFood = true;
        break;
    case MapObjectTypes::Enemy:
        this->gameMap[cords.x][cords.y].hasEnemy = true;
        break;
    case MapObjectTypes::BodyPart:
        this->gameMap[cords.x][cords.y].hasSnakeBodyPart = true;
        break;
    }
}
void Map::removeObject(Cordinates cords, MapObjectTypes object) {
    switch (object) {

    case MapObjectTypes::Food:
        this->gameMap[cords.x][cords.y].hasFood = false;
        break;
    case MapObjectTypes::Enemy:
        this->gameMap[cords.x][cords.y].hasEnemy = false;
        break;
    case MapObjectTypes::BodyPart:
        this->gameMap[cords.x][cords.y].hasSnakeBodyPart = false;
        break;
    }
}

Player::Player() {
    this->position = { 5,5 };
    this->orientation = Orientation::Down;
    this->frameNumber = 0;
    this->msSinceLastMove = 0;
    this->msSinceLastFrameChange = 0;
    this->lastUpdate = 0;
    this->alive = true;
    this->firstKeyPressed = false;
    this->applesEaten = 0;
}

bool Player::isAlive() {
    return this->alive;
}

void Player::setOrientation(Orientation ort) {
    if (this->alive) {
        this->orientation = ort;
        this->firstKeyPressed = true;
    }
}

void Player::draw(Sprites& gameSprites, sf::RenderWindow& window) {
    gameSprites.playerHead.setTextureRect(sf::IntRect(this->frameNumber * 64, 0, 64, 64));
    gameSprites.playerHead.setOrigin(32.f, 32.f);
    gameSprites.playerBody.setTextureRect(sf::IntRect(this->frameNumber * 64, 0, 64, 64));
    gameSprites.playerBody.setOrigin(32.f, 32.f);
    Cordinates tempCords = this->position;
    switch (this->orientation) {
    case Orientation::Left :
        gameSprites.playerHead.setRotation(180.f);
        gameSprites.playerBody.setRotation(180.f);
        tempCords.x += -1;
        break;
    case Orientation::Down:
        gameSprites.playerHead.setRotation(90.f);
        gameSprites.playerBody.setRotation(90.f);
        tempCords.y += 1;
        break;
    case Orientation::Right:
        gameSprites.playerHead.setRotation(0.f);
        gameSprites.playerBody.setRotation(0.f);
        tempCords.x += 1;
        break;
    case Orientation::Up:
        gameSprites.playerHead.setRotation(270.f);
        gameSprites.playerBody.setRotation(270.f);
        tempCords.y += -1;
        break;
    }  

    gameSprites.playerBody.setPosition(sf::Vector2f(float(this->position.x * 64 + 32), float(this->position.y * 64 + 32)));
    window.draw(gameSprites.playerBody);

    float offSetInPixels = (float(this->lastUpdate - this->msSinceLastMove) / 500.f) * 64.f;

    if (tempCords.x == this->position.x) {
        if (tempCords.y - this->position.y > 0) {
            gameSprites.playerHead.setPosition(sf::Vector2f(float(this->position.x * 64 + 32), float(this->position.y * 64 + 32) + offSetInPixels));
            window.draw(gameSprites.playerHead);
            return;
        }
        gameSprites.playerHead.setPosition(sf::Vector2f(float(this->position.x * 64 + 32), float(this->position.y * 64 + 32) - offSetInPixels));
        window.draw(gameSprites.playerHead);
        return;       
    }
    if (tempCords.x - this->position.x > 0) {
        gameSprites.playerHead.setPosition(sf::Vector2f(float(this->position.x * 64 + 32) + offSetInPixels, float(this->position.y * 64 + 32)));
        window.draw(gameSprites.playerHead);
        return;
    }
    gameSprites.playerHead.setPosition(sf::Vector2f(float(this->position.x * 64 + 32) - offSetInPixels, float(this->position.y * 64 + 32)));
    window.draw(gameSprites.playerHead);
    return;
}

void Player::update(sf::Time& gameTime, Map& gameMap, std::vector<Bodypart>& bodyParts) {
    if (!this->firstKeyPressed) {
        return;
    }
    this->lastUpdate = gameTime.asMilliseconds();
    if (!this->alive) {
        this->msSinceLastFrameChange = gameTime.asMilliseconds();
        this->msSinceLastMove = gameTime.asMilliseconds();
        return;
    }
    if (this->msSinceLastFrameChange < gameTime.asMilliseconds() - 250) {
        this->msSinceLastFrameChange = gameTime.asMilliseconds();
        this->frameNumber++;
        if (this->frameNumber >= 4) {
            this->frameNumber = 0;
        }
    }
    if (this->msSinceLastMove < gameTime.asMilliseconds() - 500) {
        this->msSinceLastMove = gameTime.asMilliseconds();
        Cordinates tempCords = this->position;
        switch (this->orientation) {
        case Orientation::Left:
            tempCords.x += -1;
            break;
        case Orientation::Down:
            tempCords.y += 1;
            break;
        case Orientation::Right:
            tempCords.x += 1;
            break;
        case Orientation::Up:
            tempCords.y += -1;
            break;
        }

        if (!gameMap.getMapPropertiesAtPostion(tempCords).isWall && !gameMap.getMapPropertiesAtPostion(tempCords).hasSnakeBodyPart  && !gameMap.getMapPropertiesAtPostion(tempCords).hasEnemy){
            int tempFrameNumber = 0;
            if (bodyParts.size() % 2 == 0) {
                tempFrameNumber = 2;
            }
            bodyParts.push_back(Bodypart(this->position, this->orientation, this->lastUpdate, tempFrameNumber , 500*this->applesEaten));
            gameMap.addObject(this->position, MapObjectTypes::BodyPart);
            this->position = tempCords;
            if(gameMap.getMapPropertiesAtPostion(tempCords).hasFood){
                gameMap.removeObject(this->position, MapObjectTypes::Food);
                this->applesEaten++;
            }
        }
        else {
            this->alive = false;
            this->frameNumber = 4;
        }
    }
}

Bodypart::Bodypart(Cordinates cords, Orientation orientation, int timeInMs, int frameNumber, int maxLifetimeInMs) {
    this->cords = cords;
    this->orientation = orientation;
    this->msPassedWhenSpawned = timeInMs;
    this->maxLifetimeInMs = timeInMs + maxLifetimeInMs;
    this->frameNumber = frameNumber;
    this->msSinceLastFrameChange = timeInMs;
    this->lifetimeInMs = timeInMs;
}

bool Bodypart::getCanBeDeleted() {
    if (this->maxLifetimeInMs < this->lifetimeInMs) {
        return true;
    }
    return false;
}

void Bodypart::draw(sf::RenderWindow& window, Sprites& gameSprites) {
    gameSprites.playerBody.setTextureRect(sf::IntRect(this->frameNumber * 64, 0, 64, 64));
    gameSprites.playerBody.setOrigin(32.f, 32.f);
    switch (this->orientation) {
    case Orientation::Left:
        gameSprites.playerBody.setRotation(180.f);
        break;
    case Orientation::Down:
        gameSprites.playerBody.setRotation(90.f);
        break;
    case Orientation::Right:
        gameSprites.playerBody.setRotation(0.f);
        break;
    case Orientation::Up:
        gameSprites.playerBody.setRotation(270.f);
        break;
    }
    gameSprites.playerBody.setPosition(sf::Vector2f(float(this->cords.x * 64 + 32), float(this->cords.y * 64 + 32)));
    window.draw(gameSprites.playerBody);
    return;
}

void Bodypart::update(sf::Time& gameTime, Map& gameMap){
    this->lifetimeInMs = gameTime.asMilliseconds();
    if (this->msSinceLastFrameChange < gameTime.asMilliseconds() - 250) {
        this->msSinceLastFrameChange = gameTime.asMilliseconds();
        this->frameNumber++;
        if (this->frameNumber >= 4) {
            this->frameNumber = 0;
        }
    }
    if (this->maxLifetimeInMs < this->lifetimeInMs) {
        gameMap.removeObject(this->cords, MapObjectTypes::BodyPart);
    }
}

void drawBodyparts(std::vector<Bodypart>& bodyParts, sf::RenderWindow& window, Sprites& gameSprites) {
    for (Bodypart& bodyPart : bodyParts)
    {
        bodyPart.draw(window, gameSprites);
    }
}

void updateBodyparts(std::vector<Bodypart>& bodyParts, sf::Time& gameTime, Map& gameMap) {
    for (std::vector<Bodypart>::iterator it = bodyParts.begin(); it != bodyParts.end();) {
        it->update(gameTime, gameMap);
        if (it->getCanBeDeleted()) {
            it = bodyParts.erase(it);
        }
        else
            ++it;
    }
}

void reloadGame(Player& player, sf::Clock& gameClock, Map& gameMap, bool defaultGameMap[10][10], std::vector<Bodypart>& bodyParts) {
    new (&player) Player();
    new (&gameClock) sf::Clock;
    new (&gameMap) Map(defaultGameMap);
    bodyParts.clear();
}

int main()
{
    sf::RenderWindow window(sf::VideoMode(1024, 640), "Snake");
    Textures gameTextures;
    Sprites gameSprites;
    Player player;
    std::vector<Bodypart> bodyParts;
    loadAndApplyTextures(gameTextures, gameSprites);
    sf::Clock gameClock;
    bool defaultGameMap[10][10] = {
        {true,  true, true, true, true, true, true, true, true, true },
        {true,  false, false, false, false, false, false, false, false, true },
        {true,  false, false, false, false, false, false, false, false, true },
        {true,  false, false, false, false, false, false, false, false, true },
        {true,  false, false, false, false, false, false, false, false, true },
        {true,  false, false, false, false, false, false, false, false, true },
        {true,  false, false, false, false, false, false, false, false, true },
        {true,  false, false, false, false, false, false, false, false, true },
        {true,  false, false, false, false, false, false, false, false, true },
        {true,  true, true, true, true, true, true, true, true, true },
    };
    Map gameMap(defaultGameMap);
    bool windowHasFocus = true;

    while (window.isOpen())
    {
        sf::Time gameTime = gameClock.getElapsedTime();
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Key::Up) {
                    player.setOrientation(Orientation::Up);
                }
                else if (event.key.code == sf::Keyboard::Key::Down) {
                    player.setOrientation(Orientation::Down);
                }
                else if (event.key.code == sf::Keyboard::Key::Left) {
                    player.setOrientation(Orientation::Left);
                }
                else if (event.key.code == sf::Keyboard::Key::Right) {
                    player.setOrientation(Orientation::Right);
                }
                else if (!player.isAlive() && event.key.code == sf::Keyboard::Key::Enter) {
                    reloadGame(player, gameClock, gameMap, defaultGameMap, bodyParts);
                    gameTime = gameClock.getElapsedTime();
                }
            }
            else if (event.type == sf::Event::GainedFocus) {
                windowHasFocus = true;
            }
            else if (event.type == sf::Event::LostFocus) {
                windowHasFocus = false;
            }
            else if (event.type == sf::Event::Closed)
                window.close();
        }
        if (windowHasFocus) {
            updateBodyparts(bodyParts, gameTime, gameMap);
            player.update(gameTime, gameMap, bodyParts);
        }
        window.clear();
        gameMap.drawWorldMap(gameSprites, window);
        drawBodyparts(bodyParts, window, gameSprites);
        player.draw(gameSprites, window);
        window.display();
    }

    return 0;
}


