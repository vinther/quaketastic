#ifndef DEFAULT_CONTROLS_HPP
#define DEFAULT_CONTROLS_HPP

#include <cstdint>
#include <tuple>


#include "controls.hpp"

namespace qts {
namespace default_keybindings {

using namespace qts::controls;
using std::make_tuple;

keybinding_map keybinding{
    {make_tuple('W', GLFW_PRESS, KEYBOARD), fixed_movement("MOVEMENT_FORWARD", glm::vec3{0.0f, 0.0f, -1.0f})},
    {make_tuple('A', GLFW_PRESS, KEYBOARD), fixed_movement("MOVEMENT_LEFT", glm::vec3(-1.0f, 0.0f, 0.0f))},
    {make_tuple('S', GLFW_PRESS, KEYBOARD), fixed_movement("MOVEMENT_BACKWARD", glm::vec3(0.0f, 0.0f, 1.0f))},
    {make_tuple('D', GLFW_PRESS, KEYBOARD), fixed_movement("MOVEMENT_RIGHT", glm::vec3(1.0f, 0.0f, 0.0f))},
    {make_tuple(GLFW_KEY_LSHIFT, GLFW_PRESS, KEYBOARD), fixed_movement("MOVEMENT_DOWN", glm::vec3(0.0f, -1.0f, 0.0f))},
    {make_tuple(GLFW_KEY_SPACE, GLFW_PRESS, KEYBOARD), fixed_movement("MOVEMENT_UP", glm::vec3(0.0f, 1.0f, 0.0f))},

    {make_tuple('W', GLFW_RELEASE, KEYBOARD), cancel_effect("MOVEMENT_FORWARD")},
    {make_tuple('A', GLFW_RELEASE, KEYBOARD), cancel_effect("MOVEMENT_LEFT")},
    {make_tuple('S', GLFW_RELEASE, KEYBOARD), cancel_effect("MOVEMENT_BACKWARD")},
    {make_tuple('D', GLFW_RELEASE, KEYBOARD), cancel_effect("MOVEMENT_RIGHT")},
    {make_tuple(GLFW_KEY_LSHIFT, GLFW_RELEASE, KEYBOARD), cancel_effect("MOVEMENT_DOWN")},
    {make_tuple(GLFW_KEY_SPACE, GLFW_RELEASE, KEYBOARD), cancel_effect("MOVEMENT_UP")},

    {make_tuple('R', GLFW_PRESS, KEYBOARD), [](game_state& g){ g.active_camera->eye = glm::vec3(0.0f, 1.0f, 0.0f); }},

    {make_tuple(GLFW_KEY_ESC, GLFW_PRESS, KEYBOARD), [](game_state& g){ g.quit_requested = true; }},

    {make_tuple(GLFW_MOUSE_BUTTON_1, GLFW_PRESS, MOUSE_BUTTON), [](game_state& g){ std::cout << "Shoot!" << std::endl; }},
    {make_tuple(GLFW_MOUSE_BUTTON_1, GLFW_RELEASE, MOUSE_BUTTON), cancel_effect("MOVEMENT_FORWARD")},

    {make_tuple(GLFW_MOUSE_BUTTON_2, GLFW_PRESS, MOUSE_BUTTON), [](game_state& g){ g.set_mouse_mode(game_state::mouse_mode::FREE); }},
    {make_tuple(GLFW_MOUSE_BUTTON_2, GLFW_RELEASE, MOUSE_BUTTON), [](game_state& g){ g.set_mouse_mode(game_state::mouse_mode::LOCKED); }},
};

}
}

#endif // DEFAULT_CONTROLS_HPP


