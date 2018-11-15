//
// Created by Reece Mackie on 13/11/18.
//

#include <ctime>

#include "Content/ContentLoader.h"
#include "Graphics/Color.h"
#include "Graphics/GraphicsDevice.h"
#include "Graphics/Renderer.h"
#include "Graphics/RenderTarget.h"
#include "Graphics/Texture2D.h"
#include "Game.h"
#include "GameWindow.h"
#include "Point.h"
#include "Rectangle.h"

namespace Inferno {
    //Private Methods
    
    void Game::begin_update() {
    
    }
    
    void Game::do_draw() {
        if (paused | lock_framerate)
            return;
    
        //Draw frame
        draw();
    
        //Present
        game_window->present();
        
        //End draw
        graphics_device->end_draw();
    }
    
    void Game::do_tick() {
        if (paused)
            return;
    
        //Run an update
        begin_update();
        update();
        end_update();
    
        //Draw a frame if the framerate is locked
        if (lock_framerate) {
            //Draw
            draw();
        
            //Present
            game_window->present();
            
            //End draw
            graphics_device->end_draw();
        }
    }
    
    //Test globals:
    Graphics::Texture2D* tmb = nullptr;
    Graphics::RenderTarget* tmb1 = nullptr;
    
    void Game::draw() {
        //Get window dimensions
        Point window_size = game_window->get_size();
        
        //Set view size
        int view_width = window_size.x;
        int view_height = window_size.y;
        
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
    
        //The following is test code
        if (tmb == nullptr)
            tmb = Content::ContentLoader::load_texture(graphics_device, "/home/reece/Programs/mc/icon.png");
        
        graphics_device->clear(Graphics::Color(255, 255, 255, 255));
        renderer->draw_rectangle(Rectangle(10, 10, 50, 50), Graphics::Color(0, 255, 0, 255), 0);
        renderer->draw_texture(tmb, Rectangle(60, 60, 144, 144), Graphics::Color(255, 255, 255, 255), 0);
        
        //Unset target
        graphics_device->set_render_target(nullptr);
        
        //Draw target
        renderer->draw_render_target(render_target, Rectangle(bar_width, bar_height, view_width, view_height), Graphics::Color(255, 255, 255, 255), 0);
    }
    
    void Game::end_update() {
    
    }
    
    void Game::update() {
    
    }
    
    //Protected Methods
    void Game::initialise() {
        game_window->resizable(true);
    }
    
    //Constructors
    
    Game::Game(int width, int height, const char *title, int fps, bool fullscreen) : _virtual_width(width), _virtual_height(height), frames_per_second(fps) {
        //Create window
        game_window = new GameWindow(this, title, width, height);
        
        //Create graphics device
        graphics_device = new Graphics::GraphicsDevice(this);
        
        //Create renderer
        renderer = new Graphics::Renderer(graphics_device);
        
        //Create main render target
        render_target = new Graphics::RenderTarget(graphics_device, _virtual_width, _virtual_height);
    
        //Set properties
        game_window->set_fullscreen(fullscreen);
        paused = false;
        running = false;
    }
    
    //Deconstructors
    
    Game::~Game() {
    
    }
    
    //Methods
    
    Point Game::get_virtual_dimensions() {
        return {_virtual_width, _virtual_height};
    }
    
    void Game::run() {
        //Run init
        initialise();
        
        //Begin loop
        int previous = int(double(clock()) / CLOCKS_PER_SEC) * 1000;
        float lag = 0.0f;
        running = true;
        while (running) {
            const int current = int(double(clock()) / CLOCKS_PER_SEC) * 1000;
            const int delta = current - previous;
            previous = current;
            lag += delta;
            
            while (lag >= 1000.0f / frames_per_second) {
                //Run a tick
                do_tick();
                
                lag -= 1000.f / frames_per_second;
            }
            
            //Draw a frame
            do_draw();
            
            //Run window events
            if (!game_window->run_events()) {
                running = false;
            }
        }
        
        //Fix for the linus resolution bug
        game_window->set_fullscreen(false);
        
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
}