#include <vector>
#include <iostream>

#include <chrono>
#include <array>
#include <fstream>
#include <algorithm>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <thread>

#include <boost/log/trivial.hpp>
#include <boost/log/sinks/text_file_backend.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>

#include "scene/scene.hpp"
#include "scene/camera.hpp"

#include "input/controls.hpp"
#include "input/default_controls.hpp"

#include "resources/resource_cache.hpp"
#include "resources/bsp_map_loader.hpp"
#include "resources/mesh_loader.hpp"

#include <GL/glfw.h>
#include <glm/gtc/matrix_transform.hpp>

template<typename T, int size>
int GetArrLength(T(&)[size]){ return size; }

class application {
public:
    application(std::vector<std::string> args)
        : args(args)
        , window_instance(0)
        , window_width(1366), window_height(768), color_depth(8)
        , window_title("Quaketastic")
    {
    }

    int run() {
        if (!initialize()) {
            return 1;
        }

        app_scene.initialize();

        qts::resource_cache<qts::mesh_loader> mesh_cache;
        qts::static_object obj1("models/cow-nonormals.obj", mesh_cache);
        qts::static_object obj2("models/sponza.obj", mesh_cache);
        qts::static_object obj3("models/cow-nonormals.obj", mesh_cache);
        qts::static_object obj4("models/ladybird.obj", mesh_cache);

        obj1.transform = glm::translate(obj1.transform, glm::vec3(-100.0f, 0.0f, 0.0f));
        obj3.transform = glm::translate(obj3.transform, glm::vec3( 100.0f, 500.0f, 0.0f));

        qts::resource_cache<qts::bsp_map_loader> bsp_map_cache;
        bsp_map_cache.get_resource("maps/q3ctf1.bsp");

        app_scene.append(obj1);
        app_scene.append(obj2);
        app_scene.append(obj3);
        app_scene.append(obj4);

        uint64_t frame_id = 0;
        int key, action, type;
        int mouse_x, mouse_y;
        decltype(keybindings)::const_iterator keybinding_iter;

        while (glfwGetWindowParam(GLFW_OPENED) && ++frame_id) {
            glfwSwapBuffers();

            while (0 < input_events.size()) {
                std::tie(key, action, type) = input_events.front();

                keybinding_iter = keybindings.find(std::make_tuple(key, action, type));

                if (std::end(keybindings) != keybinding_iter) {
                    keybinding_iter->second(game_state);
                }

                if (GLFW_PRESS == action) {
                    active_keys.insert(key);
                } else if (GLFW_RELEASE == action) {
                    active_keys.erase(key);
                }

                input_events.pop();
            }

            if (game_state.quit_requested) {
                glfwCloseWindow();
            }

            if (qts::game_state::mouse_mode::LOCKED == game_state.current_mouse_mode) {
                int current_mouse_x, current_mouse_y;
                int dx, dy;
                auto& rotation = game_state.active_camera->rotation;

                glfwGetMousePos(&current_mouse_x, &current_mouse_y);

                dx = (current_mouse_x - mouse_x);
                dy = (current_mouse_y - mouse_y);

                rotation = rotation * (glm::quat(1.0f, glm::vec3(dy * 0.000f, dx * 0.001f, 0.0f)));
                rotation = (glm::quat(1.0f, glm::vec3(dy * 0.001f, dx * 0.000f, 0.0f))) * rotation;
                rotation = glm::normalize(rotation);

                mouse_x = current_mouse_x;
                mouse_y = current_mouse_y;
            }

            for (auto& kp: game_state.active_effects) {
                kp.second(game_state);
            }

            app_scene.render(*game_state.active_camera);

            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }

        return 0;
    }

    bool initialize() {
    	BOOST_LOG_TRIVIAL(info) << "Application initialized";

        if (!glfwInit())
            return false;

        window_instance = glfwOpenWindow(window_width, window_height, color_depth,
                                         color_depth, color_depth, 0, 24, 0, GLFW_WINDOW);

        if (!window_instance)
            return false;

        glfwSetWindowTitle(window_title.c_str());
        glfwSetKeyCallback(application::key_callback);
        glfwSetMouseButtonCallback(mouse_button_callback);
        glfwSetMouseWheelCallback(mouse_wheel_callback);

        glfwSwapBuffers();

        keybindings = qts::default_keybindings::keybinding;

        game_state.active_camera.reset(new qts::camera());
        game_state.active_camera->eye = glm::vec3(0.0f, 500.0f, 0.0f);

        return true;
    }

    std::vector<std::string> args;

    qts::scene app_scene;
    qts::game_state game_state;

    qts::keybinding_map keybindings;

    qts::key_set active_keys;
    qts::mouse_button_set active_mouse_buttons;

    int window_instance;
    int window_width, window_height, color_depth;
    std::string window_title;

    typedef std::tuple<int, int8_t, int8_t> input_event;
    typedef int mouse_wheel_event;

    static std::queue<input_event> input_events;
    static std::queue<mouse_wheel_event> mouse_wheel_events;

    static void key_callback(int key, int state) { input_events.emplace(key, state, qts::controls::KEYBOARD); }
    static void mouse_button_callback(int button, int action) { input_events.emplace(button, action, qts::controls::MOUSE_BUTTON); }
    static void mouse_wheel_callback(int pos) { mouse_wheel_events.emplace(pos); }
};

std::queue<application::input_event> application::input_events;
std::queue<application::mouse_wheel_event> application::mouse_wheel_events;

int main(int argc, char** argv) {
    std::vector<std::string> args;

    for (int i = 0; i < argc; ++i) {
        args.push_back(std::string(argv[i]));
    }

    application quaketastic(args);

    return quaketastic.run();
}
