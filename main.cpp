#include <iostream>

#include <GL/gl.h>
#include <GL/glfw.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>

#include <chrono>
#include <array>
#include <fstream>
#include <algorithm>
#include <queue>
#include <unordered_map>
#include <unordered_set>

#include "model_loader.hpp"
#include "bsp_map_loader.hpp"

using namespace std;

template<typename T, int size>
int GetArrLength(T(&)[size]){ return size; }

typedef std::pair<int, int> key_event;
typedef std::pair<int, int> mouse_button_event;
typedef int mouse_wheel_event;

std::queue<key_event> key_events;
std::queue<mouse_button_event> mouse_button_events;
std::queue<mouse_wheel_event> mouse_wheel_events;

struct camera {
    glm::vec3 eye;
    glm::vec3 up;

    glm::quat rotation;

    glm::vec3 translational_velocity;
    glm::quat rotational_velocity;

    camera()
        : up(glm::vec3(0.0f, 1.0f, 0.0f))
        , rotation(glm::quat(1.0f, glm::vec3(0.0f, 0.0f, 0.0f)))
    {}
};

enum class control_type : char {
    TRANSLATE_FORWARDS,
    TRANSLATE_BACKWARDS,
    TRANSLATE_LEFT,
    TRANSLATE_RIGHT,
    TRANSLATE_UP,
    TRANSLATE_DOWN,

    ROTATE_YAW_POS,
    ROTATE_YAW_NEG,

    ROTATE_PITCH_POS,
    ROTATE_PITCH_NEG,
};

typedef std::pair<glm::vec3, glm::quat> actuator;

std::unordered_map<int, control_type> keybindings {
    {'W', control_type::TRANSLATE_FORWARDS},
    {'S', control_type::TRANSLATE_BACKWARDS},
    {'A', control_type::TRANSLATE_LEFT},
    {'D', control_type::TRANSLATE_RIGHT},
    {'Z', control_type::TRANSLATE_DOWN},
    {'X', control_type::TRANSLATE_UP},

    {'Q', control_type::ROTATE_YAW_NEG},
    {'E', control_type::ROTATE_YAW_POS},

    {GLFW_KEY_SPACE, control_type::TRANSLATE_UP},
    {GLFW_KEY_LSHIFT, control_type::TRANSLATE_DOWN},
};

namespace std
{
    template <>
    struct hash<control_type>
    {
        size_t operator()(const control_type& v) const
        {
            return hash<int>()(static_cast<int>(v));
        }
    };
}

std::unordered_map<control_type, std::function<actuator(const camera&)>> keybinding_actions {
    {control_type::TRANSLATE_BACKWARDS, [](const camera& cam) { return actuator{glm::vec3(0.0f, 0.0f, -1.0f) * cam.rotation, glm::quat(1.0f, glm::vec3(0.0f, 0.0f, 0.0f))}; }},
    {control_type::TRANSLATE_FORWARDS, [](const camera& cam) { return actuator{glm::vec3(0.0f, 0.0f, 1.0f) * cam.rotation, glm::quat(1.0f, glm::vec3(0.0f, 0.0f, 0.0f))}; }},
    {control_type::TRANSLATE_LEFT, [](const camera& cam) { return actuator{glm::vec3(1.0f, 0.0f, 0.0f) * cam.rotation, glm::quat(1.0f, glm::vec3(0.0f, 0.0f, 0.0f))}; }},
    {control_type::TRANSLATE_RIGHT, [](const camera& cam) { return actuator{glm::vec3(-1.0f, 0.0f, 0.0f) * cam.rotation, glm::quat(1.0f, glm::vec3(0.0f, 0.0f, 0.0f))}; }},
    {control_type::TRANSLATE_DOWN, [](const camera& cam) { return actuator{cam.up * 1.0f, glm::quat(1.0f, glm::vec3(0.0f, 0.0f, 0.0f))}; }},
    {control_type::TRANSLATE_UP, [](const camera& cam) { return actuator{-cam.up * 1.0f, glm::quat(1.0f, glm::vec3(0.0f, 0.0f, 0.0f))}; }},
    {control_type::ROTATE_YAW_NEG, [](const camera& cam) { return actuator{glm::vec3(0.0f), glm::quat(-M_PI_4, glm::vec3(0.0f, 1.0f, 0.0f)) }; }},
    {control_type::ROTATE_YAW_POS, [](const camera& cam) { return actuator{glm::vec3(0.0f), glm::quat( M_PI_4, glm::vec3(0.0f, 1.0f, 0.0f)) }; }},
};

std::unordered_set<control_type> active_keys;

void printMat(const glm::mat4&  mat){
  int i,j;
  for (j=0; j<4; j++){
    for (i=0; i<4; i++){
    printf("%f ",mat[i][j]);
  }
  printf("\n\n");
 }
}

int main(int argc, char** argv)
{
    const std::string model_directory = "models/";
    const std::string map_directory = "maps/";
    const std::string window_title = "Quaketastic";
    const std::string map_path = map_directory + "jof3dm2.bsp";
    const std::string model_path = model_directory + "ladybird.obj";
    const int window_width = 1280, window_height = 800;
    const int color_depth = 8;

    std::vector<std::string> args;
    args.reserve(argc);

    for (unsigned int i = 0; i < argc; ++i) {
        args.push_back(std::string(argv[i]));
    }

    camera cam;
    cam.eye = glm::vec3(-0.5f, -0.5f, -5.0f);

    glm::mat4 projection_matrix = glm::perspective(56.25f, 16.0f / 10.0f, 0.1f, 10000.0f);

    std::vector<vertex> ladybug_vertex_data;
    model_loader::load_model(model_path, ladybug_vertex_data);

    std::cout << "Done loading object with " << ladybug_vertex_data.size() << " vertices!" << std::endl;

    glm::vec3 model_min, model_max;
    model_loader::get_extrema(ladybug_vertex_data, model_min, model_max);
    model_loader::transform_vertices(model_min, model_max, ladybug_vertex_data);
    model_loader::get_extrema(ladybug_vertex_data, model_min, model_max);

    cam.eye = (-model_max / 2.0f) - glm::vec3(0.0f, 0.0f, 10.0f);
    cam.rotation = glm::quat(1.0f, glm::vec3(0.0f, 0.0f, 0.0f));

    bsp::map map;
    bsp::load_map(map_path, map);

    std::cout << "Done loading map with " << map.vertexes.size() << " vertices!" << std::endl;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    const int window_instance = glfwOpenWindow(window_width, window_height, color_depth, color_depth, color_depth, 0, 24, 0, GLFW_WINDOW);

    /* Create a windowed mode window and its OpenGL context */
    if (!window_instance)
        return -1;

    //glfwDisable(GLFW_MOUSE_CURSOR);
    glfwEnable(GLFW_KEY_REPEAT);
    glfwSetWindowTitle(window_title.c_str());
    glfwSetKeyCallback([&](int key, int state) { key_events.emplace(key, state); });
    glfwSetMouseButtonCallback([&](int button, int action) { mouse_button_events.emplace(button, action); });
    glfwSetMouseWheelCallback([&](int pos) { mouse_wheel_events.emplace(pos); });


//    for (auto& x: map.vertexes) {
//        std::cout << x.position[0] << " " << x.position[1] << " " << x.position[2] << std::endl;
//    }

    std::cout << std::count_if (std::begin(map.faces), std::end(map.faces), [](bsp::face& face) { return face.type == 1; }) << std::endl;
    std::cout << std::count_if (std::begin(map.faces), std::end(map.faces), [](bsp::face& face) { return face.type == 2; }) << std::endl;
    std::cout << std::count_if (std::begin(map.faces), std::end(map.faces), [](bsp::face& face) { return face.type == 3; }) << std::endl;
    std::cout << std::count_if (std::begin(map.faces), std::end(map.faces), [](bsp::face& face) { return face.type == 4; }) << std::endl;

    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(glm::value_ptr(projection_matrix));

    //glEnable(GL_CULL_FACE);
    //glEnable(GL_LIGHTING);
    //glEnable(GL_LIGHT0);

    const auto start = std::chrono::system_clock::now();

    printMat(glm::mat4_cast(cam.rotation));
    std::cout << std::endl;

    /* Loop until the user closes the window */
    float rot = 0.0f;
    while (glfwGetWindowParam(GLFW_OPENED))
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);     // Clear The Screen And The Depth Buffer
        glColor3f(1.0f, 0.0f, 0.0f);

        glMatrixMode(GL_MODELVIEW);

        cam.rotation = glm::quat(2.0f, cam.up);//glm::rotate(cam.rotation, 0.1f, cam.up);
        rot += 0.1f;

        cam.rotation = glm::normalize(cam.rotation);

        glm::mat4 ViewTranslate = glm::translate(glm::mat4(1.0f), cam.eye);
        glm::mat4 ViewRotate = glm::mat4_cast(cam.rotation) ;//glm::rotate(glm::mat4(1.0f), rot, cam.up);//glm::mat4_cast(cam.rotation);
        glm::mat4 Model = glm::scale(glm::mat4(1.0f), glm::vec3(glm::max(model_max.x, glm::max(model_max.y, model_max.z))));
        glm::mat4 MVM =  ViewTranslate * ViewRotate * Model;

        glLoadMatrixf(glm::value_ptr(MVM));

//        glRotatef( / 50, 0.0f, 1.0f, 0.0f);
        //glTranslatef(camera_x, camera_y, camera_z);

        glEnableClientState(GL_VERTEX_ARRAY);
        //glEnableClientState(GL_NORMAL_ARRAY);

        glVertexPointer(3, GL_FLOAT, sizeof(bsp::vertex), reinterpret_cast<GLvoid*>(&map.vertexes[0].position));
        glNormalPointer(GL_FLOAT, sizeof(bsp::vertex), reinterpret_cast<GLvoid*>(&map.vertexes[0].normal));

        //glDrawArrays(GL_POINTS, 0, map.vertexes.size());

        glVertexPointer(3, GL_FLOAT, sizeof(vertex), reinterpret_cast<GLvoid*>(ladybug_vertex_data.data()));
        glDrawArrays(GL_POINTS, 0, ladybug_vertex_data.size());

//        static const int stride = sizeof(bsp::vertex); // BSP Vertex, not float[3]
//        for(const bsp::face& curFace: map.faces) {
//            const int offset    = curFace.vertex;

//            glVertexPointer(3, GL_FLOAT, stride, &(map.vertexes[offset].position));

//            glClientActiveTextureARB(GL_TEXTURE0_ARB);
//            glTexCoordPointer(2, GL_FLOAT, stride, &(map.vertexes[offset].texcoord[0]));

//            glClientActiveTextureARB(GL_TEXTURE1_ARB);
//            glTexCoordPointer(2, GL_FLOAT, stride, &(map.vertexes[offset].texcoord[1]));

//            glDrawElements(GL_TRIANGLES, curFace.n_meshverts,
//               GL_UNSIGNED_INT, &map.meshverts[curFace.meshvert]);

//         }


        //glDisableClientState(GL_NORMAL_ARRAY);
        glDisableClientState(GL_VERTEX_ARRAY);
        /* Swap front and back buffers and process events */
        glfwSwapBuffers();

        while (0 < key_events.size()) {
            const key_event& event = key_events.front();
            const int key = std::get<0>(event);
            const int action = std::get<1>(event);
            //const auto view_direction = glm::normalize(camera_look_at - camera);

            const auto& key_to_movement_iterator = keybindings.find(key);

            if (std::end(keybindings) != key_to_movement_iterator) {
                const auto& movement = key_to_movement_iterator->second;

                if (GLFW_PRESS == action) {
                    active_keys.insert(movement);
                } else if (GLFW_RELEASE == action) {
                    active_keys.erase(movement);
                }
            }

            key_events.pop();
        }

        glm::vec3 temp_translation(0.0f);
        glm::quat temp_rotation(1.0f, glm::vec3(0.0f));

        for (const auto& movement: active_keys) {
            const std::function<actuator(const camera&)>& func = keybinding_actions[movement];

            if (func) {
                actuator act = func(cam);

                temp_translation += std::get<0>(act);
                temp_rotation = temp_rotation * std::get<1>(act);
            }
        }

        temp_rotation = glm::normalize(temp_rotation);

        cam.translational_velocity += temp_translation;
        cam.rotational_velocity = glm::quat_cast(glm::mat4_cast(cam.rotational_velocity) + glm::mat4_cast(temp_rotation));

        cam.eye += cam.translational_velocity * 0.001f;
        //cam.rotation = glm::quat_cast(glm::mat4_cast(cam.rotation) + glm::mat4_cast(temp_rotation) * 0.01f);
        cam.rotation = glm::normalize(cam.rotation);

        std::cout << cam.rotation.x << " " << cam.rotation.y << " " << cam.rotation.z << " " << cam.rotation.w << std::endl;

        cam.translational_velocity *= 0.9f;
        cam.rotational_velocity *= 0.1f;

        assert(0 == key_events.size());
    }

    glfwTerminate();

    return 0;
}
