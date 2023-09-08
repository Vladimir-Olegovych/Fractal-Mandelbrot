#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>

void display();
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

class Complex
{
public:
    double re;
    double im;
    Complex(double are, double aim) : re(are), im(aim) {}
    Complex operator+(const Complex& v)
    {
        return Complex(re + v.re, im + v.im);
    }
};

Complex sqr(const Complex& v)
{
    return Complex(v.re * v.re - v.im * v.im, 2 * v.re * v.im);
}

double abs(const Complex& v) 
{
    return sqrt(v.re * v.re + v.im * v.im);
}
//constants
const short width = 600;
const short height = 600;
const short speed = 30;

int iterations = 1000;
int zoom = 200;

int pos_x = 450;
int pos_y = 300;

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(width, height, "Fractal Mandelbrot", NULL, NULL);
    glfwSetKeyCallback(window, key_callback);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        std::cout << 1 << std::endl;

        glLoadIdentity();
        glOrtho(0, width, height, 0, -1, 1);
        /* Render here */
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        display();

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        exit(0);
    }
    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
    {
        zoom += 100;
    }

    if (key == GLFW_KEY_W && action == GLFW_PRESS)
    {
        pos_y += speed;
    }

    if (key == GLFW_KEY_A && action == GLFW_PRESS)
    {
        pos_x += speed;
    }

    if (key == GLFW_KEY_S && action == GLFW_PRESS)
    {
        pos_y -= speed;
    }

    if (key == GLFW_KEY_D && action == GLFW_PRESS)
    {
        pos_x -= speed;
    }
}

void display() 
{
    int progress = 0;
    glBegin(GL_POINTS);
    for (int y = 0; y < 600; ++y)
        for (int x = 0; x < 600; ++x)
        {
            Complex z(0, 0);
            int i = 0;
            while (i < iterations && abs(z) < 2)
            {
                progress++;
                z = sqr(z) + Complex(1.0 * (x - pos_x) / zoom, 1.0 * (y - pos_y) / zoom);
                ++i;
            }
            if (abs(z) >= 2)
            {
                float col = (50.0 - i) / 50.0;
                glColor3f(col, col, col);
                glVertex2f(x, y);
            }
        }
    glEnd();
}