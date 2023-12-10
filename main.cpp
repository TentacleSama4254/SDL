#include <SDL2/SDL.h>
#include <vector>
#include <array>
#include <iostream>

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

    class Square {
    public:
        float x, y;
        Square(int _x, int _y) {
            this->x = _x;
            this->y = _y;
        }

        void draw(SDL_Renderer* renderer) {
            SDL_Rect square = { static_cast<int> (this->x), static_cast<int>(this->y), 50, 50 };
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            SDL_RenderFillRect(renderer, &square);
        };
    };

    class Snake {
    public:
        float headX = 0, headY = 0;
        float dx = 0.5, dy = 0.5;
        int length = 2;
        vector<array<double, 2>> pos;

        void move() {
            headX += dx;
            headY += dy;

            // Update the position of each segment of the snake's body
            pos.push_back({ 0,0 });
            for (int i = (length - 1); i > 0; i--) {
                pos[i] = pos[i - 1];
            }
            pos[0] = { headX, headY };
            cout << headX << " " << headY << endl;
        };

        void user_move(SDL_Keycode key) {
            switch (key) {
            case SDLK_UP:
                this->dy = -0.5;
                this->dx = 0;
                break;
            case SDLK_DOWN:
                this->dy = 0.5;
                this->dx = 0;
                break;
            case SDLK_LEFT:
                this->dx = -0.5;
                this->dy = 0;
                break;
            case SDLK_RIGHT:
                this->dx = 0.5;
                this->dy = 0;
                break;
            }
        };

        void draw(SDL_Renderer* renderer) {
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderClear(renderer);

            for (int i = 0; i < this->pos.size(); i++) {
                Square sq(this->pos[i][0], this->pos[i][1]);
                sq.draw(renderer);
            }

            SDL_RenderPresent(renderer);
        }

    };

    // Event loop
    bool running = true;
    Snake mySnake;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
            else if (event.type == SDL_KEYDOWN) {
                mySnake.user_move(event.key.keysym.sym);
            }
        }

        // Clear screen
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        mySnake.move();
        mySnake.draw(renderer);

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