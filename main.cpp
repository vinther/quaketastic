#include <iostream>

#include <GL/gl.h>
#include <GL/glfw.h>

#include <chrono>
#include <array>
#include <fstream>
#include <algorithm>

#include "bsp_map_loader.hpp"

using namespace std;

template<typename T, int size>
int GetArrLength(T(&)[size]){return size;}

GLfloat colors[] = {1.0f, 0.75f, 0.5f, 0.25f, 0.0f};
int choice = 0;

void draw_triangle()
{
    glBegin(GL_TRIANGLES);                      // Drawing Using Triangles
        glVertex3f( 0.0f, 1.0f, 0.0f);              // Top
        glVertex3f(-1.0f,-1.0f, 0.0f);              // Bottom Left
        glVertex3f( 1.0f,-1.0f, 0.0f);              // Bottom Right
    glEnd();
}

int main(int argc, char** argv)
{
    const std::string window_title = "Quaketastic";
    const std::string map_path = "maps/jof3dm2.bsp";
    const int window_width = 1024, window_height = 768;
    const int color_depth = 8;

    GLfloat camera_x = 0.0f, camera_y = 0.0f, camera_z = -1000.0f;

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

    glfwSetWindowTitle(window_title.c_str());

    auto start = std::chrono::system_clock::now();

//    for (auto& x: map.vertexes) {
//        std::cout << x.position[0] << " " << x.position[1] << " " << x.position[2] << std::endl;
//    }

    std::function<void(int,int)> key_callback = [&](int key, int state){
        switch (key) {
        case 'w':
            camera_y += 5.0f;
            break;
        case 'a':
            camera_y -= 5.0f;
            break;
        case 's':
            camera_x -= 5.0f;
            break;
        case 'd':
            camera_x += 5.0f;
            break;
        }

    };

    //glfwSetKeyCallback(key_callback);

    std::string s(map.entities.ents.get());

    std::cout << std::count_if (std::begin(map.faces), std::end(map.faces), [](bsp::face& face) { return face.type == 1; }) << std::endl;
    std::cout << std::count_if (std::begin(map.faces), std::end(map.faces), [](bsp::face& face) { return face.type == 2; }) << std::endl;
    std::cout << std::count_if (std::begin(map.faces), std::end(map.faces), [](bsp::face& face) { return face.type == 3; }) << std::endl;
    std::cout << std::count_if (std::begin(map.faces), std::end(map.faces), [](bsp::face& face) { return face.type == 4; }) << std::endl;

    /* Loop until the user closes the window */
    while (glfwGetWindowParam(GLFW_OPENED))
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);     // Clear The Screen And The Depth Buffer
        glLoadIdentity();

        gluPerspective(45, 1.333, 0, 5000000);

        glRotatef(std::chrono::duration_cast<std::chrono::milliseconds>
                  (std::chrono::system_clock::now()-start).count() / 50, 0.0f, 1.0f, 0.0f);
        glTranslatef(camera_x, camera_y, camera_z);


        glEnableClientState(GL_VERTEX_ARRAY);

        glVertexPointer(3, GL_FLOAT, sizeof(bsp::vertex), reinterpret_cast<GLvoid*>(map.vertexes.data()));
        glDrawArrays(GL_POINTS, 0, map.vertexes.size());

        glDisableClientState(GL_VERTEX_ARRAY);
        /* Swap front and back buffers and process events */
        glfwSwapBuffers();
    }

    glfwTerminate();

    return 0;
}

