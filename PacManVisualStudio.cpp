#include <iostream>
#include <conio.h>
#include <cstdlib>
#include <windows.h>
#include <string>
#include <sstream>
#include <stdlib.h>
#include <time.h>
#include <fstream>
#include <unistd.h>
#include <chrono>

using namespace std;

#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_LEFT 75
#define KEY_RIGHT 77
#define ESCAPE 27
#define W 32
#define M 178
#define PLAYER 153
#define G 148
#define P1 46
#define P2 175

// Game variables
bool ifGame = true;
bool ifChange;

clock_t CurrentFrame;
clock_t PrevFrame;
clock_t deltaTime = 0;

// Map Variables
const int MAX_MAP_SIZE = 20;
const int MAP_SIZE_X = 20;
const int MAP_SIZE_Y = 19;

int iMapSizeX = 20;
int iMapSizeY = 20;
int iMapFromFile[MAX_MAP_SIZE][MAX_MAP_SIZE];
int iSecret[MAX_MAP_SIZE][MAX_MAP_SIZE];

int iMap[MAX_MAP_SIZE][MAX_MAP_SIZE] = {
    {M,M,M,M,M,M,M,M,M,W,M,M,M,M,M,M,M,M,M,M},                              //LINE 1
    {M,P1,P1,P1,P1,P1,P1,P1,P1,P1,P1,P1,P1,P1,P1,P1,P1,P1,P2,M},            //LINE 2
    {M,P1,M,M,M,M,P1,M,M,M,P1,M,M,P1,M,M,M,M,P1,M},                         //LINE 3
    {M,P1,M,M,M,M,P1,M,M,M,P1,M,M,P1,M,M,M,M,P1,M},                         //LINE 4
    {M,P1,M,P1,P1,P1,P1,P1,M,M,P1,M,P1,P1,P1,P1,P1,M,P1,M},                 //LINE 5
    {M,P1,M,P1,M,M,M,P1,M,M,P1,M,P1,M,M,M,P1,M,P1,M},                       //LINE 6
    {M,P1,M,P1,M,M,M,P1,M,M,P1,M,P1,M,M,M,P1,M,P1,M},                       //LINE 7
    {M,P1,M,P1,M,M,P1,P1,P1,P1,P1,P1,P1,P1,M,M,P1,M,P1,M},                  //LINE 8
    {M,P1,M,P1,M,M,P1,M,M,P1,M,M,M,P1,M,M,P1,M,P1,M},                       //LINE 9
    {W,P1,P1,P1,P1,P1,P1,M,P1,P1,P1,P1,M,P1,P1,P1,P1,P1,P1,W},              //LINE 10
    {M,P1,M,P1,M,M,P1,M,M,M,M,M,M,P1,M,M,P1,M,P1,M},                        //LINE 11
    {M,P1,M,P1,M,M,P1,P1,P1,P1,P1,P1,P1,P1,M,M,P1,M,P1,M},                  //LINE 12
    {M,P1,M,P1,M,M,M,P1,M,P1,M,M,P1,M,M,M,P1,M,P1,M},                       //LINE 13
    {M,P1,M,P1,M,M,M,P1,M,P1,M,M,P1,M,M,M,P1,M,P1,M},                       //LINE 14
    {M,P1,M,P1,P1,P1,P1,P1,M,P1,M,M,P1,P1,P1,P1,P1,M,P1,M},                 //LINE 15
    {M,P1,M,M,M,M,P1,M,M,P1,M,M,M,P1,M,M,M,M,P1,M},                         //LINE 16
    {M,P1,M,M,M,M,P1,M,M,P1,M,M,M,P1,M,M,M,M,P1,M},                         //LINE 17
    {M,P1,P1,P1,P1,P1,P1,P1,P1,P1,P1,P1,P1,P1,P1,P1,P1,P1,P1,M},            //LINE 18
    {M,M,M,M,M,M,M,M,M,M,M,W,M,M,M,M,M,M,M,M}                               //LINE 19
};

class Player {
private:
    int X = 9;
    int Y = 11;
    int iScore = 0;
    float fVelocity = 1.0;
    int iPlayerSign = PLAYER;
    bool ifBuff = false;
    bool ifDebuff = false;
    chrono::steady_clock::time_point tBuffTime;
    chrono::steady_clock::time_point tDebuffTime;

public:
    // Change position of a player
    void ChangePosition(int pos_X, int pos_Y) {
        if (pos_X >= 0) {
            X = pos_X;
        }
        if (pos_Y >= 0) {
            Y = pos_Y;
        }
    }

    // Score
    void SetScore(int value) {
        iScore = value;
    }
    int GetScore() {
        return iScore;
    }

    // Buff and debuff
    int getBuff() {
        return ifBuff;
    }
    void setBuff(bool ifBuffed) {
        ifBuff = ifBuffed;
    }
    void changeBuff() {
        tBuffTime = chrono::steady_clock::now() + chrono::seconds(5);
        ifBuff = true;
    }
    chrono::steady_clock::time_point getBuffTime() {
        return tBuffTime;
    }

    void setDebuff(bool ifDebuffed) {
        ifDebuff = ifDebuffed;
    }
    int getDebuff() {
        return ifDebuff;
    }
    void changeDebuff() {
        tDebuffTime = chrono::steady_clock::now() + chrono::seconds(5);
        ifDebuff = true;
    }
    chrono::steady_clock::time_point getDebuffTime() {
        return tDebuffTime;
    }

    // Y axis of player
    void setY(float value) {
        Y = value;
    }
    void addY(float value) {
        Y = Y + value;
    }
    int getY() {
        return ((int)Y);
    }

    // X axis of player
    void setX(float value) {
        X = value;
    }
    void addX(float value) {
        X = X + value;
    }
    int getX() {
        return ((int)X);
    }

    // Velocity of player
    void setVelocity(float vel) {
        fVelocity = vel;
    }
    float getVelocity() {
        return fVelocity;
    }
};

class Map {
public:

    int iMaxScore = 164;

    void SaveMapFile(char* FileName) {
        fstream file;
        file.open(FileName, ios::out);

        if (file.good() == true) {
            file << MAP_SIZE_X << " " << '\n';
            file << MAP_SIZE_Y << " " << '\n';
            for (int i = 0; i < MAP_SIZE_Y; i++) {
                for (int j = 0; j < MAP_SIZE_X; j++) {
                    file << iMap[i][j] << " ";
                }// endfor j < MAP_SIZE_X
                file << '\n';
            }// endfor j < MAP_SIZE_Y
            file.close();
        }
        else {
            cout << "File missing";
        }// endif file.good() == true
    }

    void DrawMap() {
        for (int i = 0; i <= MAP_SIZE_Y - 1; i++) {
            for (int j = 0; j <= MAP_SIZE_X - 1; j++) {
                iMapFromFile[i][j] = iMap[i][j];
                cout << char(iMapFromFile[i][j]);
            }// end for j <= MAP_SIZE_X
            cout << '\n';
        }// endfor i <= MAP_SIZE_Y
    }

    void LoadMapFile(char* FileName) {
        fstream file;
        file.open(FileName, ios::in);

        if (file.good() == false) {
            cout << "File doesn't exist" << '\n';
            exit(0);
        }
        else {
            file >> iMapSizeX;
            file >> iMapSizeY;
            for (int i = 0; i < iMapSizeY; i++) {
                for (int j = 0; j < iMapSizeX; j++) {
                    file >> iMapFromFile[i][j];
                }// endfor j < iMapSizeX
            }// endfor i < iMapSizeY
        }// endif file.good() == false
    }
};

class Ghost {
private:
    int X = 9;
    int Y = 9;
    int iScore = 0;
    int iGhostSign = G;
    float fVelocity = 1.2;
    int ifBuff = false;
    int ifDebuff = false;


public:

    void ChangePosition(int pos_X, int pos_Y) {
        if (pos_X >= 0) {
            X = pos_X;
        }
        if (pos_Y >= 0) {
            Y = pos_Y;
        }
    }


    void addAxis(int pos_X, int pos_Y) {
        X += pos_X;
        Y += pos_Y;
    }


    // Buff and debuff functions
    bool setBuff(bool ifBuffed) {
        ifBuff = ifBuffed;
        fVelocity = 5.97;
    }
    int getBuff() {
        return ifBuff;
    }
    bool setDebuff(bool ifDebuffed) {
        ifDebuff = ifDebuffed;
    }
    int getDebuff() {
        return ifDebuff;
    }


    // Y axis of Ghost
    void setY(float value) {
        Y = value;
    }
    void addY(float value) {
        Y = Y + value;
    }
    int getY() {
        return ((int)Y);
    }


    // X axis of Ghost
    void setX(float value) {
        X = value;
    }
    void addX(float value) {
        X = X + value;
    }
    int getX() {
        return ((int)X);
    }

    // Velocity of ghost
    void setVelocity(float vel) {
        fVelocity = vel;
    }
    float getVelocity() {
        return fVelocity;
    }
};

// Class Objects definition
Player Player1;
Map mainMap;
Ghost Ghost1;

void GhostMovement() {
    // Ghost movement
    //If the block above is not a wall - move there
    if (iMap[Ghost1.getY() + 1][Ghost1.getX()] != M) {
        Ghost1.addY(1);
        ifGame = true;
    }
    //If the block under is not a wall - move there
    else if (iMap[Ghost1.getY() - 1][Ghost1.getX()] != M) {
        Ghost1.addY(-1);
        ifGame = true;
    }
    //If the block on the right is not a wall - move there
    else if (iMap[Ghost1.getY()][Ghost1.getX() + 1] != M) {
        Ghost1.addX(1);
        ifGame = true;
    }
    //If the block on the left is not a wall - move there
    else if (iMap[Ghost1.getY()][Ghost1.getX() - 1] != M) {
        Ghost1.addX(-1);
        ifGame = true;
    }
}

void MapOutput() {
    // Drawing map
    if (ifChange) {
        system("cls");
        string sMapa = "";
        for (int i = 0; i < MAP_SIZE_Y; i++) {
            for (int j = 0; j < MAP_SIZE_X; j++) {
                if (Player1.getX() == j && Player1.getY() == i) {
                    sMapa += char(PLAYER);
                }// endif Player1.getX() == j && Player1.getY() == i
                else if (Ghost1.getX() == j && Ghost1.getY() == i) {
                    sMapa += char(G);
                }// endif Ghost1.getX() == j && Ghost1.getY() == i
                else {
                    sMapa += char(iMap[i][j]);
                }
            }// endfor j < MAP_SIZE_X
            sMapa += '\n';
        }// endfor i < MAP_SIZE_Y

        // Outputing score, velocity, SpeedBuff timer to check if everything is working properly.
        if (Player1.GetScore() != mainMap.iMaxScore) {
            sMapa += "Score: " + to_string(Player1.GetScore());
            //sMapa += "\nVelocity: " + to_string(Movement);
            sMapa += "\nifBuffed: " + to_string(Player1.getBuff());
        }
        else if (Player1.GetScore() == mainMap.iMaxScore) {
            sMapa += "\tYOU WON!";
        }
        cout << sMapa;
    }
    Sleep(120);
}

void PlayerMovement() {
    // Move down
    if (GetAsyncKeyState(VK_DOWN))
    {
        if (iMap[Player1.getY() + 1][Player1.getX()] != M)
        {
            Player1.addY(1);
            ifChange = true;
        }// endif iMap[Player1.getY() + 1][Player1.getX()] != M

       // Warp to the up side of the map
        if (Player1.getY() == 18 && Player1.getX() == 11)
        {
            Player1.setX(9);
            Player1.setY(0);
        }// endif Player1.getY() == 18 && Player1.getX() == 11
    }// endif (GetAsyncKeyState(VK_DOWN))

    // Move up
    if (GetAsyncKeyState(VK_UP))
    {
        if (iMap[Player1.getY() - 1][Player1.getX()] != M)
        {
            Player1.addY(-1);
            ifChange = true;
        }// endif (iMap[Player1.getY() - 1][Player1.getX()] != M)

    // Warp to the down side of the map
        if (Player1.getY() == 0 && Player1.getX() == 9)
        {
            Player1.setX(11);
            Player1.setY(18);
        }// endif (Player1.getY() == 0 && Player1.getX() == 9)
    } // endif GetAsyncKeyState(VK_UP)

    // Move to the left
    if (GetAsyncKeyState(VK_LEFT))
    {
        if (iMap[Player1.getY()][Player1.getX() - 1] != M)
        {
            Player1.addX(-1);
            ifChange = true;
        }// endif (iMap[Player1.getY()][Player1.getX() - 1] != M)

        // Warp to the right side of the map
        if (Player1.getY() == 9 && Player1.getX() == 0)
        {
            Player1.setX(19);
            Player1.setY(9);
        }// endif (Player1.getY() == 9 && Player1.getX() == 0)
    }// endif GetAsyncKeyState(VK_LEFT)

    // Move to the right
    if (GetAsyncKeyState(VK_RIGHT)) 
    {
        if (iMap[Player1.getY()][Player1.getX() + 1] != M)
        {
            Player1.addX(1);
            ifChange = true;
        }// endif (iMap[Player1.getY()][Player1.getX() + 1] != M)

        // Warp to the left side of the map
        if (Player1.getY() == 9 && Player1.getX() == 19)
        {
            Player1.setX(0);
            Player1.setY(9);
        }// endif (Player1.getY() == 9 && Player1.getX() == 19)
    }//endif GetAsyncKeyState(VK_RIGHT)
}

// Main loop
int main()
{
    // Velocity related variables
    CurrentFrame = clock();
    PrevFrame = clock();

    // Save map to file/Load map from file/Draw map from variable
    // mainMap.SaveMapFile("Map.txt");
    // mainMap.LoadMapFile("Map.txt");
    // mainMap.DrawMap();

    // Main game loop
    while (ifGame) 
    {
        // Time and velocity related variables
        PrevFrame = CurrentFrame;
        CurrentFrame = clock();
        deltaTime = CurrentFrame - PrevFrame;
        float Movement = deltaTime / 1000.0 * Player1.getVelocity();

        for (int i = 0; i <= Movement; i++) 
        {
            PlayerMovement();

            // Score +1 for P1
            if (iMap[Player1.getY()][Player1.getX()] == P1) 
            {
                iMap[Player1.getY()][Player1.getX()] = W;
                Player1.SetScore(Player1.GetScore() + 1);
                ifChange = true;
            }

            // Score +2 for P2
            if (iMap[Player1.getY()][Player1.getX()] == P2) 
            {
                iMap[Player1.getY()][Player1.getX()] = W;
                Player1.SetScore(Player1.GetScore() + 2);

                if (chrono::steady_clock::now() > Player1.getBuffTime()) 
                {
                    Player1.setBuff(false);
                    Player1.setVelocity(1);
                }
                else 
                {
                    Player1.setVelocity(5.97);
                }
                ifChange = true;
            }//endif iMap[Player1.getY()][Player1.getX()] == P2
        }// endfor i <= Movement;

        // Exit game
        if (GetAsyncKeyState(VK_ESCAPE)) 
        {
            ifGame = false;
        }

        GhostMovement();
        MapOutput();
    }// endwhile (ifGame)
}
