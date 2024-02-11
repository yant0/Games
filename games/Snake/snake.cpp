#include <iostream>
#include <raylib.h>
#include <raymath.h>
#include <deque>

using namespace std;

int cell_size = 30; 
int cell_count = 25; 
const Color primary = {174, 204, 96, 255};
const Color secondary = {43, 51, 24, 255};
bool game_started = false;
int score = 0;

double lastUpdateTime = 0;

bool EventTriggered(double interval) {
    double currentTime = GetTime();
    if (currentTime - lastUpdateTime >= interval ) {
        lastUpdateTime = currentTime;
        return true;
    }
    return false;
}

bool ElementInDeque(Vector2 vector2, deque<Vector2> deque) {
    for (int i = 0; i < deque.size(); i++) {
        if (Vector2Equals(deque[i], vector2)) {
            return true;
        }
    } return false;
}


class Snake {
public:
    deque<Vector2> body = {Vector2{6,9}, Vector2{5,9}, Vector2{4,9}};
    Vector2 direction {1,0};

    void Draw() {
        for (unsigned i = 0; i < body.size(); i++) {
            int x = body[i].x; 
            int y = body[i].y;
            Rectangle segment = Rectangle{x * cell_size, y * cell_size, cell_size, cell_size};
            DrawRectangleRounded(segment, 0.5, 6,secondary);
        }
    }

    void Update() {
        if (game_started) {
            body.pop_back();
            body.push_front(Vector2Add(body[0], direction));
        }
    }

    void reset() {
        body = {Vector2{6,9}, Vector2{5,9}, Vector2{4,9}};
        direction = {1,0};
        game_started = false;
    }
};

class Food {
public:
    Vector2 position;
    Texture2D texture;

    Food() {
        Image image = LoadImage("graphics/food.png");
        texture = LoadTextureFromImage(image);
        UnloadImage(image);
    }
    
    ~Food() {
        UnloadTexture(texture);
    }

    void Draw() {
        DrawTexture(texture, position.x * cell_size, position.y * cell_size, WHITE);
    }

    Vector2 RandomCell() {
        float x = GetRandomValue(0,cell_count-1);
        float y = GetRandomValue(0,cell_count-1);
        position = {x,y};
        return position;
    }
};

int main() { 

    ChangeDirectory(GetDirectoryPath(GetApplicationDirectory()));
    // const char * dir = GetWorkingDirectory();

    InitWindow(cell_size * cell_count, cell_size * cell_count, "Snake");
    SetTargetFPS(60);

    Food food = Food();
    food.RandomCell();
    Snake snake = Snake();
    
    while (!WindowShouldClose()) {
        BeginDrawing();

        ClearBackground(primary);

        // DrawText(dir,0,0,20,WHITE);
        snake.Draw();
        if (IsKeyPressed(KEY_UP) && snake.direction.y != 1) {snake.direction = {0,-1}; game_started = true;}
        if (IsKeyPressed(KEY_DOWN) && snake.direction.y != -1) {snake.direction = {0,1}; game_started = true;}
        if (IsKeyPressed(KEY_LEFT) && snake.direction.x != 1) {snake.direction = {-1,0}; game_started = true;}
        if (IsKeyPressed(KEY_RIGHT) && snake.direction.x != -1) {snake.direction = {1,0}; game_started = true;}

        if (Vector2Equals(snake.body[0], food.position)) { // Score Up
            food.RandomCell();
            snake.body.push_front(Vector2Add(snake.body[0], snake.direction));
            score++;
        }

        for (int i = 0; i < snake.body.size(); i++) {
            if (Vector2Equals(snake.body[i], food.position)) {
                food.RandomCell();
            }
        }

        deque<Vector2> headlessBody = snake.body;
        headlessBody.pop_front();
        if (ElementInDeque(snake.body[0], headlessBody)) {
            snake.reset();
        }
        if (snake.body[0].x == -1 || snake.body[0].x == cell_count ) {snake.reset();}
        if (snake.body[0].y == -1 || snake.body[0].y == cell_count ) {snake.reset();}

        if (EventTriggered(0.2)) {snake.Update();} // Timing update
        food.Draw();
        DrawText(TextFormat("Score : %i", score),0,cell_count*cell_size-20,20,secondary);

        EndDrawing();
    }
    return 0;
}