#include <iostream>
#include <string>
#include "Game.h"
#include <SDL2/SDL.h>
#include "entt.hpp"
#include "components.hpp"

void Game::boxRenderSystem(SDL_Renderer *renderer)
{
    const auto view = reg.view<BoxComponent, PositionComponent>();
    for (const auto e : view)
    {
        auto box = view.get<BoxComponent>(e);
        auto position = view.get<PositionComponent>(e);
        SDL_Rect rect;
        rect.x = position.x;
        rect.y = position.y;
        rect.w = box.w;
        rect.h = box.h;
        SDL_RenderFillRect(renderer, &rect);
    }
}

void Game::handleEventSystem()
{
    SDL_Event event;
    auto &speed1 = reg.get<VelocityComponent>(player1);
    auto &speed2 = reg.get<VelocityComponent>(player2);
    const Uint8 *state = SDL_GetKeyboardState(NULL);
    int move = 10;
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT)
        {
            isRunning = false;
        }


        if (event.type == SDL_KEYDOWN)
        {
            std::cout << "KEY PRESSED" << std::endl;

            if (state[SDL_SCANCODE_W])
            {
                speed1.vel_y = -move;
            }
            else if (state[SDL_SCANCODE_S])
            {
                speed1.vel_y = move;
            }

            if (state[SDL_SCANCODE_UP])
            {
                speed2.vel_y = -move;
            }
            else if (state[SDL_SCANCODE_DOWN])
            {
                speed2.vel_y = move;
            }
        }
        if (event.type == SDL_KEYUP)
        {
            std::cout << "KEY RELEASED" << std::endl;

            int key = event.key.keysym.sym;
            if (key == SDLK_w)
            {
                speed1.vel_y = 0;
            }
            else if (key == SDLK_s)
            {
                speed1.vel_y = 0;
            }

            if (key == SDLK_UP)
            {
                speed2.vel_y = 0;
            }
            else if (key == SDLK_DOWN)
            {
                speed2.vel_y = 0;
            }
        }
    }
}

void Game::movementSystem()
{
    const auto view = reg.view<PositionComponent, VelocityComponent>();
    for (const auto e : view)
    {
        auto &speed = view.get<VelocityComponent>(e);
        auto &position = view.get<PositionComponent>(e);

        position.x += speed.vel_x;
        position.y += speed.vel_y;
    }
}

void Game::limitSystem()
{
    //Player 1
    if (reg.get<PositionComponent>(player1).y <= 0)
    {
        reg.get<PositionComponent>(player1).y = 0;
    }
    if (reg.get<PositionComponent>(player1).y >= window_height - reg.get<BoxComponent>(player1).h)
    {
        reg.get<PositionComponent>(player1).y = window_height - reg.get<BoxComponent>(player1).h;
    }

    //Player 2
    if (reg.get<PositionComponent>(player2).y <= 0)
    {
        reg.get<PositionComponent>(player2).y = 0;
    }
    if (reg.get<PositionComponent>(player2).y >= window_height - reg.get<BoxComponent>(player2).h)
    {
        reg.get<PositionComponent>(player2).y = window_height - reg.get<BoxComponent>(player2).h;
    }
}

void Game::ballMovementSystem()
{

    reg.get<PositionComponent>(ball).y += reg.get<VelocityComponent>(ball).vel_y * dT;
    if ((reg.get<PositionComponent>(ball).y < 0) || (reg.get<PositionComponent>(ball).y > (window_height - reg.get<BoxComponent>(ball).h)))
    {
        reg.get<VelocityComponent>(ball).vel_y *= -1.05f;
    }

    reg.get<PositionComponent>(ball).x += reg.get<VelocityComponent>(ball).vel_x * dT;
    if (reg.get<VelocityComponent>(ball).vel_x < 0)
    {
        if ((reg.get<PositionComponent>(ball).y + reg.get<BoxComponent>(ball).h >= reg.get<PositionComponent>(player1).y) && (reg.get<PositionComponent>(ball).y <= reg.get<PositionComponent>(player1).y + reg.get<BoxComponent>(player1).h))
        {
            if (reg.get<PositionComponent>(ball).x <= 0 + reg.get<BoxComponent>(player1).w)
            {
                reg.get<VelocityComponent>(ball).vel_x *= -1.05f;
            }
        }
        if (reg.get<PositionComponent>(ball).x <= 0)
        {
            std::cout << "Player 2 Won" << std::endl;
            isRunning = false;
        }
    }

    else if (reg.get<VelocityComponent>(ball).vel_x > 0)
    {
        if ((reg.get<PositionComponent>(ball).y + reg.get<BoxComponent>(ball).h >= reg.get<PositionComponent>(player2).y) && (reg.get<PositionComponent>(ball).y <= reg.get<PositionComponent>(player2).y + reg.get<BoxComponent>(player2).h))
        {
            if (reg.get<PositionComponent>(ball).x >= (window_width - reg.get<BoxComponent>(ball).w - reg.get<BoxComponent>(player2).w))
            {
                reg.get<VelocityComponent>(ball).vel_x *= -1.05f;
            }
        }
        if (reg.get<PositionComponent>(ball).x >= window_width - reg.get<BoxComponent>(ball).w)
        {
            std::cout << "Player 1 Won" << std::endl;
            isRunning = false;
        }
    }
}





