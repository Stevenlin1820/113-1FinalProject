#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#define SIZE 9

int board[SIZE][SIZE];      // �C���ѽL
int solution[SIZE][SIZE];   // �O�s����ѵ����ѽL

// �ˬd�Ʀr�O�_���J�S�w��m
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

// �ϥΦ^���k��R����ѽL
bool fillBoard(int row, int col) {
    if (row == SIZE) return true;
    if (col == SIZE) return fillBoard(row + 1, 0);
    if (board[row][col] != 0) return fillBoard(row, col + 1);

    for (int num = 1; num <= SIZE; num++) {
        if (isValid(row, col, num)) {
            board[row][col] = num;
            if (fillBoard(row, col + 1)) return true;
            board[row][col] = 0;  // �^��
        }
    }

    return false;
}

// �ƻs�ѽL
void copyBoard(int dest[SIZE][SIZE], int src[SIZE][SIZE]) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            dest[i][j] = src[i][j];
        }
    }
}

// �����Ʀr�H�ЫجD�ԴѽL
void removeNumbers(int difficulty) {
    int blanks = difficulty * 10;  // �ھ����ײ����Ʀr
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

// ��ܴѽL
void printBoard() {
    printf("\n�ƿW�ѽL:\n");
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
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

// �ˬd�ѽL�O�_����
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

// �H���ͦ��ƿW�C��
void generateSudoku(int difficulty) {
    // ��l�ƴѽL
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            board[i][j] = 0;
        }
    }

    fillBoard(0, 0);       // ��R����ѽL
    copyBoard(solution, board);  // �O�s����ѵ�
    removeNumbers(difficulty);   // �����Ʀr�Φ��D�ԴѽL
}

int main() {
    int difficulty;
    int row, col, num;

    srand(time(NULL));  // �]�m�H���ƺؤl

    printf("�w��Ө�ƿW�C���I\n");
    printf("������� (1: ²��, 2: ����, 3: �x��): ");
    scanf("%d", &difficulty);

    generateSudoku(difficulty);
    printBoard();

    while (!isSolved()) {
        printf("\n��J�n��J����m�M�Ʀr (�� �C �Ʀr�A��J -1 �����C��): ");
        scanf("%d %d %d", &row, &col, &num);

        if (row == -1 || col == -1 || num == -1) {
            printf("\n�C�������A�P�¹C���I\n");
            return 0;
        }

        if (row < 0 || row >= SIZE || col < 0 || col >= SIZE || num < 1 || num > 9) {
            printf("��J�L�ġA�ЦA�դ@���C\n");
            continue;
        }

        if (board[row][col] != 0) {
            printf("�Ӧ�m�w�g���Ʀr�F�A�п�ܨ�L��m�C\n");
            continue;
        }

        if (isValid(row, col, num)) {
            board[row][col] = num;
        }
        else {
            printf("���Ʀr�L�k��J�Ӧ�m�A�ЦA�դ@���C\n");
        }

        printBoard();
    }

    printf("\n���ߡI�A�����F�ƿW�C���I\n");
    return 0;
}
