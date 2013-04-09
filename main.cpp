#include <iostream>
#include <GL/glfw3.h>

using namespace std;

int main()
{
    cout << "Hello World!" << endl;

    if (!glfwInit())
        return -1;

    return 0;
}

