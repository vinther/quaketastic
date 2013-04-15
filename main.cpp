#include <iostream>
#include <GL/glfw.h>

#include <chrono>

using namespace std;

void draw_triangle()
{
    glBegin(GL_TRIANGLES);                      // Drawing Using Triangles
        glVertex3f( 0.0f, 1.0f, 0.0f);              // Top
        glVertex3f(-1.0f,-1.0f, 0.0f);              // Bottom Left
        glVertex3f( 1.0f,-1.0f, 0.0f);              // Bottom Right
    glEnd();
}

void mouse_pos_callback(int x, int y) {

}

int main()
{
    /* Initialize the library */
    if (!glfwInit())
        return -1;

    const int window_instance = glfwOpenWindow(640, 480, 8, 8, 8, 0, 24, 0, GLFW_WINDOW);

    /* Create a windowed mode window and its OpenGL context */
    if (!window_instance)
        return -1;

    auto start = std::chrono::system_clock::now();

    glfwSetMousePosCallback(mouse_pos_callback);

    /* Loop until the user closes the window */
    while (glfwGetWindowParam(GLFW_OPENED))
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);     // Clear The Screen And The Depth Buffer
        glLoadIdentity();

        gluPerspective(45, 1.333, 0, 100);

        glTranslatef(-1.5f,0.0f,-6.0f);
        glRotatef(std::chrono::duration_cast<std::chrono::milliseconds>
                  (std::chrono::system_clock::now()-start).count(), 0.0f, 0.0f, 1.0f);

        draw_triangle();

        /* Swap front and back buffers and process events */
        glfwSwapBuffers();


    }

    glfwTerminate();

    return 0;
}

