//
// Created by Reece Mackie on 13/11/18.
//

#include <chrono>
#include <cmath>
#include <thread>

#include "Inferno/Content/ContentLoader.h"
#include "Inferno/Graphics/Color.h"
#include "Inferno/Graphics/GraphicsDevice.h"
#include "Inferno/Graphics/Renderer.h"
#include "Inferno/Graphics/RenderTarget.h"
#include "Inferno/Graphics/Texture2D.h"
#include "Inferno/Game.h"
#include "Inferno/GameWindow.h"
#include "Inferno/Scene.h"
#include "Inferno/Rectangle.h"

namespace Inferno {
    //Private Methods
    
    void Game::begin_update() {
        if (_current_scene != nullptr)
            _current_scene->begin_update();
    }
    
    void Game::draw() {
        //Get window dimensions
        Vector2 window_size = game_window->get_size();
        
        //Set view size
        float view_width = window_size.x;
        float view_height = window_size.y;
        
        //Calculate ratios
        float output_aspect = window_size.x / float(window_size.y);
        float preferred_aspect = _virtual_width / float(_virtual_height);
        
        //Init bar dimensions
        float bar_width = 0;
        float bar_height = 0;
        
        //Calculate view
        if (output_aspect <= preferred_aspect) {
            view_height = int(window_size.x / preferred_aspect + 0.5f);
            bar_height = (window_size.y - view_height) / 2;
        } else {
            view_width = int(window_size.y * preferred_aspect + 0.5f);
            bar_width = (window_size.x - view_width) / 2;
        }
        
        //Clear outer zone
        graphics_device->clear(Graphics::Color(0, 0, 0, 255));
        
        //Render to target
        graphics_device->set_render_target(render_target);
    
        //Clear target
        graphics_device->clear(clear_color);
    
        //Draw current scene
        if (_current_scene != nullptr)
            _current_scene->draw(renderer);
        
        //Unset target
        graphics_device->set_render_target(nullptr);
        
        //Draw target
        renderer->draw_render_target(render_target, Rectangle(bar_width, bar_height, view_width, view_height), nullptr, 0, 0, Graphics::Color(255, 255, 255, 255));
    }
    
    void Game::end_update() {
        if (_current_scene != nullptr)
            _current_scene->end_update();
    }
    
    void Game::update() {
        if (_current_scene != nullptr)
            _current_scene->update();
    }
    
    int get_time() {
        auto time = std::chrono::high_resolution_clock::now();
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(time.time_since_epoch());
        return ms.count();
    }
    
    void Game::update_thread() {
        //Begin loop
        int previous = get_time();
        float lag = 0.0f;
        running = true;
    
        //Run an initial tick
        begin_update();
        update();
        end_update();
    
        while (running) {
            int current = get_time();
            const float delta = current - previous;
            previous = current;
            lag += delta;
        
            while (lag >= 1000.0f / frames_per_second) {
                lag -= 1000.0f / frames_per_second;
    
                if (paused)
                    return;
    
                //Run an update
                begin_update();
                update();
                end_update();
            }
        }
    }
    
    //Protected Methods
    void Game::initialise() {
    }
    
    //Constructors
    
    Game::Game(int width, int height, std::string title, int fps, bool fullscreen) : _virtual_width(width), _virtual_height(height), frames_per_second(fps) {
        //Create window
        game_window = new GameWindow(this, title, width, height);
        
        //Create graphics device
        graphics_device = new Graphics::GraphicsDevice(this);
        
        //Create renderer
        renderer = new Graphics::Renderer(graphics_device);
        
        //Create main render target
        render_target = new Graphics::RenderTarget(_virtual_width, _virtual_height);
    
        //Set properties
        game_window->set_fullscreen(fullscreen);
        paused = false;
        running = false;
    }
    
    //Deconstructors
    
    Game::~Game() = default;
    
    //Methods
    
    Vector2 Game::get_virtual_dimensions() {
        return {float(_virtual_width), float(_virtual_height)};
    }
    
    void Game::run() {
        //Run init
        initialise();
        
        //Launch update thread
        std::thread thread(&Game::update_thread, this);
        
        //Begin draw loop
        int previous = get_time();
        float lag = 0.0f;
        running = true;
        
        while (running) {
            //Stop locked fps > 60 (Doesn't perform well enough to be viable)
            if (frames_per_second > 60 && locked_framerate)
                throw std::runtime_error("Cannot lock framerate to a value above 60fps.");

            //Run window events
            if (!game_window->run_events())
                running = false;

            //Run game specific rendering
            if (locked_framerate) {
                int current = get_time();
                const float delta = current - previous;
                previous = current;
                lag += delta;

                while (lag >= 1000.0f / frames_per_second) {
                    lag -= 1000.0f / frames_per_second;

                    //Begin draw
                    renderer->begin_draw();

                    //Draw
                    draw();

                    //End draw
                    renderer->end_draw();

                    //Present
                    game_window->present();
                }
            } else {
                //Begin draw
                renderer->begin_draw();

                //Draw
                draw();

                //End draw
                renderer->end_draw();

                //Present
                game_window->present();
            }
        }
        
        //Wait for update thread to finish
        thread.join();
    
        //Fix for the linux resolution bug
        game_window->set_fullscreen(false);
    
        //Unload current scene
        if (_current_scene != nullptr) {
            _current_scene->unloaded();
            _current_scene = nullptr;
        }
    
        //Delete render target
        delete render_target;
        render_target = nullptr;
    
        //Delete renderer
        delete renderer;
        renderer = nullptr;
    
        //Delete graphics device
        delete graphics_device;
        graphics_device = nullptr;
    
        //Delete game window
        delete game_window;
        game_window = nullptr;
    }
    
    void Game::set_scene(Inferno::Scene *scene) {
        //Unload current scene
        if (_current_scene != nullptr)
            _current_scene->unloaded();
        
        //Update scene
        _current_scene = scene;
        
        //Load new scene
        if (_current_scene != nullptr)
            _current_scene->loaded();
    }
}
