
#include <stdbool.h>
#include <stdio.h>

#include <SDL2/SDL.h>

#include "basic.h"


// const int SCREEN_X = 1000;
// const int SCREEN_Y = 680;
// const int CELL_X = 20;
// const int CELL_Y = 20;
// const int BOARD_X = SCREEN_X / CELL_X;
// const int BOARD_Y = SCREEN_Y / CELL_Y;
// //const int board_size = BOARD_X * BOARD_Y;
// 
// bool board[1000/20][680/20];

#define SCREEN_X 1000
#define SCREEN_Y 680
#define CELL_X   20
#define CELL_Y   20

#define BOARD_X  (SCREEN_X / CELL_X)
#define BOARD_Y  (SCREEN_Y / CELL_Y)

bool board[BOARD_X][BOARD_Y];


struct pair {
    int x, y;
};
typedef struct pair Pair;

Pair coord_to_cell(int x, int y) {
    Pair p = { x/CELL_X, y/CELL_Y };
    return p;
}

void render_gol(SDL_Surface *s, bool b[BOARD_X][BOARD_Y]) {
    SDL_Rect rect = {0};

    for (usize i=0; i<BOARD_X; i++) {
        for (usize j=0; j<BOARD_Y; j++) {

            rect.x = i * CELL_X;
            rect.y = j * CELL_Y;
            rect.w = CELL_X;
            rect.h = CELL_Y;

            if (b[i][j]) {
                SDL_FillRect(s, &rect, SDL_MapRGB(s->format, 0x80, 0x80, 0x80));
            } else {
                SDL_FillRect(s, &rect, SDL_MapRGB(s->format, 0x00, 0x00, 0x00));
            }
        }
    }
}

int wrap(int v, int total) {
    if (v >= total) {
        return v % total;
    }

    if (v < 0) {
        return v + total;
    } else {
        return v;
    }
}

bool next(bool (*b)[BOARD_X][BOARD_Y], int x, int y) {
    usize count = 0;

    for (isize i=-1; i<=1; i++) {
        for (isize j=-1; j<=1; j++) {
            if (i == 0 && j == 0) {
                continue;
            }

            int x_coord = wrap(x+i, BOARD_X);
            int y_coord = wrap(y+j, BOARD_Y);

            if ((*b)[x_coord][y_coord]) {
                count += 1;
            }
        }
    }
    if (count == 2) { return (*b)[x][y]; }
    if (count == 3) { return true; }
    return false;
}

void advance_gol(bool b[BOARD_X][BOARD_Y]) {
    bool scratch[BOARD_X][BOARD_Y] = {{false}};

    for (usize i=0; i<BOARD_X; i++) {
        for (usize j=0; j<BOARD_Y; j++) {
            scratch[i][j] = next(&board, i, j);
        }
    }
    memmove(b, scratch, sizeof(bool[BOARD_X][BOARD_Y]));
}

void reset_gol(bool b[BOARD_X][BOARD_Y]) {
    memset(b, false, sizeof(bool[BOARD_X][BOARD_Y]));

    /*
    b[20][30] = true;
    b[19][30] = true;
    b[20][31] = true;
    b[21][31] = true;
    b[20][32] = true;
    */
}

void clear_screen(SDL_Window *window, SDL_Surface *surface) {
    SDL_FillRect(surface, NULL, SDL_MapRGB(surface->format, 0xFF, 0xFF, 0xFF));
    SDL_UpdateWindowSurface(window);
}

int main() {
    reset_gol(board);

    SDL_Window *window = NULL;
    SDL_Surface *surface = NULL;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("init SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    window = SDL_CreateWindow("GOL",  SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_X, SCREEN_Y, SDL_WINDOW_SHOWN );
    if (window == NULL) {
        printf("window SDL_Error: %s\n", SDL_GetError());
        return 2;
    }

    surface = SDL_GetWindowSurface(window);
    clear_screen(window, surface);

    SDL_Event event;

    bool stepwize = true;
    bool run_step = false; // run the first step automaticaly
    int delay = 20;
    int current_delay = 0;

    bool mouse_pressed = false;
    bool mouse_to_state = true;
    Pair cell;

    render_gol(surface, board);
    SDL_UpdateWindowSurface(window);

    while (true) {

        // HANDLE INPUT
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
            case SDL_QUIT:
                SDL_DestroyWindow(window);
                SDL_Quit();
                return 0;
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym) {
                case SDLK_r:
                    reset_gol(board);
                    break;
                case SDLK_n:
                    run_step = true;
                    break;
                case SDLK_s:
                    stepwize = !stepwize;
                    break;
                case SDLK_q:
                    SDL_DestroyWindow(window);
                    SDL_Quit();
                    return 0;
                case SDLK_PLUS:
                case SDLK_EQUALS:
                    delay += 3;
                    break;
                case SDLK_MINUS:
                    delay -= 3;
                    if (delay < 0) { delay = 0; }
                    break;
                }
                break;
            case SDL_MOUSEBUTTONDOWN:
                cell = coord_to_cell(event.button.x, event.button.y);
                board[cell.x][cell.y] = !board[cell.x][cell.y];

                mouse_to_state = board[cell.x][cell.y];
                mouse_pressed = true;

                render_gol(surface, board);
                SDL_UpdateWindowSurface(window);
                continue;
            case SDL_MOUSEBUTTONUP:
                mouse_pressed = false;
                break;
            case SDL_MOUSEMOTION:
                if (!mouse_pressed) {
                    break;
                }
                cell = coord_to_cell(event.motion.x, event.motion.y);
                board[cell.x][cell.y] = mouse_to_state;

                render_gol(surface, board);
                SDL_UpdateWindowSurface(window);
                break;
            }
        }

        // NOTHING TO DO
        if (stepwize && !run_step) {
            SDL_Delay(15);
            continue;
        }

        if (!stepwize && delay > 0 && current_delay > 0) {
            current_delay -= 1;
            SDL_Delay(15);
            continue;
        }

        // COMPUTE
        advance_gol(board);
        run_step = false;
        current_delay = delay;

        // RENDER
        render_gol(surface, board);
        SDL_UpdateWindowSurface(window);


        // FRAME SLEEP
        SDL_Delay(15);
    }


    return 0;
}

