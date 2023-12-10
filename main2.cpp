#include <SDL2/SDL.h>
#include <vector>
#include <array>
using namespace std;

int main(int argc, char* argv[]) {
    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;
    SDL_Event event;
    SDL_Rect square = { 400, 300, 50, 50 }; // Initial position and size of the square
    float vx = 0.5, vy = 0.5; // Initial velocity of the square

    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        SDL_Log("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    // Create window
    window = SDL_CreateWindow("SDL2 Window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 2400, 1800, SDL_WINDOW_SHOWN);
    if (window == NULL) {
        SDL_Log("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    // Create renderer
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        SDL_Log("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    class Snake {
    public:
        int headX, headY;
        int dx, dy;
        int length = 0;
        vector<array<double, 2>> pos;

        Snake();

        void move() {
            headX += dx;
            headY += dy;

            // Update the position of each segment of the snake's body
            for (int i = length - 1; i > 0; i--) {
                pos[i] = pos[i - 1];
            }
            pos[0] = { headX, headY };
        };

        void user_move(SDL_Keycode key) {
            switch (key) {
            case SDLK_UP:
                this->dy = -1;
                this->dx = 0;
                break;
            case SDLK_DOWN:
                this->dy = 1;
                this->dx = 0;
                break;
            case SDLK_LEFT:
                this->dx = -1;
                this->dy = 0;
                break;
            case SDLK_RIGHT:
                this->dx = 1;
                this->dy = 0;
                break;
            }
        };

        // void draw() {
        //     for (int i=0; i<this->length;i++){
        //         SDL_Rect square = { this->pos[i][0],this->pos[i][1], 50, 50 };

        //         SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        //         SDL_RenderFillRect(renderer, &square);
        //     }
        // }

    };

    // Event loop
    bool running = true;
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
            else if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                case SDLK_UP:    vy -= 1; break;
                case SDLK_DOWN:  vy += 1; break;
                case SDLK_LEFT:  vx -= 1; break;
                case SDLK_RIGHT: vx += 1; break;
                }
            }
        }

        // Update square position
        square.x += vx;
        square.y += vy;

        // if (vx > 0) vx -= 0.01; else if (vx < 0) vx += 0.01;
        // if (vy > 0) vy -= 0.01; else if (vy < 0) vy += 0.01;

        // Check for collision with window boundaries
        if (square.x < 0 || square.x + square.w > 2400) vx = -0.95 * vx;
        if (square.y < 0 || square.y + square.h > 1800) vy = -0.95 * vy;

        // Clear screen
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Draw square
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderFillRect(renderer, &square);

        // Update screen
        SDL_RenderPresent(renderer);
    }

    // Destroy renderer and window
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    // Quit SDL subsystems
    SDL_Quit();

    return 0;
}