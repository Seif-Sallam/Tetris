
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

#define BLOCK_SIZE 30
#define COLUMNS 10
#define ROWS 20


Color intToColor(int value)
{
    switch (value)
    {
    case 1:
        return MAROON;
    case 2:
        return BLUE;
    case 3:
        return DARKGREEN;
    case 4:
        return GOLD;
    case 5:
        return ORANGE;
    case 6:
        return VIOLET;
    case 7:
        return DARKBROWN;
    default:
        return SKYBLUE;
    }
}

void drawBlock(int x, int y, Color color)
{
    DrawRectangle(x * BLOCK_SIZE, y * BLOCK_SIZE, BLOCK_SIZE - 1, BLOCK_SIZE - 1, color);
}

void drawBoard(int board[COLUMNS][ROWS])
{
    for (int x = 0; x < COLUMNS; x++)
        for (int y = 0; y < ROWS; y++)
            drawBlock(x, y, intToColor(board[x][y]));
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

    void draw()
    {
        for (int i = 0; i < 9; i++)
        {
            if (blocks[i] == 1)
            {
                int bx = i % 3;
                int by = i / 3;
                drawBlock(x + bx, y + by, intToColor(color));
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


int main(void)
{
    const int screenWidth = BLOCK_SIZE * (COLUMNS + 5);
    const int screenHeight = BLOCK_SIZE * ROWS;

    // Set custom logger
    SetTraceLogCallback(CustomLog);
    SetTraceLogLevel(LOG_ALL);

    InitWindow(screenWidth, screenHeight, "Tetris");

    SetTargetFPS(60);

    int score = 0;


    int board[COLUMNS][ROWS] = { 0 };

    Shape playerShape(4, 0);

    float time = 0.0f;
    bool canMove = true;

    bool updateOnTouch = false;
    bool paused = false;
    bool gameOver = false;

    char scoreString[128] = { 0 };

    Shape nextShape(11, 3);

    while (!WindowShouldClose())
    {
        if (!paused && !gameOver)
            time += GetFrameTime();

        if (time > 0.5f && !paused && !gameOver)
        {
            playerShape.moveDown(board);
            time = 0.0f;
            canMove = playerShape.isTouchingFloor(board) == false;
            updateOnTouch = !canMove;
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

            score += fullRows * 100 + (fullRows > 1 ? (fullRows - 1) * 50 : 0);

            nextShape.setPos(4, 0);
            playerShape = nextShape;
            nextShape = Shape(11, 3);
            if (playerShape.isTouchingFloor(board))
                gameOver = true;
            canMove = true;
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
            if (IsKeyPressed(KEY_LEFT))
                playerShape.move(-1, board);

            if (IsKeyPressed(KEY_RIGHT))
                playerShape.move(1, board);

            if (IsKeyPressed(KEY_UP))
                playerShape.rotate();

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
            nextShape = Shape(11, 3);
            paused = false;
            gameOver = false;
        }

        BeginDrawing();

        ClearBackground(DARKBLUE);

        drawBoard(board);
        playerShape.draw();

        DrawRectangle(10 * BLOCK_SIZE, 0, 5 * BLOCK_SIZE, screenHeight, GRAY);

        sprintf(scoreString, "Score: %d", score);
        DrawText(scoreString, 10 * BLOCK_SIZE + 5, 7 * BLOCK_SIZE, 25, WHITE);

        if (paused)
            DrawText("PAUSED", screenWidth / 2 - MeasureText("PAUSED", 40) / 2, screenHeight / 2 - 40, 40, WHITE);

        if (gameOver)
            DrawText("GAME OVER\n\n\nPress 'r' To\n\n\nRestart", screenWidth / 2 - MeasureText("GAME OVER", 40) / 2, screenHeight / 2 - 40, 40, WHITE);

        // Draw Grid over the nextShape
        for (int x = 0; x < 3; x++)
            for (int y = 0; y < 3; y++)
                DrawRectangle(nextShape.getX() * BLOCK_SIZE + x * BLOCK_SIZE, nextShape.getY() * BLOCK_SIZE + y * BLOCK_SIZE, BLOCK_SIZE - 1, BLOCK_SIZE - 1, LIGHTGRAY);

        nextShape.draw();

        DrawText("Next Shape", 10 * BLOCK_SIZE + 5, 1 * BLOCK_SIZE, 25, WHITE);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}