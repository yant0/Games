#include <raylib.h>

class Player {
public:
    char mark;
};

class Box {
public:
    float x_start, y_start, widht, height;
    bool marked = false;
    char mark;

    void CreateBox(Color color) {
        DrawRectangle(x_start, y_start, widht, height, color);
    }
    
    Rectangle TouchArea() {
        return Rectangle {x_start, y_start, widht, height};
    }

    void Mark(char player) {
        mark = player;
        marked = true;
    }
};

class Grid {
public:
    void CreateGrid() {
        DrawLine(GetScreenHeight()/3, 50, GetScreenHeight()/3, GetScreenHeight()-50, Color{255, 207, 135, 255});
        DrawLine(2 * GetScreenHeight()/3, 50, 2 * GetScreenHeight()/3, GetScreenHeight()-50, Color{255, 207, 135, 255});
        DrawLine(50, GetScreenHeight()/3, GetScreenHeight()-50, GetScreenHeight()/3, Color{255, 207, 135, 255});
        DrawLine(50, 2 * GetScreenHeight()/3, GetScreenHeight()-50, 2 * GetScreenHeight()/3, Color{255, 207, 135, 255});
    }
    
    bool CheckWin(Box boxes[]) {
        for (int i = 0; i < 3; i++) { // Check Rows
            if (boxes[i * 3].marked && boxes[i * 3].mark == boxes[i * 3 + 1].mark && boxes[i * 3].mark == boxes[i * 3 + 2].mark ){
                return true;
            }
        }
        for (int i = 0; i < 3; i++) { // Check Coloumns
            if (boxes[i + 3].marked && boxes[i].mark == boxes[i+3].mark && boxes[i].mark == boxes[i + 6].mark ){
                return true;
            }
        }
        if (boxes[0].marked && boxes[0].mark == boxes[4].mark && boxes[0].mark == boxes[8].mark) {
            return true;
        }
        if (boxes[2].marked && boxes[2].mark == boxes[4].mark && boxes[2].mark == boxes[6].mark) {
            return true;
        }
        return false;
    }

    void ResetBoard(Box boxes[]) {
        for (int i = 0; i < 9; i++) {
            boxes[i].marked = false;
            boxes[i].mark = ' ';
        }
    }
};

int main() {

    const int screen = 720;
    const int padding = 50;
    int x_score = 0;
    int o_score = 0;
    Grid grid;
    Box boxes[9]; // Array to store boxes
    for (int i = 0; i < 9; i++) {
        boxes[i] = Box(); // Initialize each box
    }
    Player current_player;
    current_player.mark = 'X';
    
    boxes[0].x_start = padding;
    boxes[0].y_start = padding;
    boxes[0].widht = screen/3-padding;
    boxes[0].height = screen/3-padding;

    boxes[1].x_start = screen/3;
    boxes[1].y_start = padding;
    boxes[1].widht = screen/3;
    boxes[1].height = screen/3 - padding;

    boxes[2].x_start = 2*screen/3;
    boxes[2].y_start = padding;
    boxes[2].widht = screen/3 - padding;
    boxes[2].height = screen/3 - padding;

    boxes[3].x_start = padding;
    boxes[3].y_start = screen/3;
    boxes[3].widht = screen/3-padding;
    boxes[3].height = screen/3;

    boxes[4].x_start = screen/3;
    boxes[4].y_start = screen/3;
    boxes[4].widht = screen/3;
    boxes[4].height = screen/3;

    boxes[5].x_start = 2*screen/3;
    boxes[5].y_start = screen/3;
    boxes[5].widht = screen/3-padding;
    boxes[5].height = screen/3;

    boxes[6].x_start = padding;
    boxes[6].y_start = 2*screen/3;
    boxes[6].widht = screen/3-padding;
    boxes[6].height = screen/3-padding;

    boxes[7].x_start = screen/3;
    boxes[7].y_start = 2*screen/3;
    boxes[7].widht = screen/3;
    boxes[7].height = screen/3-padding;

    boxes[8].x_start = 2*screen/3;
    boxes[8].y_start = 2*screen/3;
    boxes[8].widht = screen/3-padding;
    boxes[8].height = screen/3-padding;

    Vector2 touch_position = GetTouchPosition(0);
    SetTargetFPS(60);
    InitWindow(screen,screen,"TIC TAC TOE");
    while (!WindowShouldClose()) {
        BeginDrawing();
        
        touch_position = GetTouchPosition(0);
        ClearBackground(BROWN);
        grid.CreateGrid();
        DrawText(TextFormat("Score X : %i", x_score),10,0,20,WHITE);
        DrawText(TextFormat("Score O : %i", o_score),screen-120,0,20,WHITE);
        DrawText("TIC TAC TOE",screen/2-65,0,20,WHITE);
        DrawText(TextFormat("Current Player : %c", current_player.mark), screen/2-100, screen-20, 20, WHITE);

        for (int i = 0; i < 9; i++) {
            boxes[i].CreateBox(Color{0, 0, 0, 0});
            if (boxes[i].mark == 'X') {
                boxes[i].CreateBox(BLUE);
            }
            if (boxes[i].mark == 'O') {
                boxes[i].CreateBox(RED);
            }
            
        }
        for (int i = 0; i < 9; i++) {
            if (CheckCollisionPointRec(touch_position, boxes[i].TouchArea()) && GetGestureDetected() == GESTURE_TAP && !boxes[i].marked) {
                boxes[i].Mark(current_player.mark);
                current_player.mark = (current_player.mark == 'X') ? 'O' : 'X';
            }
        }
        if (grid.CheckWin(boxes)) {
            grid.ResetBoard(boxes);
            current_player.mark == 'X' ? o_score++ : x_score++;
        }
        
        EndDrawing();
    }
}