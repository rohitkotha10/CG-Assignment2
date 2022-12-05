#include "importer.h"
#include "mesh.h"

using namespace std;
using namespace CG;

bool firstMouse = true;
int press = 0;

int screen_width = 1280;
int screen_height = 720;
bool isFullScreen = false;
float screen_aspect = (float)screen_width / (float)screen_height;
float fov = 45.0f;

float lastX = screen_width / 2.0f;
float lastY = screen_height / 2.0f;

float pitch = 0;
float yaw = -90;

float deltaTime;
float lastFrame;

glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 cameraFront = glm::normalize(cameraTarget - cameraPos);          // also facing the scene
glm::vec3 cameraRight = glm::normalize(glm::cross(cameraFront, worldUp));  // camera is in left hand system
glm::vec3 cameraUp = glm::normalize(glm::cross(cameraFront, cameraRight));

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) press = 1;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
};

void keyboard_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) glfwSetWindowShouldClose(window, true);
};

void cursor_callback(GLFWwindow* window, double xdpos, double ydpos) {
    float xpos = (float)xdpos;
    float ypos = (float)ydpos;

    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    if (pitch > 89.0f) pitch = 89.0f;
    if (pitch < -89.0f) pitch = -89.0f;

    glm::vec3 direction;

    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

    cameraFront = glm::normalize(direction);
    cameraRight = glm::normalize(glm::cross(cameraFront, worldUp));
    cameraUp = glm::normalize(glm::cross(cameraRight, cameraFront));
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    fov -= (float)yoffset;
    if (fov < 1.0f) fov = 1.0f;
    if (fov > 45.0f) fov = 45.0f;
}

int main() {
    GLFWwindow* window = NULL;

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, false);

    window = glfwCreateWindow(
        screen_width, screen_height, "CG Assignment", isFullScreen ? glfwGetPrimaryMonitor() : NULL, NULL);

    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    std::cout << glGetString(GL_VENDOR) << std::endl;
    std::cout << glGetString(GL_VERSION) << std::endl;
    std::cout << glGetString(GL_RENDERER) << std::endl << std::endl;

    Program myProgram;
    Importer ourModel;

    // shader compilation
    myProgram.create("src/vs.shader", "src/fs.shader");

    // startup processes
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, keyboard_callback);
    glfwSetCursorPosCallback(window, cursor_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    ourModel.loadModel("media/table/table.obj");

    // render loop
    while (!glfwWindowShouldClose(window)) {
        float currentTime = (float)glfwGetTime();
        glEnable(GL_DEPTH_TEST);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        float currentFrame = (float)currentTime;
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        myProgram.use();
        glDepthFunc(GL_LESS);
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::scale(model, glm::vec3(0.3f));
        glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, worldUp);
        glm::mat4 projection = glm::perspective(glm::radians(fov), screen_aspect, 0.1f, 100.0f);
        myProgram.setMat4("view_matrix", view);
        myProgram.setMat4("projection_matrix", projection);
        myProgram.setMat4("model_matrix", model);

        myProgram.setVec3("lightPos", glm::vec3(2.0f, 2.0f, 2.0f));
        myProgram.setVec3("viewPos", glm::vec3(cameraPos));

        ourModel.draw(myProgram);

        // process inputs
        const float cameraSpeed = 2.5f * deltaTime;  // adjust accordingly

        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) cameraPos += cameraSpeed * cameraFront;
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) cameraPos -= cameraSpeed * cameraFront;
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) cameraPos -= cameraSpeed * cameraRight;
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) cameraPos += cameraSpeed * cameraRight;
        if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) cameraPos -= cameraSpeed * cameraUp;
        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) cameraPos += cameraSpeed * cameraUp;

        // roll, q and e
        float rollSpeed = 50.0f * deltaTime;

        if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
            worldUp = glm::mat3(glm::rotate(glm::mat4(1.0f), -glm::radians(rollSpeed), cameraFront)) * cameraUp;
        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
            worldUp = glm::mat3(glm::rotate(glm::mat4(1.0f), glm::radians(rollSpeed), cameraFront)) * cameraUp;

        cameraRight = glm::normalize(glm::cross(cameraFront, worldUp));
        cameraUp = glm::normalize(glm::cross(cameraRight, cameraFront));

        glfwPollEvents();
        glfwSwapInterval(1);
        glfwSwapBuffers(window);
    }

    // shutdown
    myProgram.shutdown();
    ourModel.shutdown();
    glfwDestroyWindow(window);
    glfwTerminate();
}
