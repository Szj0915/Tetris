#include <conio.h>
#include <iostream>
#include <random>
#include <string>
#include <windows.h>

namespace console
{
    void hideCursor()
    {
        HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
        CONSOLE_CURSOR_INFO cursorInfo;
        GetConsoleCursorInfo(hOut, &cursorInfo);
        cursorInfo.bVisible = FALSE;
        SetConsoleCursorInfo(hOut, &cursorInfo);
    }

    void showCursor()
    {
        HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
        CONSOLE_CURSOR_INFO cursorInfo;
        GetConsoleCursorInfo(hOut, &cursorInfo);
        cursorInfo.bVisible = TRUE;
        SetConsoleCursorInfo(hOut, &cursorInfo);
    }

    void gotoxy(int x, int y)
    {
        COORD pos = { (SHORT)x, (SHORT)y };
        HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleCursorPosition(hOut, pos);
    }
}

namespace game
{
    bool running = 1;
    int score = 0;
    bool game_over = 0;
    short speedcounter = 0;
}

namespace UI
{
    std::string chessboard[25] = {
        "|..........|", // 0 hide
        "|..........|", // 1
        "|..........|", // 2
        "|..........|", // 3
        "|..........|", // 4 show
        "|..........|", // 5
        "|..........|", // 6
        "|..........|", // 7
        "|..........|", // 8
        "|..........|", // 9
        "|..........|", // 10
        "|..........|", // 11
        "|..........|", // 12
        "|..........|", // 13
        "|..........|", // 14
        "|..........|", // 15
        "|..........|", // 16
        "|..........|", // 17
        "|..........|", // 18
        "|..........|", // 19
        "|..........|", // 20
        "|..........|", // 21
        "|..........|", // 22
        "|..........|", // 23
        "|----------|"  // 24
    };

    void Depict()
    {
        console::gotoxy(0, 0);
        std::string printing = "";
        for (int i = 4; i <= 24; i++)
        {
            printing += chessboard[i] + "\n";
        }
        printing += "running: ";
        if (game::running)
            printing += "true \n";
        else
            printing += "false\n";

        printing += "score: ";
        printing += std::to_string(game::score);
        printing += "                                    \n";

        if (game::game_over)
            printing += "Game over!";
        else
            printing += "                            ";

        std::cout << printing;
    }
}

namespace chess
{
    void elimination()
    {
        short eli = 0, scADD = 1;
    other:
        for (int i = 4; i <= 23; i++)
        {
            if (UI::chessboard[i] == "|OOOOOOOOOO|")
                eli = i;
        }

        if (eli >= 4 && eli <= 23)
        {
            for (int i = eli; i >= 4; i--)
            {
                UI::chessboard[i] = UI::chessboard[i - 1];
            }
            game::score += scADD;
            game::speedcounter += scADD;
            scADD++;
            eli = 0;
            goto other;
        }
    }
}

class Dominoes
{
    short center = 0;
    short num = 0;
    short X[4] = { 5,6,7,7 }, Y[4] = { 0,0,0,1 };

    bool collision(short newX[4], short newY[4])
    {
        for (int i = 0; i <= 3; i++)
        {
            if (newX[i] < 1 || newX[i] > 10 || newY[i] < 0 || newY[i] > 23)
                return 1;
            if (UI::chessboard[newY[i]][newX[i]] != '.')
                return 1;
        }
        return 0;
    }

public:
    void landing()
    {
        chess::elimination();
        if (UI::chessboard[4] != "|..........|")
        {
            game::game_over = 1;
            game::running = 0;
        }

        if (!game::game_over)
        {
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_int_distribution<int> dist(0, 6);
            short r = dist(gen);
            num = r;

            switch (r)
            {
            case 0:
            {
                X[0] = 4; X[1] = 5; X[2] = 6; X[3] = 7;
                Y[0] = 0; Y[1] = 0; Y[2] = 0; Y[3] = 0;
                center = 1;
                break;
            }
            case 1:
            {
                X[0] = 5; X[1] = 6; X[2] = 5; X[3] = 6;
                Y[0] = 0; Y[1] = 0; Y[2] = 1; Y[3] = 1;
                center = 0;
                break;
            }
            case 2:
            {
                X[0] = 4; X[1] = 5; X[2] = 6; X[3] = 5;
                Y[0] = 0; Y[1] = 0; Y[2] = 0; Y[3] = 1;
                center = 1;
                break;
            }
            case 3:
            {
                X[0] = 4; X[1] = 5; X[2] = 6; X[3] = 6;
                Y[0] = 0; Y[1] = 0; Y[2] = 0; Y[3] = 1;
                center = 1;
                break;
            }
            case 4:
            {
                X[0] = 4; X[1] = 5; X[2] = 6; X[3] = 4;
                Y[0] = 0; Y[1] = 0; Y[2] = 0; Y[3] = 1;
                center = 1;
                break;
            }
            case 5:
            {
                X[0] = 4; X[1] = 5; X[2] = 5; X[3] = 6;
                Y[0] = 1; Y[1] = 1; Y[2] = 0; Y[3] = 0;
                center = 2;
                break;
            }
            case 6:
            {
                X[0] = 4; X[1] = 5; X[2] = 5; X[3] = 6;
                Y[0] = 0; Y[1] = 0; Y[2] = 1; Y[3] = 1;
                center = 1;
                break;
            }
            }
        }
    }

    bool island()
    {
        std::string chbo[25] = {};
        for (int i = 0; i <= 24; i++)
        {
            chbo[i] = UI::chessboard[i];
        }
        for (int i = 0; i <= 3; i++)
        {
            chbo[Y[i]][X[i]] = '.';
        }
        for (int i = 0; i <= 3; i++)
        {
            if (chbo[Y[i] + 1][X[i]] != '.')
            {
                landing();
                return 1;
            }
        }
        return 0;
    }

    void rotate(short times)
    {
        std::string chbo[25] = {};
        for (int i = 0; i < 24; i++)
        {
            chbo[i] = UI::chessboard[i];
        }
        short oldX[4], oldY[4], newX[4], newY[4];
        for (int i = 0; i <= 3; i++)
        {
            oldX[i] = X[i];
            oldY[i] = Y[i];
        }

        if (num == 1 || !game::running)
            return;

        for (int i = 0; i <= 3; i++)
        {
            UI::chessboard[Y[i]][X[i]] = '.';
        }

        for (int t = 0; t <= times - 1; t++)
        {
            for (int i = 0; i <= 3; i++)
            {
                if (i == center)
                {
                    newX[i] = oldX[i];
                    newY[i] = oldY[i];
                }
                else
                {
                    newX[i] = oldX[center] - (oldY[i] - oldY[center]);
                    newY[i] = oldY[center] + (oldX[i] - oldX[center]);
                }
            }
            for (int i = 0; i <= 3; i++)
            {
                oldX[i] = newX[i];
                oldY[i] = newY[i];
            }
        }

        if (!collision(newX, newY))
        {
            for (int i = 0; i <= 3; i++)
            {
                X[i] = newX[i];
                Y[i] = newY[i];
            }
        }

        for (int i = 0; i <= 3; i++)
        {
            UI::chessboard[Y[i]][X[i]] = 'O';
        }
        island();
    }

    void move(short Xsize, short Ysize, bool doland)
    {
        short newX[4] = { 0 }, newY[4] = { 0 };
        for (int i = 0; i <= 3; i++)
        {
            newX[i] = X[i] + Xsize;
            newY[i] = Y[i] + Ysize;
        }

        for (int i = 0; i <= 3; i++)
        {
            UI::chessboard[Y[i]][X[i]] = '.';
        }

        if (Ysize < 0 || collision(newX, newY) || !game::running || game::game_over)
        {
            for (int i = 0; i <= 3; i++)
            {
                UI::chessboard[Y[i]][X[i]] = 'O';
            }
            return;
        }

        for (int i = 0; i <= 3; i++)
        {
            UI::chessboard[newY[i]][newX[i]] = 'O';
        }
        for (int i = 0; i <= 3; i++)
        {
            X[i] = newX[i];
            Y[i] = newY[i];
        }

        if (doland)
        {
            island();
        }
    }

    void HardDrop()
    {
        while (!island() && game::running)
        {
            move(0, 1, 0);
        }
    }
};

int main()
{
    unsigned int dropInterval = 500;
    console::hideCursor();
    DWORD lastTime = GetTickCount64();
    Dominoes obj;
    obj.landing();

    while (1)
    {
        DWORD now = GetTickCount64();
        if (_kbhit())
        {
            char key = _getch();
            if (key == 0 || key == 0xE0)
            {
                key = _getch();
                continue;
            }

            if (key == 'a')
            {
                obj.move(-1, 0, 1);
            }
            else if (key == 'd')
            {
                obj.move(1, 0, 1);
            }
            else if (key == 's')
            {
                obj.move(0, 1, 1);
            }
            else if (key == 'w')
            {
                obj.HardDrop();
            }
            else if (key == 'q')
            {
                obj.rotate(3);
            }
            else if (key == 'e')
            {
                obj.rotate(1);
            }
            else if (key == ' ')
            {
                if (!game::game_over)
                {
                    game::running = !game::running;
                }
            }
            else if (key == 27)
            {
                break;
            }
            else if (key == 'r')
            {
                for (int i = 0; i <= 23; i++)
                {
                    UI::chessboard[i] = "|..........|";
                }
                game::score = 0;
                game::game_over = 0;
                game::running = 1;
                dropInterval = 500;
                obj.landing();
            }
        }
        if (game::speedcounter >= 10) {
            dropInterval *= 0.8;
            game::speedcounter -= 10;
        }
        UI::Depict();
        if (now - lastTime >= dropInterval)
        {
            obj.move(0, 1, 1);
            lastTime = now;
        }
    }

    console::showCursor();
}
