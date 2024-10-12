#include <SFML/Graphics.hpp>
#include <cmath>
#include "avl.h"
using namespace sf;
using namespace std;

int main() {
    srand(sin(time(0)) * 1000); 
    int frameCount = 0, moveTimer = 1000;
	RenderWindow window(VideoMode(600, 600), "CoinWars");
    window.setFramerateLimit(moveTimer);

    Inventory playerinv, enemyinv;
    playerinv.insert("Health", 100); playerinv.insert("Coins", 0); playerinv.insert("Swords", 5);
    enemyinv.insert("Health", 100); enemyinv.insert("Coins", 0); enemyinv.insert("Swords", 5);

    bool updatePath = true;
    int orderedSet[225];
    int path[100];
    int pathSize = 0, pathPos = 0;

    Font f1;
    f1.loadFromFile("arial.ttf");

    Text score, enemyscore;
    score.setCharacterSize(20); score.setFont(f1);
    enemyscore.setCharacterSize(20); enemyscore.setFont(f1);

    Vector2i player = Vector2i(13, 13);
    RectangleShape playerSp = RectangleShape(Vector2f(40.f, 40.f));
    playerSp.setPosition(player.x * 40.f, player.y * 40.f);
    playerSp.setFillColor(Color(0, 180, 0));
    playerSp.setOutlineThickness(1.f);
    playerSp.setOutlineColor(Color(0, 0, 0));

    Vector2i villain = Vector2i(1, 1);
    RectangleShape villainSp = RectangleShape(Vector2f(40.f, 40.f));
    villainSp.setPosition(villain.x * 40.f, villain.y * 40.f);
    villainSp.setFillColor(Color(180, 0, 0));
    villainSp.setOutlineThickness(1.f);
    villainSp.setOutlineColor(Color(0, 0, 0));

    Texture coinTex;
    coinTex.loadFromFile("coin.png");
    Sprite coins[5];
    for (int i = 0; i < 5; i++) coins[i].setTexture(coinTex);
    int numCoins = 0, collectedCoins = 0;

    Texture swordTex;
    swordTex.loadFromFile("weapon.png");
    Sprite swords[5];
    for (int i = 0; i < 5; i++) swords[i].setTexture(swordTex);
    int numSwords = 0, collectedSwords = 5, enemySwords = 5;

    Texture healthTex;
    healthTex.loadFromFile("health.png");
    Sprite healthPoints[5];
    for (int i = 0; i < 5; i++) healthPoints[i].setTexture(healthTex);
    int numHealth = 0, health = 100, enemyHealth = 100;

    short *map = new short[225];
	RectangleShape cells[225];

	for (int i = 0; i < 15; i++) {
		for (int j = 0; j < 15; j++) {
            cells[i + j * 15].setPosition(i * 40.f, j * 40.f);
            cells[i + j * 15].setSize(Vector2f(40.f, 40.f));
            cells[i + j * 15].setOutlineThickness(1.f);
            cells[i + j * 15].setOutlineColor(Color(0, 0, 0));

            if (!(i == villain.x && j == villain.y) && !(i == player.x && j == player.y)) {
                if (rand() / (float)RAND_MAX < 0.22f || i == 0 || j == 0 || i == 14 || j == 14) {
                    map[i + j * 15] = 0;
                    cells[i + j * 15].setFillColor(Color(0, 0, 0));
                }
                else map[i + j * 15] = 1;
            }

		}
	}

    while (window.isOpen() && playerinv.get("Health") > 0 && enemyinv.get("Health") > 0) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) window.close();
            else if (event.type == Event::KeyPressed) {
                switch (event.key.code) {
                case Keyboard::Up: if (map[player.x + 15 * (player.y - 1)]) player.y--; break;
                case Keyboard::Down: if (map[player.x + 15 * (player.y + 1)]) player.y++; break;
                case Keyboard::Left: if (map[(player.x - 1) + 15 * player.y]) player.x--; break;
                case Keyboard::Right: if (map[(player.x + 1) + 15 * player.y]) player.x++; break;
                }
            }
            updatePath = true;
            pathSize = 0; pathPos = 0;
            playerSp.setPosition(player.x * 40.f, player.y * 40.f);
        }

        while (numCoins < 3) {  //  initialises coins
            int row, col;
            col = (rand() / (float)RAND_MAX) * 15.f;
            row = (rand() / (float)RAND_MAX) * 15.f;

            if (map[col + row * 15] == 1) {
                map[col + row * 15] = 2;
                coins[numCoins++].setPosition(col * 40.f, row * 40.f);
            }
        }

        while (numSwords < 3) {  //  initialises swords
            int row, col;
            col = (rand() / (float)RAND_MAX) * 15.f;
            row = (rand() / (float)RAND_MAX) * 15.f;

            if (map[col + row * 15] == 1) {
                map[col + row * 15] = 3;
                swords[numSwords++].setPosition(col * 40.f, row * 40.f);
            }
        }

        while (numHealth < 3) {  //  initialises health points
            int row, col;
            col = (rand() / (float)RAND_MAX) * 15.f;
            row = (rand() / (float)RAND_MAX) * 15.f;

            if (map[col + row * 15] == 1) {
                map[col + row * 15] = 4;
                healthPoints[numHealth++].setPosition(col * 40.f, row * 40.f);
            }
        }
        
        window.clear(Color(255,255,255));

        for (int i = 0; i < 225; i++) {
            window.draw(cells[i]);
        }
        enemyscore.setPosition(cells[210].getPosition());

        for (int i = 0; i < numCoins; i++) {
            window.draw(coins[i]);
        }

        for (int i = 0; i < numSwords; i++) {
            window.draw(swords[i]);
        }

        for (int i = 0; i < numHealth; i++) {
            window.draw(healthPoints[i]);
        }

        //  shortest path algo
        if (updatePath) {
            int counter = 0;
            int fullset[2000]; int fullsetSize = 0;
            int openset[100]; int opensetSize = 2;
            openset[0] = player.x + player.y * 15; openset[1] = counter;


            int currentIndex = player.x + player.y * 15;
            while (currentIndex != villain.x + villain.y * 15) {
                currentIndex = openset[0];
                counter = openset[1] + 1;
                int neighbors[4];
                if (currentIndex - 1 >= 0) neighbors[0] = currentIndex - 1;
                else neighbors[0] = currentIndex;
                if (currentIndex + 1 < 225) neighbors[1] = currentIndex + 1;
                else neighbors[1] = currentIndex;
                if (currentIndex - 15 >= 0) neighbors[2] = currentIndex - 15;
                else neighbors[2] = neighbors[0];
                if (currentIndex + 15 < 225) neighbors[3] = currentIndex + 15;
                else neighbors[3] = neighbors[1];

                for (int i = 0; i < 8; i += 2) {
                    bool alreadyExists = false;

                    for (int j = 0; j < fullsetSize; j += 2) {
                        if (neighbors[i / 2] == fullset[j]) {
                            alreadyExists = true; break;
                        }
                    }

                    if (!alreadyExists) {
                        if (map[neighbors[i / 2]]) {
                            fullset[fullsetSize++] = neighbors[i / 2];
                            fullset[fullsetSize++] = counter;

                            openset[opensetSize++] = neighbors[i / 2];
                            openset[opensetSize++] = counter;
                        }
                        else {
                            fullset[fullsetSize++] = neighbors[i / 2];
                            fullset[fullsetSize++] = 100000;
                        }
                    }
                }
                for (int i = 0; i < opensetSize; i++) {
                    openset[i] = openset[i + 2];
                }
                opensetSize -= 2;
            }
            for (int i = 0; i < 225; i++) {
                orderedSet[i] = 100000;
            }
            for (int i = 0; i < fullsetSize; i += 2) {
                orderedSet[fullset[i]] = fullset[i + 1];
            }
            orderedSet[player.x + player.y * 15] = 0;

            int pathIndex = villain.x + villain.y * 15;
            while (pathIndex != player.x + player.y * 15) {
                int neighbors[4];
                if (pathIndex - 1 >= 0) neighbors[0] = pathIndex - 1;
                else neighbors[0] = pathIndex;
                if (pathIndex + 1 < 225) neighbors[1] = pathIndex + 1;
                else neighbors[1] = pathIndex;
                if (pathIndex - 15 >= 0) neighbors[2] = pathIndex - 15;
                else neighbors[2] = neighbors[0];
                if (pathIndex + 15 < 225) neighbors[3] = pathIndex + 15;
                else neighbors[3] = neighbors[1];
                
                int lowest[2] = { 0,100000 };

                for (int i = 0; i < 4; i++) {
                    if (orderedSet[neighbors[i]] < lowest[1]) {
                        lowest[0] = i;
                        lowest[1] = orderedSet[neighbors[i]];
                    }
                }
                pathIndex = neighbors[lowest[0]];
                path[pathSize++] = pathIndex;
            }
            updatePath = false;
        }
        
        if (frameCount % moveTimer == 0) {
            villain.x = path[pathPos] % 15;
            villain.y = path[pathPos] / 15;
            villainSp.setPosition(villain.x * 40.f, villain.y * 40.f);
            pathPos++;

            if (moveTimer >= 250) {
                moveTimer = 1000 - frameCount / 250;
            }
        }
        //  end of shortest path algo

        if (playerSp.getPosition().x == villainSp.getPosition().x && playerSp.getPosition().y == villainSp.getPosition().y) {
            //  if the player and rival encounter each other

            if (enemyinv.get("Swords") > 0) {
                playerinv.set("Health", playerinv.get("Health") - 5 * enemyinv.get("Swords"));
                enemyinv.set("Swords", enemyinv.get("Swords") - 1);
            }
            if (playerinv.get("Swords") > 0) {
                enemyinv.set("Health", enemyinv.get("Health") - 5 * playerinv.get("Swords"));
                playerinv.set("Swords", playerinv.get("Swords") - 1);
            }

            //  resets both characters to starting position
            player.x = 13; player.y = 13; playerSp.setPosition(player.x * 40.f, player.y * 40.f);
            villain.x = 1; villain.y = 1; villainSp.setPosition(villain.x * 40.f, villain.y * 40.f);
        }
        score.setString("\t\tPlayer Stats - " + playerinv.display()); enemyscore.setString("\t\tEnemy Stats - " + enemyinv.display());

        window.draw(playerSp);
        window.draw(villainSp);
        window.draw(score);
        window.draw(enemyscore);

        window.display(); 
        
        if (map[player.x + player.y * 15] == 2) {     // if player grabs a coin
            map[player.x + player.y * 15] = 1;

            int coinIndex = -1;
            for (int i = 0; i < numCoins; i++) {
                if (coins[i].getPosition().x / 40 == player.x && coins[i].getPosition().y / 40 == player.y) {
                    coinIndex = i;
                }
            }
            if (coinIndex != numCoins - 1) {
                coins[coinIndex] = coins[numCoins - 1];
            }
            playerinv.set("Coins", playerinv.get("Coins") + 1);
            numCoins--;
        }

        if (map[villain.x + villain.y * 15] == 2) {     // if villain grabs a coin
            map[villain.x + villain.y * 15] = 1;

            int coinIndex = -1;
            for (int i = 0; i < numCoins; i++) {
                if (coins[i].getPosition().x / 40 == villain.x && coins[i].getPosition().y / 40 == villain.y) {
                    coinIndex = i;
                }
            }
            if (coinIndex != numCoins - 1) {
                coins[coinIndex] = coins[numCoins - 1];
            }
            enemyinv.set("Coins", enemyinv.get("Coins") + 1);
            numCoins--;
        }

        if (map[player.x + player.y * 15] == 3) {     // if player grabs a sword
            map[player.x + player.y * 15] = 1;

            int healthIndex = -1;
            for (int i = 0; i < numSwords; i++) {
                if (swords[i].getPosition().x / 40 == player.x && swords[i].getPosition().y / 40 == player.y) {
                    healthIndex = i;
                }
            }
            if (healthIndex != numSwords - 1) {
                swords[healthIndex] = swords[numSwords - 1];
            }
            playerinv.set("Swords", playerinv.get("Swords") + 1); numSwords--;
        }

        if (map[villain.x + villain.y * 15] == 3) {     // if enemy grabs a sword
            map[villain.x + villain.y * 15] = 1;

            int healthIndex = -1;
            for (int i = 0; i < numSwords; i++) {
                if (swords[i].getPosition().x / 40 == villain.x && swords[i].getPosition().y / 40 == villain.y) {
                    healthIndex = i;
                }
            }
            if (healthIndex != numSwords - 1) {
                swords[healthIndex] = swords[numSwords - 1];
            }
            enemyinv.set("Swords", enemyinv.get("Swords") + 1); numSwords--;
        }

        if (map[player.x + player.y * 15] == 4) {     // if player grabs a health
            map[player.x + player.y * 15] = 1;

            int healthIndex = -1;
            for (int i = 0; i < numHealth; i++) {
                if (healthPoints[i].getPosition().x / 40 == player.x && healthPoints[i].getPosition().y / 40 == player.y) {
                    healthIndex = i;
                }
            }
            if (healthIndex != numHealth - 1) {
                healthPoints[healthIndex] = healthPoints[numHealth - 1];
            }
            playerinv.set("Health", playerinv.get("Health") + 5);
            numHealth--;
        }

        if (map[villain.x + villain.y * 15] == 4) {     // if enemy grabs a health
            map[villain.x + villain.y * 15] = 1;

            int healthIndex = -1;
            for (int i = 0; i < numHealth; i++) {
                if (healthPoints[i].getPosition().x / 40 == villain.x && healthPoints[i].getPosition().y / 40 == villain.y) {
                    healthIndex = i;
                }
            }
            if (healthIndex != numHealth - 1) {
                healthPoints[healthIndex] = healthPoints[numHealth - 1];
            }
            enemyinv.set("Health", enemyinv.get("Health") + 5);
            numHealth--;
        }
        
        frameCount++;
    }

    delete[] map;

    int p = playerinv.get("Coins"), e = enemyinv.get("Coins");
    cout << "Your coins: " << p << endl;
    cout << "Enemy coins: " << e << endl;
    if (p == e) cout << "Issa draw!\n";
    else p > e ? cout << "You win!\n" : cout << "You lose!\n";

    return 0;
}
