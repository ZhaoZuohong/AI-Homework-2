#include <cstdlib>
#include <cstring>
#include <iostream>
#include <queue>
#include <set>
#include <vector>

using namespace std;

typedef struct {
    int board[3][3];
    int f; // f(n) = g(n) + h(n)
    int g;
    int h;
    int zeroI;
    int zeroJ;
    int hash;
} unit;

const int target[3][3] = {
    { 8, 7, 6 },
    { 5, 4, 3 },
    { 2, 1, 0 }
};

const int TargetI[9] = { 2, 2, 2, 1, 1, 1, 0, 0, 0 };
const int TargetJ[9] = { 2, 1, 0, 2, 1, 0, 2, 1, 0 };

struct cmp {
    bool operator()(unit a, unit b)
    {
        return a.f > b.f;
    }
};

priority_queue<unit, vector<unit>, cmp> pq;
set<int> visited;

int UnitHash(unit* u)
{
    int* arr = u->board[0];
    int hash = 0;
    for (int i = 0; i < 9; ++i) {
        hash = hash * 9 + arr[i];
    }
    return hash;
}

int Manhattan(unit* u)
{
    int(*arr)[3] = u->board;
    int result = 0;
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            int n = arr[i][j];
            if (n <= 4) {
                continue;
            }
            result += 3 * abs(TargetJ[n] - j);
            result += 3 * abs(TargetI[n] - i);
        }
    }
    return result;
}

void UpdateUnit(unit* u)
{
    u->g += 1;
    u->h = Manhattan(u);
    u->f = u->g + u->h;
    u->hash = UnitHash(u);
}

bool Reached(unit * u)
{
    if (memcmp(u->board, target, sizeof(target)) == 0) {
        return true;
    } else {
        return false;
    }
}

int A_Star(void)
{
    while (!pq.empty()) {
        unit v = pq.top();
        pq.pop();
        if (v.zeroI != 0) {
            unit u = v;
            u.board[u.zeroI][u.zeroJ] = u.board[u.zeroI - 1][u.zeroJ];
            u.zeroI -= 1;
            u.board[u.zeroI][u.zeroJ] = 0;
            UpdateUnit(&u);
            if (Reached(&u)) {
                return u.g;
            }
            if (visited.find(u.hash) == visited.end()) {
                pq.push(u);
                visited.insert(u.hash);
            }
        }
        if (v.zeroJ != 0) {
            unit u = v;
            u.board[u.zeroI][u.zeroJ] = u.board[u.zeroI][u.zeroJ - 1];
            u.zeroJ -= 1;
            u.board[u.zeroI][u.zeroJ] = 0;
            UpdateUnit(&u);
            if (Reached(&u)) {
                return u.g;
            }
            if (visited.find(u.hash) == visited.end()) {
                pq.push(u);
                visited.insert(u.hash);
            }
        }
        if (v.zeroI != 2) {
            unit u = v;
            u.board[u.zeroI][u.zeroJ] = u.board[u.zeroI + 1][u.zeroJ];
            u.zeroI += 1;
            u.board[u.zeroI][u.zeroJ] = 0;
            UpdateUnit(&u);
            if (Reached(&u)) {
                return u.g;
            }
            if (visited.find(u.hash) == visited.end()) {
                pq.push(u);
                visited.insert(u.hash);
            }
        }
        if (v.zeroJ != 2) {
            unit u = v;
            u.board[u.zeroI][u.zeroJ] = u.board[u.zeroI][u.zeroJ + 1];
            u.zeroJ += 1;
            u.board[u.zeroI][u.zeroJ] = 0;
            UpdateUnit(&u);
            if (Reached(&u)) {
                return u.g;
            }
            if (visited.find(u.hash) == visited.end()) {
                pq.push(u);
                visited.insert(u.hash);
            }
        }
    }
    return 0;
}

int main()
{
    unit u;
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            cin >> u.board[i][j];
        }
    }

    u.g = 0;
    u.h = Manhattan(&u);
    u.f = u.h;
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (u.board[i][j] == 0) {
                u.zeroI = i;
                u.zeroJ = j;
            }
        }
    }
    u.hash = UnitHash(&u);

    pq.push(u);
    visited.insert(u.hash);

    cout << A_Star() << endl;

    return 0;
}