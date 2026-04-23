#include <iostream>
#include <conio.h>   // for _kbhit() and _getch()
#include <windows.h> // for Sleep()
#include <vector>
#include <cstdlib>
using namespace std;

// Base class for all game objects
class GameObject {
protected:
    int x, y;
public:
    GameObject(int x=0, int y=0) : x(x), y(y) {}
    virtual void draw() = 0; // pure virtual
    int getX() const { return x; }
    int getY() const { return y; }
    void setPosition(int newX, int newY) { x = newX; y = newY; }
};

// Snake part (head/tail)
class SnakePart : public GameObject {
public:
    SnakePart(int x=0, int y=0) : GameObject(x,y) {}
    void draw() override { cout << "o"; }
};

// Base fruit class
class Fruit : public GameObject {
protected:
    int points;
public:
    Fruit(int x=0, int y=0, int p=10) : GameObject(x,y), points(p) {}
    virtual void draw() override { cout << "F"; }
    int getPoints() const { return points; }
};

// Different fruit types
class Apple : public Fruit {
public:
    Apple(int x=0, int y=0) : Fruit(x,y,10) {}
    void draw() override { cout << "A"; }
};

class Banana : public Fruit {
public:
    Banana(int x=0, int y=0) : Fruit(x,y,20) {}
    void draw() override { cout << "B"; }
};

class Cherry : public Fruit {
public:
    Cherry(int x=0, int y=0) : Fruit(x,y,30) {}
    void draw() override { cout << "C"; }
};

// SnakeGame class
class SnakeGame {
private:
    bool gameOver;
    const int width = 20, height = 20;
    GameObject* head;
    vector<SnakePart> tail;
    Fruit* fruit;
    int score;
    enum eDirection { STOP=0, LEFT, RIGHT, UP, DOWN };
    eDirection dir;

public:
    SnakeGame() {
        head = new SnakePart(width/2, height/2);
        fruit = nullptr;
        setup();
    }

    ~SnakeGame() {
        delete head;
        delete fruit;
    }

    void setup() {
        gameOver = false;
        dir = STOP;
        score = 0;
        tail.clear();
        spawnFruit();
    }

    void spawnFruit() {
        if (fruit) delete fruit;
        int choice = rand() % 3;
        int fx = rand() % width;
        int fy = rand() % height;
        if (choice == 0) fruit = new Apple(fx, fy);
        else if (choice == 1) fruit = new Banana(fx, fy);
        else fruit = new Cherry(fx, fy);
    }

    void draw() {
        system("cls");
        for (int i=0; i<width+2; i++) cout << "#";
        cout << endl;

        for (int i=0; i<height; i++) {
            for (int j=0; j<width; j++) {
                if (j==0) cout << "#";

                if (i == head->getY() && j == head->getX())
                    cout << "O";
                else if (i == fruit->getY() && j == fruit->getX())
                    fruit->draw();
                else {
                    bool printed = false;
                    for (auto &t : tail) {
                        if (t.getX()==j && t.getY()==i) {
                            t.draw();
                            printed = true;
                            break;
                        }
                    }
                    if (!printed) cout << " ";
                }

                if (j==width-1) cout << "#";
            }
            cout << endl;
        }

        for (int i=0; i<width+2; i++) cout << "#";
        cout << endl;
        cout << "Score: " << score << endl;
    }

    void input() {
        if (_kbhit()) {
            switch (_getch()) {
                case 'a': dir = LEFT; break;
                case 'd': dir = RIGHT; break;
                case 'w': dir = UP; break;
                case 's': dir = DOWN; break;
                case 'x': gameOver = true; break;
            }
        }
    }

    void logic() {
        // Move tail
        int prevX = head->getX();
        int prevY = head->getY();
        int prev2X, prev2Y;
        for (size_t i=0; i<tail.size(); i++) {
            prev2X = tail[i].getX();
            prev2Y = tail[i].getY();
            tail[i].setPosition(prevX, prevY);
            prevX = prev2X;
            prevY = prev2Y;
        }

        // Move head
        switch (dir) {
            case LEFT:  head->setPosition(head->getX()-1, head->getY()); break;
            case RIGHT: head->setPosition(head->getX()+1, head->getY()); break;
            case UP:    head->setPosition(head->getX(), head->getY()-1); break;
            case DOWN:  head->setPosition(head->getX(), head->getY()+1); break;
            default: break;
        }

        // Collision with walls
        if (head->getX() >= width || head->getX() < 0 || head->getY() >= height || head->getY() < 0)
            gameOver = true;

        // Collision with tail
        for (auto &t : tail)
            if (t.getX() == head->getX() && t.getY() == head->getY())
                gameOver = true;

        // Eating fruit
        if (head->getX() == fruit->getX() && head->getY() == fruit->getY()) {
            score += fruit->getPoints();
            tail.push_back(SnakePart());
            spawnFruit();
        }
    }

    void run() {
        while (!gameOver) {
            draw();
            input();
            logic();
            Sleep(100);
        }
        cout << "Game Over! Final Score: " << score << endl;
    }
};

int main() {
    SnakeGame game;
    game.run();
    return 0;
}
