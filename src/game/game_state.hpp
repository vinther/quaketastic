#ifndef GAME_STATE_HPP
#define GAME_STATE_HPP

#include <cstdint>
#include <GL/glfw.h>

#include "../scene/camera.hpp"


namespace qts {
struct game_state
{
    enum class mouse_mode : uint8_t {
        FREE,
        LOCKED,
    };

    std::shared_ptr<camera> active_camera;
    std::unordered_map<std::string, std::function<void(game_state&)>> active_effects;

    bool quit_requested = false;
    mouse_mode current_mouse_mode = mouse_mode::LOCKED;

    void set_mouse_mode(mouse_mode new_mode) {
        if (mouse_mode::FREE == new_mode) {
            glfwEnable( GLFW_MOUSE_CURSOR );
        } else {
            glfwDisable( GLFW_MOUSE_CURSOR );
        }

        current_mouse_mode = new_mode;
    }
};
}

#endif // GAME_STATE_HPP
