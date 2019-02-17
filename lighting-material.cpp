#include <iostream>
#include <cmath>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"
#include "camera.h"

GLFWwindow * setup_GLFW_window();
void processInput(GLFWwindow *window); // updates camera global position

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
// +x = right
// +y = up
// +z = out of the window
Camera camera(glm::vec3(1.23f, 1.45f, 3.16f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// lighting
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

// returns NULL on failure, otherwise returns window pointer
int main()
{
    // Point the camera to down and slightly left
    camera.Pitch -= 23.2f;
    camera.Yaw  -= 11.8f;
    camera.updateCameraVectors();

	GLFWwindow * window = setup_GLFW_window();

    Shader lightingShader("lighting-material.vs", "lighting-material.fs");
    Shader lampShader("projection-model-view.vs", "lighting-colors-lamp.fs");

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------

    /* cube: */
    // pos.x, pos.y, pos.z, norm.x, norm.y, norm.z
    float vertices[] = {
        // back face
        -0.5f, -0.5f, -0.5f, 0.f, 0.f, -1.f,
         0.5f, -0.5f, -0.5f, 0.f, 0.f, -1.f,
         0.5f,  0.5f, -0.5f, 0.f, 0.f, -1.f,
         0.5f,  0.5f, -0.5f, 0.f, 0.f, -1.f,
        -0.5f,  0.5f, -0.5f, 0.f, 0.f, -1.f,
        -0.5f, -0.5f, -0.5f, 0.f, 0.f, -1.f,

        // front face
        -0.5f, -0.5f,  0.5f, 0.f, 0.f, 1.f,
         0.5f, -0.5f,  0.5f, 0.f, 0.f, 1.f,
         0.5f,  0.5f,  0.5f, 0.f, 0.f, 1.f,
         0.5f,  0.5f,  0.5f, 0.f, 0.f, 1.f,
        -0.5f,  0.5f,  0.5f, 0.f, 0.f, 1.f,
        -0.5f, -0.5f,  0.5f, 0.f, 0.f, 1.f,

        // left face
        -0.5f,  0.5f,  0.5f, -1.f, 0.f, 0.f,
        -0.5f,  0.5f, -0.5f, -1.f, 0.f, 0.f,
        -0.5f, -0.5f, -0.5f, -1.f, 0.f, 0.f,
        -0.5f, -0.5f, -0.5f, -1.f, 0.f, 0.f,
        -0.5f, -0.5f,  0.5f, -1.f, 0.f, 0.f,
        -0.5f,  0.5f,  0.5f, -1.f, 0.f, 0.f,

        // right face
         0.5f,  0.5f,  0.5f, 1.f, 0.f, 0.f,
         0.5f,  0.5f, -0.5f, 1.f, 0.f, 0.f,
         0.5f, -0.5f, -0.5f, 1.f, 0.f, 0.f,
         0.5f, -0.5f, -0.5f, 1.f, 0.f, 0.f,
         0.5f, -0.5f,  0.5f, 1.f, 0.f, 0.f,
         0.5f,  0.5f,  0.5f, 1.f, 0.f, 0.f,

        // bottom face
        -0.5f, -0.5f, -0.5f, 0.f, -1.f, 0.f, 
         0.5f, -0.5f, -0.5f, 0.f, -1.f, 0.f,
         0.5f, -0.5f,  0.5f, 0.f, -1.f, 0.f,
         0.5f, -0.5f,  0.5f, 0.f, -1.f, 0.f,
        -0.5f, -0.5f,  0.5f, 0.f, -1.f, 0.f,
        -0.5f, -0.5f, -0.5f, 0.f, -1.f, 0.f,

        // top face
        -0.5f,  0.5f, -0.5f, 0.f, 1.f, 0.f,
         0.5f,  0.5f, -0.5f, 0.f, 1.f, 0.f,
         0.5f,  0.5f,  0.5f, 0.f, 1.f, 0.f,
         0.5f,  0.5f,  0.5f, 0.f, 1.f, 0.f,
        -0.5f,  0.5f,  0.5f, 0.f, 1.f, 0.f,
        -0.5f,  0.5f, -0.5f, 0.f, 1.f, 0.f,
    };

    // What's the relation between the following three things:
    // Vertex Buffer Object (VBO)
    // Vertex Array Object (VAO)
    // Element Buffer Object (EBO)
    //
    // it seems like it's something like
    // VAO:
    //   has 1-16 attribute pointers, each having type, offset, stride
    //    * the attribute pointers point into the VBO, which is more like a
    //      direct data buffer
    //   has 0 or 1 element buffer objects, which contain indices
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	unsigned int cubeVAO;
    glGenVertexArrays(1, &cubeVAO);
	glBindVertexArray(cubeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO); //already bound, but demonstrating complete usage
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);

	unsigned int lightVAO;
    glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO); //already bound, but demonstrating complete usage
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

    
// for the below to work, need to have the same name in C++ and the shader
#define SEND_MAT4(mat_name) ourShader.setMat4(#mat_name, mat_name)

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
		// per-frame time logic
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		
        // input
        // -----
		processInput(window);



        // render
        // ------
        // I think the clear color is the background color
        glClearColor(.1f, .1f, .1f, 1.0f);

        // This clears the buffers indicated with a bitmask
        // buffers are COLOR, DEPTH, ACCUM, STENCIL
        // the Depth buffer stores how far away each pixel is
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        float t = glfwGetTime();
        lightPos.x = 1.f + sin(t) * 2.f;
        lightPos.y = sin(t / 2.f);

		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, .1f, 100.f);
		glm::mat4 view = camera.GetViewMatrix();

		// Render the cube:
		// ------------------
		
		// calculate cube model matrix
		glm::mat4 cube_model = glm::mat4(1.f);

		// activate lighting shader
		lightingShader.use();
		lightingShader.setVec3("objectColor", 1.f, .5f, .31f);
		lightingShader.setVec3("lightColor", 1.f, 1.f, 1.f);
        lightingShader.setVec3("lightPos", lightPos);
        lightingShader.setVec3("viewPos", camera.Position);
		lightingShader.setMat4("projection", projection);
		lightingShader.setMat4("view", view);
		lightingShader.setMat4("model", cube_model);

		// render cube
		glBindVertexArray(cubeVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// Render the lamp:
		// ------------------

		// calculate the lamp model matrix
		glm::mat4 lamp_model = glm::mat4(1.f);
		lamp_model = glm::translate(lamp_model, lightPos);
		lamp_model = glm::scale(lamp_model, glm::vec3(.2f)); // smaller cube
		
		// active lamp shader
		lampShader.use();
		lampShader.setMat4("projection", projection);
		lampShader.setMat4("view", view);
		lampShader.setMat4("model", lamp_model);
		glBindVertexArray(lightVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);


        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteVertexArrays(1, &lightVAO);
    glDeleteBuffers(1, &VBO);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        camera.ProcessKeyboard(UP, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        camera.ProcessKeyboard(DOWN, deltaTime);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow * w, double xpos, double ypos) {
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

void scroll_callback(GLFWwindow * w, double x_off, double y_off) {
	camera.ProcessMouseScroll(y_off);
}

GLFWwindow * setup_GLFW_window() {
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL: Lighting: Colors", NULL, NULL);

    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
		window = NULL;
		goto Exit;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // capture mouse so that it won't leave window (ESC to leave)
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
		window = NULL;
		goto Exit;
    }

    // GL mode enabling:
    glEnable(GL_DEPTH_TEST); // this makes OpenGL do depth testing
        // which makes nearer faces overwrite further ones, but not vice-versa
Exit:
	if (NULL == window) {
		glfwTerminate();
	}
	return window;
}
