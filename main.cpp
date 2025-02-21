#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cmath>
#include <vector>
#include <iostream>

// Car structure to store position and color
struct Car
{
    float x;       // X position
    float r, g, b; // Color (RGB)
};

// Cars
std::vector<Car> cars = {
    {-1.5f, 0.0f, 0.0f, 1.0f} // Blue car
};

// Car speed
float carSpeed = 0.01f;

// Traffic light states
enum LightState
{
    RED,
    YELLOW,
    GREEN
};
LightState currentState = RED;

// Traffic light timer
int lightTimer = 0;
const int RED_TIME = 200, YELLOW_TIME = 50, GREEN_TIME = 200;

// Traffic light and zebra crossing positions
const float TRAFFIC_LIGHT_X = 0.7f;
const float ZEBRA_CROSSING_X = TRAFFIC_LIGHT_X - 0.3f;
const float STOP_LINE = ZEBRA_CROSSING_X - 0.2f;

// Function to draw a rectangle
void drawRectangle(float x1, float y1, float x2, float y2, float r, float g, float b)
{
    glColor3f(r, g, b);
    glBegin(GL_QUADS);
    glVertex2f(x1, y1);
    glVertex2f(x2, y1);
    glVertex2f(x2, y2);
    glVertex2f(x1, y2);
    glEnd();
}

// Function to draw a circle
void drawCircle(float cx, float cy, float r, int numSegments, float red, float green, float blue)
{
    glColor3f(red, green, blue);
    glBegin(GL_TRIANGLE_FAN);
    for (int i = 0; i <= numSegments; i++)
    {
        float angle = i * 2.0f * M_PI / numSegments;
        glVertex2f(cx + r * cos(angle), cy + r * sin(angle));
    }
    glEnd();
}

// Function to draw the road
void drawRoad()
{
    drawRectangle(-2.5f, -0.5f, 1.5f, 0.5f, 0.3f, 0.3f, 0.3f);

    for (float i = -2.4f; i < 1.5f; i += 0.2f)
    {
        drawRectangle(i, -0.02f, i + 0.1f, 0.02f, 1.0f, 1.0f, 1.0f);
    }

    drawRectangle(-2.5f, 0.5f, 1.5f, 0.6f, 0.6f, 0.6f, 0.6f);
    drawRectangle(-2.5f, -0.5f, 1.5f, -0.6f, 0.6f, 0.6f, 0.6f);
}

// Function to draw a car
void drawCar(float x, float y, float r, float g, float b)
{
    drawRectangle(x - 0.2f, y - 0.1f, x + 0.2f, y + 0.1f, r, g, b);
    drawRectangle(x - 0.15f, y + 0.05f, x + 0.15f, y + 0.12f, r * 0.8f, g * 0.8f, b * 0.8f);
}

// Function to draw a traffic light
void drawTrafficLight(float x, float y, LightState state)
{
    drawRectangle(x - 0.05f, y - 0.15f, x + 0.05f, y + 0.15f, 0.2f, 0.2f, 0.2f);
    drawCircle(x, y + 0.1f, 0.03f, 20, state == RED ? 1.0f : 0.3f, 0.0f, 0.0f);
    drawCircle(x, y, 0.03f, 20, state == YELLOW ? 1.0f : 0.3f, state == YELLOW ? 1.0f : 0.3f, 0.0f);
    drawCircle(x, y - 0.1f, 0.03f, 20, 0.0f, state == GREEN ? 1.0f : 0.3f, 0.0f);
}

// Update function for animation
void update()
{
    lightTimer++;
    if (currentState == RED && lightTimer > RED_TIME)
    {
        currentState = GREEN;
        lightTimer = 0;
    }
    else if (currentState == GREEN && lightTimer > GREEN_TIME)
    {
        currentState = YELLOW;
        lightTimer = 0;
    }
    else if (currentState == YELLOW && lightTimer > YELLOW_TIME)
    {
        currentState = RED;
        lightTimer = 0;
    }

    if (currentState == GREEN)
    {
        for (auto &car : cars)
        {
            car.x += carSpeed;
        }
    }
    else if (currentState == RED)
    {
        for (auto &car : cars)
        {
            if (car.x >= STOP_LINE - 0.1f && car.x <= STOP_LINE + 0.1f)
            {
                car.x = STOP_LINE - 0.1f;
            }
        }
    }

    for (auto &car : cars)
    {
        if (car.x > 1.5f)
        {
            car.x = -2.0f;
        }
    }
}

// Render function
void render()
{
    glClear(GL_COLOR_BUFFER_BIT);

    drawRoad();

    for (const auto &car : cars)
    {
        drawCar(car.x, -0.05f, car.r, car.g, car.b);
    }

    drawTrafficLight(TRAFFIC_LIGHT_X, 0.4f, currentState);

    glfwSwapBuffers(glfwGetCurrentContext());
}

// Keyboard input function
void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_Q && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
}

// Main function
int main()
{
    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW\n";
        return -1;
    }

    GLFWwindow *window = glfwCreateWindow(800, 600, "Cars Stop at Zebra Crossing", nullptr, nullptr);
    if (!window)
    {
        std::cerr << "Failed to create window\n";
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, keyCallback);

    while (!glfwWindowShouldClose(window))
    {
        update();
        render();
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
