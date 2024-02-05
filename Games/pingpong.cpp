#include <iostream>
#include <raylib.h>

int player_score, cpu_score;
bool game_started = false;

class Ball {
public:
    float x,y;
    int speed_x, speed_y;
    int radius;
    
    void Draw() {
        DrawCircle(x,y,radius,WHITE);
    }

    void Update() {
        int speed_choices[2] = {1,-1};
        if (!game_started && (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_DOWN))) {
            speed_x *= speed_choices[GetRandomValue(0,1)];
            speed_y *= speed_choices[GetRandomValue(0,1)];
            game_started = true;
        }
        
        if (game_started == true){
            x += speed_x;
            y += speed_y;
        }

        if (y + radius >= GetScreenHeight() || y - radius <= 0 ) {
            speed_y *= -1;
        }
        if (x + radius >= GetScreenWidth()) {
            player_score++;
            ResetBall();
        }
        if (x - radius <= 0) {
            cpu_score++;
            ResetBall();
        }
    }
    
    void ResetBall() {
        x = GetScreenWidth()/2;
        y = GetScreenHeight()/2;
        game_started = false;
    }
};

class Paddle {
public:
    float x,y;
    float widht, height;
    int speed;

    void Draw() {
        DrawRectangle(x,y,widht,height,WHITE);
    }
    void Update() {
        if (IsKeyDown(KEY_UP)) {
            y = y - speed;
        }
        if (IsKeyDown(KEY_DOWN)) {
            y = y + speed;
        }
        if (y <= 0) {
            y = 0;
        }
        if (y + height >= GetScreenHeight()) {
            y = GetScreenHeight() - height;
        }
    }
};

class CpuPaddle : public Paddle {
protected:
    void LimitMovement() {
        if (y <= 0) {
            y = 0;
        }
        if (y + height >= GetScreenHeight()) {
            y = GetScreenHeight() - height;
        }
    }
    
public:
    void Update(int ball_y) {
        if (y+height/2 > ball_y) {
            y = y - speed;
        }
        if (y+height/2 <= ball_y) {
            y = y + speed;
        }
        LimitMovement();
    }
};

Ball ball;
Paddle player;
CpuPaddle cpu;

int main() {

    const int screen_widht = 1280;
    const int screen_height = 800;
    SetTargetFPS(60);
    InitWindow(screen_widht,screen_height,"PONG");

    ball.radius = 20;
    ball.x = screen_widht/2;
    ball.y = screen_height/2;
    ball.speed_x = 7;
    ball.speed_y = 7;

    cpu.widht = 25;
    cpu.height = 120;
    cpu.x = screen_widht - cpu.widht-10;
    cpu.y = screen_height/2 - cpu.height/2;
    cpu.speed = 6;

    player.widht = 25;
    player.height = 120;
    player.x = 10;
    player.y = screen_height/2 - player.height/2;
    player.speed = 6;

    while (!WindowShouldClose()) {
        BeginDrawing();
        
        player.Update();
        ball.Update();
        cpu.Update(ball.y);

        if (CheckCollisionCircleRec(Vector2 {ball.x,ball.y}, ball.radius, Rectangle {player.x,player.y,player.widht,player.height})) {
            ball.speed_x *= -1;
        }
        if (CheckCollisionCircleRec(Vector2 {ball.x,ball.y}, ball.radius, Rectangle {cpu.x,cpu.y,cpu.widht,cpu.height})) {
            ball.speed_x *= -1;
        }

        ClearBackground(BLACK);
        ball.Draw();
        cpu.Draw();
        player.Draw();
        DrawLine(screen_widht/2,0,screen_widht/2,screen_height,WHITE);
        DrawText(TextFormat("%i", player_score),screen_widht/4 - 20, 20, 80, WHITE);
        DrawText(TextFormat("%i", cpu_score),3 * screen_widht/4 + 20, 20, 80, WHITE);
        
        EndDrawing();
    }
    CloseWindow();
    return 0;
}