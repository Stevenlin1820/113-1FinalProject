#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#define SIZE 9

int board[SIZE][SIZE];      // �C���ѽL
int solution[SIZE][SIZE];   // �O�s����ѵ����ѽL

// ======== �\���ƫŧi ========
void sudokuGame();           // �ƿW�C���\��
void anotherFeature();       // �t�@�ӥ\��d��
void printMenu();            // ��ܿ��

// ======== �ƿW�C��������� ========
bool isValid(int row, int col, int num);
bool fillBoard(int row, int col);
void copyBoard(int dest[SIZE][SIZE], int src[SIZE][SIZE]);
void removeNumbers(int difficulty);
void printBoard();
bool isSolved();
void generateSudoku(int difficulty);

// �D���
int main() {
    int choice;

    while (1) {
        printMenu();  // ��ܿ��
        printf("�п�ܥ\�� (��J 0 �����{��): ");
        scanf("%d", &choice);

        switch (choice) {
        case 0:
            printf("\n�{�������A�P�¨ϥΡI\n");
            return 0;
        case 1://�ƿW�C��
            sudokuGame();
            break;
        case 2:
            anotherFeature();
            break;
        default:
            printf("�L�Ŀ�ܡA�Э��s��J�C\n");
            break;
        }
    }

    return 0;
}

// ======== �ƿW�C���\�� ========
void sudokuGame() {
    int difficulty;
    int row, col, num;

    printf("\n�ƿW�C���}�l�I\n");
    printf("������� (1: ²��, 2: ����, 3: �x��): ");
    scanf("%d", &difficulty);

    generateSudoku(difficulty);
    printBoard();

    while (!isSolved()) {
        printf("\n��J�n��J����m�M�Ʀr (�� �C �Ʀr�A��J -1 �����C��): ");
        scanf("%d %d %d", &row, &col, &num);

        if (row == -1 || col == -1 || num == -1) {
            printf("\n�C�������A�P�¹C���I\n");
            return;
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
}

// ======== �ܨҪ��t�@�ӥ\�� ========
void anotherFeature() {
    printf("\n�o�O�@�ӥܨҥ\��A�|����{�C\n");
}

// ======== ��ܿ�� ========
void printMenu() {
    printf("\n========== �\���� ==========\n");
    printf("1. �ƿW�C��\n");
    printf("2. ��L�\�� (�ܨ�)\n");
    printf("================================\n");
}

// ======== �ƿW������ƹ�@ ========
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
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            board[i][j] = 0;
        }
    }

    fillBoard(0, 0);       // ��R����ѽL
    copyBoard(solution, board);  // �O�s����ѵ�
    removeNumbers(difficulty);   // �����Ʀr�Φ��D�ԴѽL
}
