#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;

const int ROWS = 9;
const int COLS = 9;
const int MINES = 10;

// Board arrays
int  board[ROWS][COLS];    // actual board: -1 = mine, 0-8 = adjacent mines
bool revealed[ROWS][COLS]; // has cell been revealed
bool flagged[ROWS][COLS];  // has cell been flagged

// ─── Setup ────────────────────────────────────────────────────────────────────

void initBoard() {
    for (int i = 0; i < ROWS; i++)
        for (int j = 0; j < COLS; j++) {
            board[i][j]    = 0;
            revealed[i][j] = false;
            flagged[i][j]  = false;
        }
}

void placeMines() {
    srand(time(0));
    int placed = 0;
    while (placed < MINES) {
        int r = rand() % ROWS;
        int c = rand() % COLS;
        if (board[r][c] != -1) {
            board[r][c] = -1;
            placed++;
        }
    }
}

void calculateNumbers() {
    int dr[] = {-1,-1,-1, 0, 0, 1, 1, 1};
    int dc[] = {-1, 0, 1,-1, 1,-1, 0, 1};

    for (int i = 0; i < ROWS; i++)
        for (int j = 0; j < COLS; j++) {
            if (board[i][j] == -1) continue;
            int count = 0;
            for (int d = 0; d < 8; d++) {
                int ni = i + dr[d];
                int nj = j + dc[d];
                if (ni >= 0 && ni < ROWS && nj >= 0 && nj < COLS)
                    if (board[ni][nj] == -1)
                        count++;
            }
            board[i][j] = count;
        }
}

// ─── Display ──────────────────────────────────────────────────────────────────

void printBoard(bool showAll = false) {
    cout << "\n   ";
    for (int j = 0; j < COLS; j++)
        cout << j + 1 << " ";
    cout << "\n   ";
    for (int j = 0; j < COLS; j++)
        cout << "--";
    cout << "\n";

    for (int i = 0; i < ROWS; i++) {
        cout << i + 1 << " | ";
        for (int j = 0; j < COLS; j++) {
            if (showAll && board[i][j] == -1) {
                cout << "* ";
            } else if (flagged[i][j]) {
                cout << "F ";
            } else if (!revealed[i][j]) {
                cout << ". ";
            } else if (board[i][j] == 0) {
                cout << "  ";
            } else {
                cout << board[i][j] << " ";
            }
        }
        cout << "\n";
    }
    cout << "\n";
}

// ─── Game Logic ───────────────────────────────────────────────────────────────

// Recursive flood fill - reveals empty cells automatically
void reveal(int r, int c) {
    if (r < 0 || r >= ROWS || c < 0 || c >= COLS) return;
    if (revealed[r][c] || flagged[r][c]) return;

    revealed[r][c] = true;

    // If empty cell, reveal all neighbors recursively
    if (board[r][c] == 0) {
        int dr[] = {-1,-1,-1, 0, 0, 1, 1, 1};
        int dc[] = {-1, 0, 1,-1, 1,-1, 0, 1};
        for (int d = 0; d < 8; d++)
            reveal(r + dr[d], c + dc[d]);
    }
}

bool checkWin() {
    for (int i = 0; i < ROWS; i++)
        for (int j = 0; j < COLS; j++)
            if (board[i][j] != -1 && !revealed[i][j])
                return false;
    return true;
}

// ─── Main ─────────────────────────────────────────────────────────────────────

int main() {
    initBoard();
    placeMines();
    calculateNumbers();

    cout << "================================\n";
    cout << "    MINESWEEPER - C++ Console   \n";
    cout << "================================\n";
    cout << "Commands:\n";
    cout << "  r <row> <col>  - reveal a cell\n";
    cout << "  f <row> <col>  - flag/unflag a cell\n";
    cout << "  q              - quit\n";
    cout << "================================\n";

    bool gameOver = false;
    bool won      = false;

    while (!gameOver) {
        printBoard();

        char cmd;
        int  r, c;

        cout << "Enter command: ";
        cin  >> cmd;

        if (cmd == 'q') {
            cout << "Thanks for playing!\n";
            break;
        }

        cin >> r >> c;
        r--; c--;  // convert to 0-indexed

        if (r < 0 || r >= ROWS || c < 0 || c >= COLS) {
            cout << "Invalid position! Row and col must be 1-" << ROWS << "\n";
            continue;
        }

        if (cmd == 'f') {
            if (!revealed[r][c])
                flagged[r][c] = !flagged[r][c];
            else
                cout << "Cannot flag a revealed cell!\n";

        } else if (cmd == 'r') {
            if (flagged[r][c]) {
                cout << "Cell is flagged! Unflag first.\n";
                continue;
            }
            if (revealed[r][c]) {
                cout << "Cell already revealed!\n";
                continue;
            }
            if (board[r][c] == -1) {
                // Hit a mine
                revealed[r][c] = true;
                printBoard(true);
                cout << "BOOM! You hit a mine! Game Over.\n";
                gameOver = true;
            } else {
                reveal(r, c);
                if (checkWin()) {
                    printBoard(true);
                    cout << "Congratulations! You Win!\n";
                    gameOver = true;
                    won = true;
                }
            }
        } else {
            cout << "Unknown command. Use 'r', 'f', or 'q'.\n";
        }
    }

    if (!won && !gameOver)
        cout << "Game ended.\n";

    return 0;
}