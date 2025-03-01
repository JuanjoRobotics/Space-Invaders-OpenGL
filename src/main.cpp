#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <debug.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <stb_image.h>

#include <shader.h>
#include <camera.h>
#include <model.h>
#include <player.h>
#include <alien.h>
#include <light.h>
#include <textRenderer.h>

#include <iostream>
#include <cstdlib>
#include <ctime>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window, Player& player);
GLFWwindow* createWindow();

// settings
const unsigned int SCR_WIDTH = 1920;
const unsigned int SCR_HEIGHT = 1080;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

int main()
{
    // Seed the random number generator
    srand(static_cast<unsigned int>(time(0)));

    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    GLFWwindow* window = createWindow();
    if (window == NULL) {
        return -1;
    }

    // glfwSetCursorPosCallback(window, mouse_callback);
    // glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // tell stb_image.h to flip loaded texture's on the y-axis (before loading model).
    stbi_set_flip_vertically_on_load(true);

    glEnable(GL_DEPTH_TEST);

    Shader ourShader("shaders/model_simple.vs", "shaders/model_simple.fs");
    // Shader ourShader("shaders/model_lighting.vs", "shaders/model_lighting.fs");

    Player& player = Player::getInstance("resources/objects/boss-invader/source/master_invader.obj");
    unsigned int nAliens = 3;
    std::vector<Alien> aliens = Alien::generate(nAliens);

    // text rendering
    TextRenderer textRenderer(SCR_WIDTH, SCR_HEIGHT);

    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        // --------------------
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        // -----
        processInput(window, player);

        // render
        // ------
        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        ourShader.use();

        // view/projection transformations
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        ourShader.setMat4("projection", projection);
        ourShader.setMat4("view", view);

        if (aliens.size() < nAliens + 2) {
            Alien::addAlien(aliens);
        }
        glm::mat4 model = glm::mat4(1.0f);
        float relPos = -2.0f;
        for (auto& alien : aliens) {
            model = glm::mat4(1.0f);
            alien.Position.x = relPos;
            alien.Position.y = 0.5f;
            relPos += 0.6f;
            model = glm::translate(model, alien.Position);
            model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
            ourShader.setMat4("model", model);
            alien.Draw(ourShader);
        }

        // Draw player (boss)
        model = glm::mat4(1.0f);
        model = glm::translate(model, player.Position);
        model = glm::scale(model, glm::vec3(0.5f, 0.5f, 1.0f));
        ourShader.setMat4("model", model);
        player.Draw(ourShader);

        textRenderer.renderText("This is sample text", 25.0f, 25.0f, 1.0f, glm::vec3(0.5, 0.8f, 0.2f));
        textRenderer.renderText("(C) LearnOpenGL.com", 1200.0f, 1000.0f, 0.7f, glm::vec3(0.3, 0.7f, 0.9f));

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

GLFWwindow* createWindow()
{
    const GLFWvidmode* mode;
    int count;
    GLFWmonitor** monitors = glfwGetMonitors(&count);
    GLFWmonitor* monitor;
    if (count > 2) { // lets avoid the second monitor for now
        mode = glfwGetVideoMode(monitor = monitors[1]);
        DEBUG("Multiple monitors detected! Using secondary monitor...");
    }
    else {
        mode = glfwGetVideoMode(monitor = glfwGetPrimaryMonitor());
        DEBUG("Single monitor detected!");
    }

    // Calculate the position to center the window
    int windowPosX = (mode->width - SCR_WIDTH) / 2;
    int windowPosY = (mode->height - SCR_HEIGHT) / 2;

    // Create the window
    GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL); // change to monitor to use the secondary

    // Center the window
    glfwSetWindowPos(window, windowPosX, windowPosY);

    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return NULL;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    return window;
}
// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window, Player& player)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    using Direction = Player::Direction;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        player.processKeyboard(Direction::UP, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        player.processKeyboard(Direction::DOWN, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        player.processKeyboard(Direction::LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        player.processKeyboard(Direction::RIGHT, deltaTime);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}