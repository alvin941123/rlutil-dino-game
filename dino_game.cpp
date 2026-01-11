#include <iostream>
#include <cstdlib>
#include <ctime>
#include "rlutil.h"

using namespace std;

// 遊戲常數定義
const int SCREEN_WIDTH = 80;
const int SCREEN_HEIGHT = 20;
const int GROUND_Y = 15;
const int DINO_X = 10;

// 遊戲狀態
int dinoY = GROUND_Y;
int dinoVelocity = 0;
bool isJumping = false;
int obstacleX = SCREEN_WIDTH;
int score = 0;
bool gameOver = false;

// 重力與跳躍參數
const int GRAVITY = 1;
const int JUMP_STRENGTH = -8;
const int GAME_SPEED = 50; // 毫秒

void drawBorder() {
    rlutil::setColor(rlutil::CYAN);
    // 上邊界
    for (int x = 1; x <= SCREEN_WIDTH; x++) {
        rlutil::locate(x, 1);
        cout << "=";
    }
    // 下邊界
    for (int x = 1; x <= SCREEN_WIDTH; x++) {
        rlutil::locate(x, SCREEN_HEIGHT);
        cout << "=";
    }
}

void drawGround() {
    rlutil::setColor(rlutil::BROWN);
    for (int x = 1; x <= SCREEN_WIDTH; x++) {
        rlutil::locate(x, GROUND_Y + 1);
        cout << "-";
    }
}

void drawDino() {
    rlutil::setColor(rlutil::GREEN);
    rlutil::locate(DINO_X, dinoY);
    cout << "@";
}

void drawObstacle() {
    rlutil::setColor(rlutil::RED);
    if (obstacleX > 0 && obstacleX <= SCREEN_WIDTH) {
        rlutil::locate(obstacleX, GROUND_Y);
        cout << "#";
    }
}

void drawScore() {
    rlutil::setColor(rlutil::YELLOW);
    rlutil::locate(SCREEN_WIDTH - 15, 2);
    cout << "分數: " << score << "  ";
}

void drawInstructions() {
    rlutil::setColor(rlutil::WHITE);
    rlutil::locate(5, 2);
    cout << "[空白鍵/↑: 跳躍] [ESC: 結束]";
}

void clearDino(int y) {
    rlutil::locate(DINO_X, y);
    cout << " ";
}

void clearObstacle(int x) {
    if (x > 0 && x <= SCREEN_WIDTH) {
        rlutil::locate(x, GROUND_Y);
        cout << " ";
    }
}

void updatePhysics() {
    // 更新恐龍位置
    if (isJumping || dinoY < GROUND_Y) {
        dinoVelocity += GRAVITY;
        int newY = dinoY + dinoVelocity;
        
        if (newY >= GROUND_Y) {
            newY = GROUND_Y;
            dinoVelocity = 0;
            isJumping = false;
        }
        
        clearDino(dinoY);
        dinoY = newY;
    }
    
    // 更新障礙物位置
    clearObstacle(obstacleX);
    obstacleX -= 1;
    
    // 障礙物移出畫面後重置
    if (obstacleX < 0) {
        obstacleX = SCREEN_WIDTH + rand() % 20;
        score += 10;
    }
}

bool checkCollision() {
    // 檢查恐龍與障礙物碰撞
    if (obstacleX == DINO_X && dinoY == GROUND_Y) {
        return true;
    }
    return false;
}

void handleInput() {
    if (rlutil::kbhit()) {
        int key = rlutil::getkey();
        
        // 空白鍵或向上鍵跳躍
        if ((key == ' ' || key == rlutil::KEY_UP) && !isJumping && dinoY == GROUND_Y) {
            isJumping = true;
            dinoVelocity = JUMP_STRENGTH;
        }
        
        // ESC 結束遊戲
        if (key == rlutil::KEY_ESCAPE) {
            gameOver = true;
        }
    }
}

void gameOverScreen() {
    rlutil::cls();
    rlutil::setColor(rlutil::RED);
    
    int centerY = SCREEN_HEIGHT / 2;
    rlutil::locate(SCREEN_WIDTH / 2 - 10, centerY - 2);
    cout << "===================";
    rlutil::locate(SCREEN_WIDTH / 2 - 10, centerY - 1);
    cout << "   遊戲結束！";
    rlutil::locate(SCREEN_WIDTH / 2 - 10, centerY);
    cout << "===================";
    
    rlutil::setColor(rlutil::YELLOW);
    rlutil::locate(SCREEN_WIDTH / 2 - 10, centerY + 2);
    cout << "最終分數: " << score;
    
    rlutil::setColor(rlutil::WHITE);
    rlutil::locate(SCREEN_WIDTH / 2 - 15, centerY + 4);
    cout << "感謝遊玩！按任意鍵退出...";
    
    rlutil::anykey();
}

void initGame() {
    rlutil::cls();
    rlutil::hidecursor();
    srand(time(0));
    
    dinoY = GROUND_Y;
    dinoVelocity = 0;
    isJumping = false;
    obstacleX = SCREEN_WIDTH;
    score = 0;
    gameOver = false;
}

int main() {
    initGame();
    
    // 繪製靜態元素
    drawBorder();
    drawGround();
    drawInstructions();
    
    // 主遊戲迴圈
    while (!gameOver) {
        handleInput();
        updatePhysics();
        
        if (checkCollision()) {
            gameOver = true;
            break;
        }
        
        drawDino();
        drawObstacle();
        drawScore();
        
        rlutil::msleep(GAME_SPEED);
    }
    
    // 遊戲結束畫面
    gameOverScreen();
    
    rlutil::showcursor();
    return 0;
}