#include "raylib.h"
#include "time.h"
#include "math.h"
#define BLOCKWIDTH 32
#define BLOCKHEIGHT 20
#define ROWS 6

struct player{
    float posx;
    float posy;
    int width;
    int height;
    int speed;
};


// ball
struct ball
{
    float posx; 
    float posy; 
    int width;
    int height;
    int velx; 
    int vely;
};

void ballphysics(struct ball *b, struct player *p, float dt) {
    b->posx += b->velx * dt;
    b->posy += b->vely * dt;

    if (CheckCollisionRecs((Rectangle){b->posx, b->posy, b->width, b->height},(Rectangle){p->posx, p->posy, p->width, p->height/2})) {
        if (fabsf(b->velx) > fabsf(b->vely))
            b->velx *= -1;
        else
            b->vely *= -1;
    }
    if (b->posx <= 0 || b->posx + b->width >= GetScreenWidth())
        b->velx *= -1;

    if (b->posy <= 0)
        b->vely *= -1;
    if (b->posy >= GetScreenHeight()){
        DrawText("YOU LOST!", (GetScreenWidth()/2)-150, GetScreenHeight()/2,80,RED);
    }
}
void drawball(struct ball *b){
    DrawRectangle(b->posx, b->posy,b->width, b->height, WHITE);
}

// blocks
typedef struct blocks {
    float posx;
    float posy;
} block;

void drawblocks(block *b, int i){
    if (i%2 == 0){
    DrawRectangle(b->posx,b->posy,BLOCKWIDTH,BLOCKHEIGHT,RED);}
    else {
        DrawRectangle(b->posx,b->posy,BLOCKWIDTH,BLOCKHEIGHT,DARKGRAY);
    }
}

void blockphysics(block *b, struct ball *ba, Sound brick) {
    if (CheckCollisionRecs((Rectangle){b->posx, b->posy, BLOCKWIDTH, BLOCKHEIGHT},(Rectangle){ba->posx, ba->posy, ba->width, ba->height})) {
        PlaySound(brick);
        if (fabsf(ba->velx) > fabsf(ba->vely))
            ba->velx *= -1;
        else
            ba->vely *= -1;

        b->posx = 90000000000;
    }
}


// PLAYER


void playercontrol(struct player *p, float dt, int border){
    if (IsKeyDown(KEY_A)){
            if (p->posx <= 0){
                p->posx = 0;
            }
            else{
            p->posx -= p->speed*dt;}
        }
    else if (IsKeyDown(KEY_D)){
            if (p->posx+p->width >= border){
                p->posx = border-p->width;
            }
            else{
            p->posx += p->speed*dt;}
        }

    }

void drawplayer(struct player *p){
    DrawRectangle(p->posx,p->posy,p->width,p->height,ORANGE);
}



//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    SetRandomSeed(time(NULL));
    const int screenWidth = 800;
    const int screenHeight = 450;
  
    struct player p = {400, 400,60,10,300};
    block blocks[(screenWidth/BLOCKWIDTH)*ROWS];
    int col =0;
    int row = -1;
        for (int i = 0; i < (screenWidth/BLOCKWIDTH)*ROWS; i++){
            if (i%25 == 0){
                col =0;
                row++;
            }
            blocks[i].posx = col*BLOCKWIDTH;
            blocks[i].posy = 50+row*BLOCKHEIGHT;
            col++;

    }
    struct ball ball = {300, 200, 15,15,120*GetRandomValue(-1,1),120};
    InitWindow(screenWidth, screenHeight, "BREAKOUT");
    InitAudioDevice();
    Sound brick = LoadSound("break.mp3");
    SetTargetFPS(60);               
    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        
        //----------------------------------------------------------------------------------
        float dt = GetFrameTime();
        playercontrol(&p,dt, screenWidth);
        ballphysics(&ball,&p, dt);
        for (int i = 0; i < (screenWidth/BLOCKWIDTH)*ROWS; i++){
            blockphysics(&blocks[i], &ball, brick);
        }
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();
        ClearBackground(GRAY);
        drawplayer(&p);
        for (int i = 0; i < (screenWidth/BLOCKWIDTH)*ROWS; i++){
            drawblocks(&blocks[i], i);
        }
        drawball(&ball);
        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        
    //--------------------------------------------------------------------------------------

    return 0;
}