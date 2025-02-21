#include <GL/glut.h>
#include <cmath>
#include <vector>

// Car structure to store position and color
struct Car {
    float x; // X position
    float r, g, b; // Color (RGB)
};

// Cars
std::vector<Car> cars = {
    {-1.5f, 0.0f, 0.0f, 1.0f} // Blue car
};

// Car speed
float carSpeed = 0.01f;

// Traffic light states
enum LightState { RED, YELLOW, GREEN };
LightState currentState = RED;

// Traffic light timer
int lightTimer = 0;
const int RED_TIME = 200;    // Time for red light
const int YELLOW_TIME = 50;  // Time for yellow light
const int GREEN_TIME = 200;  // Time for green light

// Traffic light position
const float TRAFFIC_LIGHT_X = 0.7f; // X position of the traffic light

// Zebra crossing position (just before the traffic light)
const float ZEBRA_CROSSING_X = TRAFFIC_LIGHT_X - 0.3f;

// Stopping point (just before the zebra crossing)
const float STOP_LINE = ZEBRA_CROSSING_X - 0.2f;

// Function to draw a rectangle
void drawRectangle(float x1, float y1, float x2, float y2, float r, float g, float b) {
    glColor3f(r, g, b);
    glBegin(GL_QUADS);
    glVertex2f(x1, y1);
    glVertex2f(x2, y1);
    glVertex2f(x2, y2);
    glVertex2f(x1, y2);
    glEnd();
}

// Function to draw a circle
void drawCircle(float cx, float cy, float r, int numSegments, float red, float green, float blue) {
    glColor3f(red, green, blue);
    glBegin(GL_TRIANGLE_FAN);
    for (int i = 0; i <= numSegments; i++) {
        float angle = i * 2.0f * 3.14159f / numSegments;
        glVertex2f(cx + r * cos(angle), cy + r * sin(angle));
    }
    glEnd();
}

// Function to draw the road
void drawRoad() {
    drawRectangle(-2.5f, -0.5f, 1.5f, 0.5f, 0.3f, 0.3f, 0.3f); // Road surface
    
    // Lane dividers (dashed white lines)
    for (float i = -2.4f; i < 1.5f; i += 0.2f) {
        drawRectangle(i, -0.02f, i + 0.1f, 0.02f, 1.0f, 1.0f, 1.0f);
    }
    
    // Sidewalks
    drawRectangle(-2.5f, 0.5f, 1.5f, 0.6f, 0.6f, 0.6f, 0.6f); // Top sidewalk
    drawRectangle(-2.5f, -0.5f, 1.5f, -0.6f, 0.6f, 0.6f, 0.6f); // Bottom sidewalk

    // Zebra crossing stripes (just before the traffic light)
    for (float i = -0.4f; i <= 0.4f; i += 0.2f) {
        drawRectangle(ZEBRA_CROSSING_X - 0.1f, i - 0.05f, ZEBRA_CROSSING_X + 0.1f, i + 0.05f, 1.0f, 1.0f, 1.0f);
    }
}

// Function to draw a car
void drawCar(float x, float y, float r, float g, float b) {
    drawRectangle(x - 0.2f, y - 0.1f, x + 0.2f, y + 0.1f, r, g, b); // Car body
    drawRectangle(x - 0.15f, y + 0.05f, x + 0.15f, y + 0.12f, r * 0.8f, g * 0.8f, b * 0.8f); // Car roof
    
    // Windows
    drawRectangle(x - 0.13f, y + 0.06f, x - 0.02f, y + 0.11f, 0.6f, 0.8f, 1.0f);
    drawRectangle(x + 0.02f, y + 0.06f, x + 0.13f, y + 0.11f, 0.6f, 0.8f, 1.0f);
    
    // Wheels
    drawCircle(x - 0.15f, y - 0.12f, 0.04f, 20, 0.0f, 0.0f, 0.0f);
    drawCircle(x + 0.15f, y - 0.12f, 0.04f, 20, 0.0f, 0.0f, 0.0f);
}

// Function to draw a tree
void drawTree(float x, float y) {
    // Trunk
    drawRectangle(x - 0.05f, y - 0.3f, x + 0.05f, y, 0.55f, 0.27f, 0.07f);
    // Leaves
    drawCircle(x, y + 0.1f, 0.15f, 20, 0.0f, 0.5f, 0.0f);
}

// Function to draw the traffic light
void drawTrafficLight(float x, float y, LightState state) {
    // Traffic light box
    drawRectangle(x - 0.05f, y - 0.15f, x + 0.05f, y + 0.15f, 0.2f, 0.2f, 0.2f);
    
    // Red light
    glColor3f(state == RED ? 1.0f : 0.3f, 0.0f, 0.0f);
    drawCircle(x, y + 0.1f, 0.03f, 20, state == RED ? 1.0f : 0.3f, 0.0f, 0.0f);
    
    // Yellow light
    glColor3f(state == YELLOW ? 1.0f : 0.3f, state == YELLOW ? 1.0f : 0.3f, 0.0f);
    drawCircle(x, y, 0.03f, 20, state == YELLOW ? 1.0f : 0.3f, state == YELLOW ? 1.0f : 0.3f, 0.0f);
    
    // Green light
    glColor3f(0.0f, state == GREEN ? 1.0f : 0.3f, 0.0f);
    drawCircle(x, y - 0.1f, 0.03f, 20, 0.0f, state == GREEN ? 1.0f : 0.3f, 0.0f);
}

// Display function
void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    
    drawRoad();
    
    // Draw cars
    for (const auto& car : cars) {
        drawCar(car.x, -0.05f, car.r, car.g, car.b);
    }
    
    // Draw trees
    drawTree(-1.2f, 0.6f);
    drawTree(1.2f, 0.6f);
    
    // Draw traffic light
    drawTrafficLight(TRAFFIC_LIGHT_X, 0.4f, currentState);
    
    glutSwapBuffers();
}

// Timer function for animation
void update(int value) {
    // Update traffic light state
    lightTimer++;
    if (currentState == RED && lightTimer > RED_TIME) {
        currentState = GREEN;
        lightTimer = 0;
    } else if (currentState == GREEN && lightTimer > GREEN_TIME) {
        currentState = YELLOW;
        lightTimer = 0;
    } else if (currentState == YELLOW && lightTimer > YELLOW_TIME) {
        currentState = RED;
        lightTimer = 0;
    }
    
    // Move cars only if the light is green or they are not at the stop line
    if (currentState == GREEN) {
        for (auto& car : cars) {
            car.x += carSpeed; // Move the car to the right
        }
    } else if (currentState == RED) {
        // Stop cars at the stop line (just before the zebra crossing)
        for (auto& car : cars) {
            if (car.x >= STOP_LINE - 0.1f && car.x <= STOP_LINE + 0.1f) {
                car.x = STOP_LINE - 0.1f; // Stop just before the zebra crossing
            }
        }
    }
    
    // Reset car positions when they go off the right side of the screen
    for (auto& car : cars) {
        if (car.x > 1.5f) {
            car.x = -2.0f; // Move car back to the left side
        }
    }
    
    glutPostRedisplay(); // Redraw the scene
    glutTimerFunc(16, update, 0); // 60 FPS (1000ms / 60 ≈ 16ms)
}

// Keyboard input function
void handleKeypress(unsigned char key, int x, int y) {
    if (key == 'q' || key == 'Q') {
        exit(0); // Quit the program
    }
}

// Main function
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Cars Stop at Zebra Crossing and Continue");
    
    glClearColor(0.53f, 0.81f, 0.92f, 1.0f); // Sky blue background
    gluOrtho2D(-2.5, 1.5, -1.0, 1.0); // Set coordinate system
    
    glutDisplayFunc(display);
    glutTimerFunc(0, update, 0); // Start animation timer
    glutKeyboardFunc(handleKeypress); // Enable keyboard input
    glutMainLoop();
    
    return 0;
}