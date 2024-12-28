#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#define SIZE 9

int board[SIZE][SIZE];      // 遊戲棋盤
int solution[SIZE][SIZE];   // 保存完整解答的棋盤

// ======== 功能函數宣告 ========
void sudokuGame();           // 數獨遊戲功能
void anotherFeature();       // 另一個功能範例
void printMenu();            // 顯示選單

// ======== 數獨遊戲相關函數 ========
bool isValid(int row, int col, int num);
bool fillBoard(int row, int col);
void copyBoard(int dest[SIZE][SIZE], int src[SIZE][SIZE]);
void removeNumbers(int difficulty);
void printBoard();
bool isSolved();
void generateSudoku(int difficulty);

// 主函數
int main() {
    int choice;

    while (1) {
        printMenu();  // 顯示選單
        printf("請選擇功能 (輸入 0 結束程式): ");
        scanf("%d", &choice);

        switch (choice) {
        case 0:
            printf("\n程式結束，感謝使用！\n");
            return 0;
        case 1://數獨遊戲
            sudokuGame();
            break;
        case 2:
            anotherFeature();
            break;
        default:
            printf("無效選擇，請重新輸入。\n");
            break;
        }
    }

    return 0;
}

// ======== 數獨遊戲功能 ========
void sudokuGame() {
    int difficulty;
    int row, col, num;

    printf("\n數獨遊戲開始！\n");
    printf("選擇難度 (1: 簡單, 2: 中等, 3: 困難): ");
    scanf("%d", &difficulty);

    generateSudoku(difficulty);
    printBoard();

    while (!isSolved()) {
        printf("\n輸入要填入的位置和數字 (行 列 數字，輸入 -1 結束遊戲): ");
        scanf("%d %d %d", &row, &col, &num);

        if (row == -1 || col == -1 || num == -1) {
            printf("\n遊戲結束，感謝遊玩！\n");
            return;
        }

        if (row < 0 || row >= SIZE || col < 0 || col >= SIZE || num < 1 || num > 9) {
            printf("輸入無效，請再試一次。\n");
            continue;
        }

        if (board[row][col] != 0) {
            printf("該位置已經有數字了，請選擇其他位置。\n");
            continue;
        }

        if (isValid(row, col, num)) {
            board[row][col] = num;
        }
        else {
            printf("此數字無法填入該位置，請再試一次。\n");
        }

        printBoard();
    }

    printf("\n恭喜！你完成了數獨遊戲！\n");
}

// ======== 示例的另一個功能 ========
void anotherFeature() {
    printf("\n這是一個示例功能，尚未實現。\n");
}

// ======== 顯示選單 ========
void printMenu() {
    printf("\n========== 功能選單 ==========\n");
    printf("1. 數獨遊戲\n");
    printf("2. 其他功能 (示例)\n");
    printf("================================\n");
}

// ======== 數獨相關函數實作 ========
// 檢查數字是否能放入特定位置
bool isValid(int row, int col, int num) {
    for (int i = 0; i < SIZE; i++) {
        if (board[row][i] == num || board[i][col] == num) {
            return false;
        }
    }

    int startRow = (row / 3) * 3;
    int startCol = (col / 3) * 3;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (board[startRow + i][startCol + j] == num) {
                return false;
            }
        }
    }

    return true;
}

// 使用回溯法填充完整棋盤
bool fillBoard(int row, int col) {
    if (row == SIZE) return true;
    if (col == SIZE) return fillBoard(row + 1, 0);
    if (board[row][col] != 0) return fillBoard(row, col + 1);

    for (int num = 1; num <= SIZE; num++) {
        if (isValid(row, col, num)) {
            board[row][col] = num;
            if (fillBoard(row, col + 1)) return true;
            board[row][col] = 0;  // 回溯
        }
    }

    return false;
}

// 複製棋盤
void copyBoard(int dest[SIZE][SIZE], int src[SIZE][SIZE]) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            dest[i][j] = src[i][j];
        }
    }
}

// 移除數字以創建挑戰棋盤
void removeNumbers(int difficulty) {
    int blanks = difficulty * 10;  // 根據難度移除數字
    for (int i = 0; i < blanks; i++) {
        int row = rand() % SIZE;
        int col = rand() % SIZE;
        while (board[row][col] == 0) {
            row = rand() % SIZE;
            col = rand() % SIZE;
        }
        board[row][col] = 0;
    }
}

// 顯示棋盤
void printBoard() {
    printf("\n數獨棋盤:\n");
    for (int i = 0; i < SIZE; i++) {
        if (i % 3 == 0 && i != 0) {
            printf("---------------------\n");
        }
        for (int j = 0; j < SIZE; j++) {
            if (j % 3 == 0 && j != 0) {
                printf("| ");
            }
            if (board[i][j] == 0) {
                printf(". ");
            }
            else {
                printf("%d ", board[i][j]);
            }
        }
        printf("\n");
    }
}

// 檢查棋盤是否完成
bool isSolved() {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (board[i][j] == 0) {
                return false;
            }
        }
    }
    return true;
}

// 隨機生成數獨遊戲
void generateSudoku(int difficulty) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            board[i][j] = 0;
        }
    }

    fillBoard(0, 0);       // 填充完整棋盤
    copyBoard(solution, board);  // 保存完整解答
    removeNumbers(difficulty);   // 移除數字形成挑戰棋盤
}
