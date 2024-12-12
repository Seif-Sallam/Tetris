
#include "raylib.h"

#include <array>
#include <stdio.h>
#include <time.h>

// Custom logging function
void CustomLog(int msgType, const char *text, va_list args)
{
    char timeStr[64] = { 0 };
    time_t now = time(NULL);
    struct tm *tm_info = localtime(&now);

    strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S", tm_info);
    printf("[%s] ", timeStr);

    switch (msgType)
    {
        case LOG_INFO: printf("[INFO] : "); break;
        case LOG_ERROR: printf("[ERROR]: "); break;
        case LOG_WARNING: printf("[WARN] : "); break;
        case LOG_DEBUG: printf("[DEBUG]: "); break;
        default: break;
    }

    vprintf(text, args);
    printf("\n");
}

#define BLOCK_SIZE 24
#define COLUMNS 10
#define ROWS 20
#define BACKGROUND_COLOR { 34, 32, 52, 255 }
#define BORDER_COLOR { 44, 42, 60, 255 }



Texture2D blockTexture = {};

void drawBlock(int x, int y, int color)
{
    DrawTextureRec(blockTexture, { (float)color * BLOCK_SIZE, 0, BLOCK_SIZE, BLOCK_SIZE }, { (float)x * BLOCK_SIZE, (float)y * BLOCK_SIZE }, WHITE);
}

void drawBoard(int board[COLUMNS][ROWS])
{
    for (int x = 0; x < COLUMNS; x++)
        for (int y = 0; y < ROWS; y++)
        {
            if (board[x][y] != 0)
                drawBlock(x, y, board[x][y]);
            else
                DrawRectangleLines(x * BLOCK_SIZE, y * BLOCK_SIZE, BLOCK_SIZE - 1, BLOCK_SIZE - 1, BORDER_COLOR);
        }
}

class Shape {
public:
    Shape(int x = 4, int y = 0)
        : x(x), y(y), rotation(0), color(GetRandomValue(1, 7)), type((Type)GetRandomValue(0, int(Type::COUNT - 1)))
    {
        initBlocks();
    }

    void setPos(int x, int y)
    {
        this->x = x;
        this->y = y;
    }

    int getX() const
    {
        return x;
    }

    int getY() const
    {
        return y;
    }

    int getColor() const
    {
        return color;
    }

    void makeGhost() {
        color = 0; // Transparent
    }

    void draw()
    {
        for (int i = 0; i < 9; i++)
        {
            if (blocks[i] == 1)
            {
                int bx = i % 3;
                int by = i / 3;
                drawBlock(x + bx, y + by, color);
            }
        }
    }

    void move(int dx, const int board[COLUMNS][ROWS])
    {
        if (x + dx >= 0 && x + shapeWidth - 1 + dx < COLUMNS)
        {
            for (int i = 0; i < 9; i++)
            {
                if (blocks[i] == 1)
                {
                    int bx = i % 3;
                    int by = i / 3;
                    if (board[x + bx + dx][y + by] != 0)
                        return;
                }
            }
            x += dx;
        }
    }

    bool isTouchingFloor(const int board[COLUMNS][ROWS]) const
    {
        for (int i = 0; i < 9; i++)
        {
            if (blocks[i] == 1)
            {
                int bx = i % 3;
                int by = i / 3;
                if (y + by + 1 >= ROWS || board[x + bx][y + by + 1] != 0)
                    return true;
            }
        }
        return false;
    }

    void moveDown(const int board[COLUMNS][ROWS])
    {
        if (y + shapeHeight < ROWS)
        {
           if (!isTouchingFloor(board))
                y++;
        }
    }

    void rotate()
    {
        rotation++;
        initBlocks();
    }

    const std::array<uint8_t, 9> getBlocks()
    {
        return blocks;
    }

private:
    void initBlocks()
    {
        switch (type)
        {
        case I:
        {
            if (rotation % 2 == 0)
                blocks = {
                    0, 0, 0,
                    1, 1, 1,
                    0, 0, 0
                };
            else if (rotation % 2 == 1)
                blocks = {
                    1, 0, 0,
                    1, 0, 0,
                    1, 0, 0
                };
        }
        break;
        case J:
        {
            if (rotation % 4 == 0)
                blocks = {
                    0, 0, 0,
                    1, 1, 1,
                    0, 0, 1
                };
            else if (rotation % 4 == 1)
                blocks = {
                    0, 1, 0,
                    0, 1, 0,
                    1, 1, 0
                };
            else if (rotation % 4 == 2)
                blocks = {
                    1, 0, 0,
                    1, 1, 1,
                    0, 0, 0
                };
            else if (rotation % 4 == 3)
                blocks = {
                    1, 1, 0,
                    1, 0, 0,
                    1, 0, 0
                };
        }
        break;
        case L:
        {
            if (rotation % 4 == 0)
                blocks = {
                    0, 0, 0,
                    1, 1, 1,
                    1, 0, 0
                };
            else if (rotation % 4 == 1)
                blocks = {
                    1, 1, 0,
                    0, 1, 0,
                    0, 1, 0
                };
            else if (rotation % 4 == 2)
                blocks = {
                    0, 0, 1,
                    1, 1, 1,
                    0, 0, 0
                };
            else if (rotation % 4 == 3)
                blocks = {
                    1, 0, 0,
                    1, 0, 0,
                    1, 1, 0
                };

        }
        break;
        case O:
        {
            blocks = {
                1, 1, 0,
                1, 1, 0,
                0, 0, 0
            };
        }
        break;
        case S:
        {
            if (rotation % 2 == 0)
                blocks = {
                    0, 1, 1,
                    1, 1, 0,
                    0, 0, 0
                };
            else if (rotation % 2 == 1)
                blocks = {
                    1, 0, 0,
                    1, 1, 0,
                    0, 1, 0
                };
        }
        break;
        case T:
        {
            if (rotation % 4 == 0)
                blocks = {
                    0, 1, 0,
                    1, 1, 1,
                    0, 0, 0
                };
            else if (rotation % 4 == 1)
                blocks = {
                    1, 0, 0,
                    1, 1, 0,
                    1, 0, 0
                };
            else if (rotation % 4 == 2)
                blocks = {
                    0, 0, 0,
                    1, 1, 1,
                    0, 1, 0
                };
            else if (rotation % 4 == 3)
                blocks = {
                    0, 1, 0,
                    1, 1, 0,
                    0, 1, 0
                };
        }
        break;
        case Z:
        {
            if (rotation % 2 == 0)
                blocks = {
                    1, 1, 0,
                    0, 1, 1,
                    0, 0, 0
                };
            else if (rotation % 2 == 1)
                blocks = {
                    0, 1, 0,
                    1, 1, 0,
                    1, 0, 0
                };
        }
        break;
        default:
            break;
        }

        shapeHeight = 2;
        for (int i = 6; i < 9; ++i)
        {
            if (blocks[i] == 1)
            {
                shapeHeight = 3;
                break;
            }
        }

        shapeWidth = 2;
        for (int i = 2; i < 9; i += 3)
        {
            if (blocks[i] == 1)
            {
                shapeWidth = 3;
                break;
            }
        }
    }

private:
    int x, y;
    int rotation;
    int color;
    int shapeHeight = 2;
    int shapeWidth = 2;
    // 3x3 grid of blocks
    std::array<uint8_t, 9> blocks;
    enum Type {
        I = 0,
        J,
        L,
        O,
        S,
        T,
        Z,
        COUNT
    } type;
};

int cleanFullRows(int board[COLUMNS][ROWS])
{
    int cleanedRows = 0;

    for (int y = 0; y < ROWS; y++)
    {
        bool full = true;
        for (int x = 0; x < COLUMNS; x++)
        {
            if (board[x][y] == 0)
            {
                full = false;
                break;
            }
        }

        if (full)
        {
            for (int y2 = y; y2 > 0; y2--)
            {
                for (int x = 0; x < COLUMNS; x++)
                {
                    board[x][y2] = board[x][y2 - 1];
                }
            }

            cleanedRows += 1;
        }
    }
    return cleanedRows;
}

int level = 1;
int linesCleared = 0;
float speed = 1.0f;

void updateLevelAndSpeed() {
    level = linesCleared / 10 + 1;
    speed = 1.0f - (level - 1) * 0.1f;
    if (speed < 0.1f) speed = 0.1f;
}

int main(void)
{
    const int screenWidth = BLOCK_SIZE * (COLUMNS + 9);
    const int screenHeight = BLOCK_SIZE * (ROWS + 2);

    // Set custom logger
#ifdef DEBUG
    SetTraceLogCallback(CustomLog);
    SetTraceLogLevel(LOG_ALL);
#else
    SetTraceLogLevel(LOG_NONE);
#endif


    InitWindow(screenWidth, screenHeight, "Tetris");

    SetTargetFPS(20);

    int score = 0;


    int board[COLUMNS][ROWS] = { 0 };

    Shape playerShape(4, 0);

    float time = 0.0f;
    bool canMove = true;

    bool updateOnTouch = false;
    bool paused = false;
    bool gameOver = false;

    char scoreString[128] = { 0 };

    Shape nextShape(13, 3);
    Shape ghostShape(4, 0);
    ghostShape.makeGhost();
    bool redrawGhost = true;

    bool touchedTheFloor = false;

    float fallInterval = 1.0f;

    // Define the camera with the desired offset
    Camera2D camera = { 0 };
    camera.target = { -24.0f, -24.0f };
    camera.offset = { 0.0f, 0.0f };
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;

    blockTexture =  LoadTexture(RESOURCES_DIR"/Blocks/spritesheet.png");

    Texture2D backgroundTexture = LoadTexture(RESOURCES_DIR"/background.png");

    while (!WindowShouldClose())
    {
        if (!paused && !gameOver)
            time += GetFrameTime();

        if (time > fallInterval * speed && !paused && !gameOver)
        {
            playerShape.moveDown(board);
            time = 0.0f;
            touchedTheFloor = playerShape.isTouchingFloor(board);
            updateOnTouch = touchedTheFloor;
            canMove = !touchedTheFloor;
        }

        if (updateOnTouch)
        {
            updateOnTouch = false;
            for (int i = 0; i < 9; i++)
            {
                if (playerShape.getBlocks()[i] == 1)
                {
                    int bx = i % 3;
                    int by = i / 3;

                    board[playerShape.getX() + bx][playerShape.getY() + by] = playerShape.getColor();
                }
            }

            // Clean full rows
            int fullRows = cleanFullRows(board);
            linesCleared += fullRows;

            updateLevelAndSpeed();

            score += fullRows * 100 + (fullRows > 1 ? (fullRows - 1) * 50 : 0);

            nextShape.setPos(4, 0);
            playerShape = nextShape;
            nextShape = Shape(13, 3);
            if (playerShape.isTouchingFloor(board))
                gameOver = true;
            redrawGhost = true;
        }

        // Process Input
        if (canMove && !paused && !gameOver)
        {
            if (IsKeyPressed(KEY_SPACE))
            {
                while (!playerShape.isTouchingFloor(board))
                    playerShape.moveDown(board);
                updateOnTouch = true;
            }
            if (IsKeyPressed(KEY_LEFT)) {
                playerShape.move(-1, board);
                redrawGhost = true;
            }

            if (IsKeyPressed(KEY_RIGHT)) {
                playerShape.move(1, board);
                redrawGhost = true;
            }

            if (IsKeyPressed(KEY_UP)) {
                playerShape.rotate();
                redrawGhost = true;
            }

            if (IsKeyPressed(KEY_DOWN))
            {
                playerShape.moveDown(board);
                canMove = playerShape.isTouchingFloor(board) == false;
                updateOnTouch = !canMove;
            }
        }

        if (IsKeyPressed(KEY_P))
            paused = !paused;

        if (IsKeyPressed(KEY_R))
        {
            for (int x = 0; x < COLUMNS; x++)
                for (int y = 0; y < ROWS; y++)
                    board[x][y] = 0;
            nextShape.setPos(4, 0);
            playerShape = nextShape;
            nextShape = Shape(13, 3);
            paused = false;
            gameOver = false;
            redrawGhost = true;

            level = 1;
            linesCleared = 0;
            score = 0;
            speed = 1.0f;
            fallInterval = 1.0f;

        }

        BeginDrawing();

        ClearBackground(BACKGROUND_COLOR);

        BeginMode2D(camera);

        drawBoard(board);

        if (redrawGhost)
        {
            ghostShape = playerShape;
            while (!ghostShape.isTouchingFloor(board))
                ghostShape.moveDown(board);

            ghostShape.makeGhost();
            redrawGhost = false;
        }

        ghostShape.draw();

        playerShape.draw();

        DrawTextureRec(backgroundTexture, { 100, 100, 7 * BLOCK_SIZE, screenHeight }, { 11 * BLOCK_SIZE, -24 }, WHITE);

        // Draw Grid and Border over the nextShape
        for (int x = -1; x <= 3; x++)
        {
            for (int y = -1; y <= 3; y++)
            {
                drawBlock(nextShape.getX() + x, nextShape.getY() + y, 9);
            }
        }

        DrawRectangle(nextShape.getX() * BLOCK_SIZE, nextShape.getY() * BLOCK_SIZE, BLOCK_SIZE * 3, BLOCK_SIZE * 3, BACKGROUND_COLOR);
        for (int x = 0; x < 3; x++)
            for (int y = 0; y < 3; y++){
                DrawRectangleLines(nextShape.getX() * BLOCK_SIZE + x * BLOCK_SIZE, nextShape.getY() * BLOCK_SIZE + y * BLOCK_SIZE, BLOCK_SIZE - 1, BLOCK_SIZE - 1, BORDER_COLOR);
            }

        nextShape.draw();

        DrawText("Next Shape", 12 * BLOCK_SIZE + 5, 1 * BLOCK_SIZE, 16, RAYWHITE);

        // Draw the level and lines cleared
        sprintf(scoreString, "Level: %d\n\nLines Cleared: %d", level, linesCleared);
        DrawText(scoreString, 12 * BLOCK_SIZE + 5, 10 * BLOCK_SIZE, 16, RAYWHITE);



        // Draw A border around the board of the texture with the block texture at index 9
        for (int y = -1; y <= ROWS; y++)
        {
            if (y == -1 || y == ROWS)
            {
                for (int x = -1; x <= COLUMNS; ++x)
                    drawBlock(x, y, 9);
            }
            else
            {
                drawBlock(-1, y, 9);
                drawBlock(COLUMNS, y, 9);
            }
        }

        if (paused)
            DrawText("PAUSED", 15, 25, 20, WHITE);

        if (gameOver)
            DrawText("      GAME OVER\n\n\nPress 'R' to restart", screenWidth / 2 - MeasureText("GAME OVER", 40) / 2 - 70, screenHeight / 2 + 100, 30, WHITE);

        EndMode2D();
        EndDrawing();
    }

    CloseWindow();

    return 0;
}