#include <gtk/gtk.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* 數獨遊戲題目與解答 */
static int puzzle[9][9];
static int solution[9][9] = {
    {5, 3, 4, 6, 7, 8, 9, 1, 2},
    {6, 7, 2, 1, 9, 5, 3, 4, 8},
    {1, 9, 8, 3, 4, 2, 5, 6, 7},
    {8, 5, 9, 7, 6, 1, 4, 2, 3},
    {4, 2, 6, 8, 5, 3, 7, 9, 1},
    {7, 1, 3, 9, 2, 4, 8, 5, 6},
    {9, 6, 1, 5, 3, 7, 2, 8, 4},
    {2, 8, 7, 4, 1, 9, 6, 3, 5},
    {3, 4, 5, 2, 8, 6, 1, 7, 9}
};

/* 存儲 GtkEntry 指標 */
static GtkWidget* entries[9][9] = { NULL };

/* 狀態標籤與計時器 */
static GtkWidget* status_label = NULL;
static GtkWidget* timer_label = NULL;
static int elapsed_time = 0; // 已用時間（秒）

/* 遊戲難度 */
static int difficulty = 40;

/* 計時器更新函數 */
static gboolean update_timer(gpointer user_data) {
    elapsed_time++;
    char buf[50];
    snprintf(buf, sizeof(buf), "Elapsed Time: %02d:%02d", elapsed_time / 60, elapsed_time % 60);
    gtk_label_set_text(GTK_LABEL(timer_label), buf);
    return TRUE;
}

/* 設置格子文字顏色 */
static void set_entry_color(GtkWidget* entry, GdkRGBA color) {
    PangoAttrList* attr_list = pango_attr_list_new();
    PangoAttribute* fg_color_attr = pango_attr_foreground_new(
        (guint16)(color.red * 65535),
        (guint16)(color.green * 65535),
        (guint16)(color.blue * 65535)
    );
    fg_color_attr->start_index = 0;
    fg_color_attr->end_index = -1;
    pango_attr_list_insert(attr_list, fg_color_attr);

    gtk_entry_set_attributes(GTK_ENTRY(entry), attr_list);
    pango_attr_list_unref(attr_list);
}

/* 提供提示功能 */
static void on_hint_button_clicked(GtkWidget* button, gpointer user_data) {
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (puzzle[i][j] == 0) {
                const gchar* text = gtk_editable_get_text(GTK_EDITABLE(entries[i][j]));
                if (text == NULL || strlen(text) == 0) {
                    char buf[2];
                    snprintf(buf, sizeof(buf), "%d", solution[i][j]);
                    gtk_editable_set_text(GTK_EDITABLE(entries[i][j]), buf);

                    GdkRGBA blue = { 0.0, 0.0, 1.0, 1.0 };
                    set_entry_color(entries[i][j], blue);

                    gtk_label_set_text(GTK_LABEL(status_label), "Hint provided!");
                    return;
                }
            }
        }
    }
    gtk_label_set_text(GTK_LABEL(status_label), "No hints available!");
}

/* 保存遊戲進度 */
static void save_game(GtkWidget* button, gpointer user_data) {
    FILE* file = fopen("sudoku_save.txt", "w");
    if (file == NULL) {
        gtk_label_set_text(GTK_LABEL(status_label), "Failed to save game!");
        return;
    }

    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            fprintf(file, "%d ", puzzle[i][j]);
        }
        fprintf(file, "\n");
    }
    fprintf(file, "---\n");
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            const gchar* text = gtk_editable_get_text(GTK_EDITABLE(entries[i][j]));
            if (text == NULL || strlen(text) == 0) {
                fprintf(file, "0 ");
            }
            else {
                fprintf(file, "%s ", text);
            }
        }
        fprintf(file, "\n");
    }

    fclose(file);
    gtk_label_set_text(GTK_LABEL(status_label), "Game saved successfully!");
}

/* 加載遊戲進度 */
static void load_game(GtkWidget* button, gpointer user_data) {
    FILE* file = fopen("sudoku_save.txt", "r");
    if (file == NULL) {
        gtk_label_set_text(GTK_LABEL(status_label), "No saved game found!");
        return;
    }

    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            fscanf(file, "%d", &puzzle[i][j]);
        }
    }

    char buf[10];
    fscanf(file, "%s", buf);

    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            int value;
            fscanf(file, "%d", &value);
            if (value == 0) {
                gtk_editable_set_text(GTK_EDITABLE(entries[i][j]), "");
                gtk_editable_set_editable(GTK_EDITABLE(entries[i][j]), TRUE);
            }
            else {
                char text[2];
                snprintf(text, sizeof(text), "%d", value);
                gtk_editable_set_text(GTK_EDITABLE(entries[i][j]), text);
                gtk_editable_set_editable(GTK_EDITABLE(entries[i][j]), puzzle[i][j] == 0);
            }
        }
    }

    fclose(file);
    gtk_label_set_text(GTK_LABEL(status_label), "Game loaded successfully!");
}

/* 檢查玩家答案是否正確 */
static void on_check_button_clicked(GtkWidget* button, gpointer user_data) {
    gboolean all_correct = TRUE;
    GdkRGBA green = { 0.0, 1.0, 0.0, 1.0 };
    GdkRGBA red = { 1.0, 0.0, 0.0, 1.0 };

    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (puzzle[i][j] == 0) {
                const gchar* text = gtk_editable_get_text(GTK_EDITABLE(entries[i][j]));
                if (text == NULL || strlen(text) == 0 || atoi(text) != solution[i][j]) {
                    set_entry_color(entries[i][j], red);
                    all_correct = FALSE;
                }
                else {
                    set_entry_color(entries[i][j], green);
                }
            }
        }
    }

    if (all_correct) {
        gtk_label_set_text(GTK_LABEL(status_label), "Congratulations! Puzzle solved!");
    }
    else {
        gtk_label_set_text(GTK_LABEL(status_label), "There are some errors. Keep trying!");
    }
}

/* 生成數獨題目 */
static void generate_puzzle(int blanks) {
    memcpy(puzzle, solution, sizeof(solution));
    srand(time(NULL));
    while (blanks > 0) {
        int row = rand() % 9;
        int col = rand() % 9;
        if (puzzle[row][col] != 0) {
            puzzle[row][col] = 0;
            blanks--;
        }
    }
}

/* 更新棋盤 */
static void update_grid() {
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (entries[i][j] == NULL) continue;

            if (puzzle[i][j] != 0) {
                char buf[2];
                snprintf(buf, sizeof(buf), "%d", puzzle[i][j]);
                gtk_editable_set_text(GTK_EDITABLE(entries[i][j]), buf);
                gtk_editable_set_editable(GTK_EDITABLE(entries[i][j]), FALSE);
            }
            else {
                gtk_editable_set_text(GTK_EDITABLE(entries[i][j]), "");
                gtk_editable_set_editable(GTK_EDITABLE(entries[i][j]), TRUE);
            }
        }
    }
}

/* 開始新遊戲 */
static void start_new_game() {
    elapsed_time = 0; // 重置計時器
    generate_puzzle(difficulty);
    update_grid();
    gtk_label_set_text(GTK_LABEL(status_label), "New game started!");
}

/* 設置遊戲難度 */
static void set_difficulty(GtkWidget* button, gpointer level) {
    difficulty = GPOINTER_TO_INT(level);
    start_new_game();
}

/* 初始化 GUI */
static void activate(GtkApplication* app, gpointer user_data) {
    GtkWidget* window;
    GtkWidget* grid;
    GtkWidget* vbox;
    GtkWidget* check_button, * hint_button, * save_button, * load_button;
    GtkWidget* easy_button, * medium_button, * hard_button;

    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Sudoku");
    gtk_window_set_default_size(GTK_WINDOW(window), 400, 500);

    vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_window_set_child(GTK_WINDOW(window), vbox);

    grid = gtk_grid_new();
    gtk_grid_set_row_spacing(GTK_GRID(grid), 2);
    gtk_grid_set_column_spacing(GTK_GRID(grid), 2);
    gtk_box_append(GTK_BOX(vbox), grid);

    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            GtkWidget* entry = gtk_entry_new();
            entries[i][j] = entry;
            gtk_entry_set_max_length(GTK_ENTRY(entry), 1);
            gtk_entry_set_alignment(GTK_ENTRY(entry), 0.5f);
            gtk_grid_attach(GTK_GRID(grid), entry, j, i, 1, 1);
        }
    }

    timer_label = gtk_label_new("Elapsed Time: 00:00");
    gtk_box_append(GTK_BOX(vbox), timer_label);

    easy_button = gtk_button_new_with_label("Easy");
    g_signal_connect(easy_button, "clicked", G_CALLBACK(set_difficulty), GINT_TO_POINTER(30));
    gtk_box_append(GTK_BOX(vbox), easy_button);

    medium_button = gtk_button_new_with_label("Medium");
    g_signal_connect(medium_button, "clicked", G_CALLBACK(set_difficulty), GINT_TO_POINTER(40));
    gtk_box_append(GTK_BOX(vbox), medium_button);

    hard_button = gtk_button_new_with_label("Hard");
    g_signal_connect(hard_button, "clicked", G_CALLBACK(set_difficulty), GINT_TO_POINTER(50));
    gtk_box_append(GTK_BOX(vbox), hard_button);

    save_button = gtk_button_new_with_label("Save Game");
    g_signal_connect(save_button, "clicked", G_CALLBACK(save_game), NULL);
    gtk_box_append(GTK_BOX(vbox), save_button);

    load_button = gtk_button_new_with_label("Load Game");
    g_signal_connect(load_button, "clicked", G_CALLBACK(load_game), NULL);
    gtk_box_append(GTK_BOX(vbox), load_button);

    hint_button = gtk_button_new_with_label("Hint");
    g_signal_connect(hint_button, "clicked", G_CALLBACK(on_hint_button_clicked), NULL);
    gtk_box_append(GTK_BOX(vbox), hint_button);

    check_button = gtk_button_new_with_label("Check");
    g_signal_connect(check_button, "clicked", G_CALLBACK(on_check_button_clicked), NULL);
    gtk_box_append(GTK_BOX(vbox), check_button);

    status_label = gtk_label_new("");
    gtk_box_append(GTK_BOX(vbox), status_label);

    start_new_game();
    g_timeout_add_seconds(1, update_timer, NULL);

    gtk_window_present(GTK_WINDOW(window));
}

/* 主程式入口 */
int main(int argc, char** argv) {
    GtkApplication* app;
    int status;

    app = gtk_application_new("org.gtk.sudoku", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);

    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);

    return status;
}
