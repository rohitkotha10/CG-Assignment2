#include "importer.h"
#include "mesh.h"

using namespace std;
using namespace CG;

/**
 * @brief so that cursor operates properly
 */
bool firstMouse = true;
int press = 0;

int screen_width = 1280;
int screen_height = 720;
bool isFullScreen = false;
float screen_aspect = (float)screen_width / (float)screen_height;
/**
 * @brief for zooming in and out
 */
float fov = 45.0f;

/**
 * @brief set cursor x position in center
 */
float lastX = screen_width / 2.0f;
/**
 * @brief set cursor y position in center
 */
float lastY = screen_height / 2.0f;

/**
 * @brief initial pitch of camera
 */
float pitch = 0;
/**
 * @brief initial yaw of camera
 */
float yaw = -90;

/**
 * @brief time between frames
 */
float deltaTime;
/**
 * @brief store last frame time for computing delta time
 */
float lastFrame;

glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
/**
 * @brief camera position in world
 */
glm::vec3 cameraPos = glm::vec3(0.0f, 5.0f, 20.0f);
/**
 * @brief must change yaw and pitch accordingly when setting the camera target
 */
glm::vec3 cameraTarget = glm::vec3(0.0f, 5.0f, 0.0f);
/**
 * @brief the front direction of camera
 * the camera is in left hand coordinate sytem
 * this is because increasing z value should move you closer to the world
 */
glm::vec3 cameraFront = glm::normalize(cameraTarget - cameraPos);
/**
 * @brief right direction of camera
 */
glm::vec3 cameraRight = glm::normalize(glm::cross(cameraFront, worldUp));
/**
 * @brief up direction of camera
 */
glm::vec3 cameraUp = glm::normalize(glm::cross(cameraFront, cameraRight));

/**
 * @brief updated the values whenever they are pressed
 * @param window
 * @param button
 * @param action
 * @param mods
 */
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) press = 1;
}

/**
 * @brief change viewport when window resizes
 * @param window
 * @param width
 * @param height
 */
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
};

/**
 * @brief closes the window when escape is pressed
 * @param window
 * @param key
 * @param scancode
 * @param action
 * @param mods
 */
void keyboard_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) glfwSetWindowShouldClose(window, true);
};

/**
 * @brief simulate the yaw and pitches here
 * @param window
 * @param xdpos
 * @param ydpos
 */
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

    // mouse moving horizontally gives you yaw
    // mouse moving vertically gives you pitch
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

/**
 * @brief simulate the zoom function here by changing fov
 * @param window
 * @param xoffset
 * @param yoffset
 */
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

    /**
     * @brief the shaders for my scene
     */
    Program myProgram;
    Importer man;
    Mesh myPlane;
    Importer door;
    Importer table;

    // shader compilation
    myProgram.create("src/shaders/vs.shader", "src/shaders/fs.shader");

    // startup processes
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, keyboard_callback);
    glfwSetCursorPosCallback(window, cursor_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    door.loadModel("media/door/door.obj");
    myPlane.createPlaneColor(glm::vec3(0.0f, -0.5f, 0.0f), 50.0f, glm::vec4(0.6f, 0.5f, 0.3f, 1.0f));
    man.loadModel("media/man/man.obj");
    table.loadModel("media/table/table.obj");
    // render loop
    while (!glfwWindowShouldClose(window)) {
        float currentTime = (float)glfwGetTime();
        glEnable(GL_DEPTH_TEST);
        glClearColor(0.9f, 0.8f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        float currentFrame = (float)currentTime;
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        myProgram.use();
        glm::mat4 projection = glm::perspective(glm::radians(fov), screen_aspect, 0.1f, 100.0f);
        glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, worldUp);

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::scale(model, glm::vec3(0.3f));
        model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));

        myProgram.setMat4("projection_matrix", projection);
        myProgram.setMat4("view_matrix", view);
        myProgram.setMat4("model_matrix", model);

        myProgram.setVec3("lightPos1", glm::vec3(4.0f, 5.0f, -10.0f));
        myProgram.setVec3("lightPos2", glm::vec3(-5.0f, 5.0f, 10.0f));
        myProgram.setVec3("viewPos", glm::vec3(cameraPos));

        door.draw(myProgram);

        model = glm::scale(glm::mat4(1.0f), glm::vec3(0.2f));
        model = glm::rotate(model, 3.14f, glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::translate(model, glm::vec3(5.0f, 1.0f, -10.0f));
        myProgram.setMat4("model_matrix", model);

        man.draw(myProgram);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(3.0f, -0.5f, 6.0f));
        myProgram.setMat4("model_matrix", model);

        table.draw(myProgram);

        model = glm::mat4(1.0f);
        myProgram.setMat4("model_matrix", model);

        myPlane.draw(myProgram);
        // process inputs
        const float cameraSpeed = 3.5f * deltaTime;  // adjust accordingly

        // simulate pan here
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) cameraPos += cameraSpeed * cameraFront;
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) cameraPos -= cameraSpeed * cameraFront;
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) cameraPos -= cameraSpeed * cameraRight;
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) cameraPos += cameraSpeed * cameraRight;
        if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) cameraPos -= cameraSpeed * cameraUp;
        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) cameraPos += cameraSpeed * cameraUp;

        // simulate roll, q and e
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
    myPlane.shutdown();
    door.shutdown();
    man.shutdown();
    table.shutdown();
    glfwDestroyWindow(window);
    glfwTerminate();
}
