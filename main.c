#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <Windows.h>

#define KIT_IMPL
#include "kit.h"

typedef struct
{
    int x;
    int y;
} vec2;

int score = 0;
char score_text[32];
vec2 segments[256] = {0};
bool gameover = true;

int main()
{
    srand((unsigned)time(NULL));

    int cellsz = 10;
    int rows = 20;
    int cols = 20;
    int screenw = cellsz * cols;
    int screenh = cellsz * rows;

    kit_Context *ctx = kit_create("Snaki", screenw, screenh, KIT_SCALE2X);
    double dt;

    // snake
    vec2 head = {0, 1};
    vec2 dir = {1, 0};

    // berry
    vec2 berry = {rand() % cols, rand() % rows};

    while (kit_step(ctx, &dt))
    {
        if (kit_key_pressed(ctx, VK_ESCAPE))
            break;

        // Draw game over screen
        if (gameover)
        {
            // draw score
            sprintf(score_text, "%d", score);
            int spacetexty = ((cols / 2) * cellsz) - cellsz;
            if (score > 0)
            {
                spacetexty = cols / 2 + (cellsz * 2);
                kit_draw_text(ctx, kit_rgb(180, 180, 180), "score:", ((rows / 2) * cellsz) - (kit_text_width(ctx->font, "score:") / 2), ((cols / 2) * cellsz) - cellsz);
                kit_draw_text(ctx, kit_rgb(180, 180, 180), score_text, ((rows / 2) * cellsz) - (kit_text_width(ctx->font, score_text) / 2), ((cols / 2) * cellsz) + cellsz);
            }
            kit_draw_text(ctx, KIT_WHITE, "Press Space", ((rows / 2) * cellsz) - (kit_text_width(ctx->font, "Press Space") / 2), spacetexty);

            if (kit_key_pressed(ctx, VK_SPACE))
            {
                score = 0;
                berry.x = rand() % cols;
                berry.y = rand() % rows;

                head.x = rand() % cols;
                head.y = rand() % rows;
                gameover = false;
            }
            Sleep(100);
            continue;
        }

        if (kit_key_pressed(ctx, VK_LEFT) || kit_key_pressed(ctx, 0x41)) // LEFT or A
        {
            if (dir.x == 1)
                continue;
            dir.x = -1;
            dir.y = 0;
        }

        if (kit_key_pressed(ctx, VK_RIGHT) || kit_key_pressed(ctx, 0x44)) // RIGHT or D
        {
            if (dir.x == -1)
                continue;
            dir.x = 1;
            dir.y = 0;
        }

        if (kit_key_pressed(ctx, VK_UP) || kit_key_pressed(ctx, 0x57)) // UP or W
        {
            if (dir.y == 1)
                continue;
            dir.x = 0;
            dir.y = -1;
        }

        if (kit_key_pressed(ctx, VK_DOWN) || kit_key_pressed(ctx, 0x53)) // DOWN or S
        {
            if (dir.y == -1)
                continue;
            dir.x = 0;
            dir.y = 1;
        }

        // ===== update =====
        // update segments
        for (int i = score; i > 0; i--)
        {
            // update movement of segments
            segments[i] = segments[i - 1];

            // check if head collides with a segment (game over conditon)
            if (segments[i].x == head.x && segments[i].y == head.y)
            {
                gameover = true;
            }
        }

        segments[0] = head;

        head.x += dir.x;
        head.y += dir.y;

        if (head.x >= cols)
        {
            head.x = 0;
        }
        if (head.y >= rows)
        {
            head.y = 0;
        }
        if (head.x < 0)
        {
            head.x = cols - 1;
        }
        if (head.y < 0)
        {
            head.y = rows - 1;
        }

        if (head.x == berry.x && head.y == berry.y)
        {
            score++;
            berry.x = rand() % cols;
            berry.y = rand() % rows;
        }

        // ===== draw =====
        kit_clear(ctx, kit_rgb(50, 30, 30));

        // draw berry
        int berryx = berry.x * cellsz;
        int berryy = berry.y * cellsz;
        kit_draw_rect(ctx, kit_rgb(180, 0, 80), (kit_Rect){berryx, berryy, cellsz, cellsz});

        // draw snake
        int headx = head.x * cellsz;
        int heady = head.y * cellsz;
        kit_draw_rect(ctx, KIT_WHITE, (kit_Rect){headx, heady, cellsz, cellsz});

        for (int i = 0; i < score; i++)
        {
            int sx = segments[i].x * cellsz;
            int sy = segments[i].y * cellsz;
            kit_draw_rect(ctx, KIT_WHITE, (kit_Rect){sx + 1, sy + 1, cellsz - 2, cellsz - 2});
        }

        // draw score
        sprintf(score_text, "score: %d", score);
        kit_draw_text(ctx, kit_rgb(120, 120, 120), score_text, 5, 5);

        Sleep(180);
    }

    kit_destroy(ctx);
    return 0;
}