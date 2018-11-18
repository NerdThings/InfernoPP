//
// Created by Reece Mackie on 15/11/18.
//

#include "TestScene.h"

#include "Input/Keyboard.h"
#include "Input/KeyboardState.h"
#include "Input/Mouse.h"
#include "Input/MouseState.h"
#include "Vector3.h"

TestScene::TestScene(Inferno::Game* parent_game) : Scene(parent_game, 1024, 768) {}

void TestScene::draw(Inferno::Graphics::Renderer *renderer) {
    std::string working_dir = Inferno::Content::ContentLoader::get_working_directory();
    
    if (tmb == nullptr) {
        Inferno::Graphics::Texture2D* temp = Inferno::Content::ContentLoader::load_texture(parent_game->graphics_device, working_dir + "/Content/Test_Sprite.png");
        tmb = new Inferno::Graphics::Sprite(temp, Inferno::Vector2(0, 0), 16, 16, 10);
    }
    
    Inferno::Input::MouseState s = Inferno::Input::Mouse::get_state(parent_game, nullptr);
    Inferno::Input::KeyboardState ks = Inferno::Input::Keyboard::get_state();
    
    Inferno::Graphics::Color c;
    
    if (s.left_button == Inferno::Input::Pressed) {
        c = Inferno::Graphics::Color(0.5f, 1.0f, 1.0f, 1.0f);
    } else if (ks.is_key_down(Inferno::Input::Space)) {
        c = Inferno::Graphics::Color(1.0f, 1.0f, 0.5f, 1.0f);
    } else {
        c = Inferno::Graphics::Color(1.0f, 0.5f, 1.0f, 1.0f);
    }
    
    renderer->draw_rectangle(Inferno::Rectangle(s.x, s.y, 50, 50), c, 0);
    renderer->draw_texture(tmb->get_current_texture(), Inferno::Rectangle(60, 60, 16, 16), tmb->get_source_rectangle(), tmb->origin, Inferno::Graphics::Color(255, 255, 255, 255), 0);
    
    printf("DRAWN\n");
}

void TestScene::update() {
	if (tmb != nullptr)
		tmb->update();
	printf("UPDATED\n");
}