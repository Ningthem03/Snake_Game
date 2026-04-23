#include <iostream>
#include <conio.h>   // for _kbhit() and _getch()
#include <windows.h> // for Sleep()
using namespace std;

class SnakeGame {
private:
    bool gameOver;
    const int width = 20;
    const int height = 20;
    int x, y;          // snake head position
    int fruitX, fruitY;
    int score;
    int tailX[100], tailY[100];
    int nTail;
    enum eDirection { STOP = 0, LEFT, RIGHT, UP, DOWN };
    eDirection dir;

public:
    SnakeGame() {
        setup();
    }

    void setup() {
        gameOver = false;
        dir = STOP;
        x = width / 2;
        y = height / 2;
        fruitX = rand() % width;
        fruitY = rand() % height;
        score = 0;
        nTail = 0;
    }

    void draw() {
        system("cls"); // clear console

        // Top wall
        for (int i = 0; i < width+2; i++) cout << "#";
        cout << endl;

        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                if (j == 0) cout << "#"; // left wall

                if (i == y && j == x)
                    cout << "O"; // snake head
                else if (i == fruitY && j == fruitX)
                    cout << "F"; // fruit
                else {
                    bool print = false;
                    for (int k = 0; k < nTail; k++) {
                        if (tailX[k] == j && tailY[k] == i) {
                            cout << "o"; // snake tail
                            print = true;
                        }
                    }
                    if (!print) cout << " ";
                }

                if (j == width-1) cout << "#"; // right wall
            }
            cout << endl;
        }

        // Bottom wall
        for (int i = 0; i < width+2; i++) cout << "#";
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
        int prevX = tailX[0];
        int prevY = tailY[0];
        int prev2X, prev2Y;
        tailX[0] = x;
        tailY[0] = y;
        for (int i = 1; i < nTail; i++) {
            prev2X = tailX[i];
            prev2Y = tailY[i];
            tailX[i] = prevX;
            tailY[i] = prevY;
            prevX = prev2X;
            prevY = prev2Y;
        }

        switch (dir) {
            case LEFT:  x--; break;
            case RIGHT: x++; break;
            case UP:    y--; break;
            case DOWN:  y++; break;
            default: break;
        }

        // Collision with walls
        if (x >= width || x < 0 || y >= height || y < 0)
            gameOver = true;

        // Collision with tail
        for (int i = 0; i < nTail; i++)
            if (tailX[i] == x && tailY[i] == y)
                gameOver = true;

        // Eating fruit
        if (x == fruitX && y == fruitY) {
            score += 10;
            fruitX = rand() % width;
            fruitY = rand() % height;
            nTail++;
        }
    }

    void run() {
        while (!gameOver) {
            draw();
            input();
            logic();
            Sleep(100); // control speed
        }
        cout << "Game Over! Final Score: " << score << endl;
    }
};

int main() {
    SnakeGame game;
    game.run();
    return 0;
}
