
#include <iostream>
#include <Windows.h>

volatile int* positions = nullptr;
volatile bool* finished = nullptr;
volatile int winner = -1;
volatile bool end = false;
int numTurtles;
int* steps = nullptr;

DWORD WINAPI Move(LPVOID iNum)
{
    int turtlenum = (int)iNum;
    int index = 0;
    while (!end) {
       
        int step = steps[turtlenum];
        int sleep = 1000 + rand() % 2001;

        index += step;
        if (index > 49)
            index = 49;

        positions[turtlenum] = index;

        if (index >= 49 && !finished[turtlenum]) {
            finished[turtlenum] = true;
            if (winner == -1) {
                winner = turtlenum;
                std::cout << "\nЧерепашка " << winner << " победила!" << std::endl;
            }

            bool allFinished = true;
            for (int i = 0; i < numTurtles; i++) {
                if (!finished[i]) {
                    allFinished = false;
                    break;
                }
            }
            if (allFinished)
                end = true;
        }
        Sleep(sleep);
        for (int i = 0; i < numTurtles; i++) {
            steps[i] = 1 + rand() % 3;
        }
    }
    return 0;
}
void DrawGame() {
    char game[50];
    while (!end) { 
        system("cls");
        for (int i = 0; i < 50; i++) {
            game[i] = '-';
        }
        for (int i = 0; i < numTurtles; i++) {
            if (positions[i] < 50) {
                game[positions[i]] = '@';
            }
        }
        std::cout << "|";
        for (int i = 0; i < 50; i++) {
            std::cout << game[i];
        }
        std::cout << "|";
        std::cout << "\nТабло" << std::endl;
        for (int i = 0; i < numTurtles; i++) {
            std::cout << "Черепаха " << i+1 << ": " << positions[i] + 1 << "/50";
            if (finished[i])
                std::cout << " ФИНИШ";
            std::cout << std::endl;
        }

        if (winner != -1)
            std::cout << "\nЧерепашка № " << winner+1 << " победила!" << std::endl;

        Sleep(500);
    }
}

int main()
{
    setlocale(0, "rus");
    srand(time(NULL)+GetCurrentProcessId());
    std::cout << "Введите кол-во участников (до 10): ";
    std::cin >> numTurtles;
    if (numTurtles > 10) {
        std::cout << "Вы ввели слишком большое кол-во участников. Максимум 10";
        return 0;
    }
    positions = new int[numTurtles];
    finished = new bool[numTurtles];
    steps = new int[numTurtles];
    for (int i = 0; i < numTurtles; i++) {
        positions[i] = 0;
        finished[i] = false;

        steps[i] = 1 + rand() % 3;
    }
    HANDLE* hThread = new HANDLE[numTurtles];
    DWORD* IDThread = new DWORD[numTurtles];

    for (int i = 0; i < numTurtles; i++) {
        hThread[i] = CreateThread(NULL, 0, Move, (LPVOID)i, 0, &IDThread[i]);
        if (hThread[i] == NULL)
            return GetLastError();
    }
    DrawGame();
    WaitForMultipleObjects(numTurtles, hThread, TRUE, INFINITE);

    for (int i = 0; i < numTurtles; i++)
        CloseHandle(hThread[i]);
}
