/* Exercises:
 * 1. Make sure ONLY the happy face looks in the reverse direction
 *    by changing the fragment shader
 * 2. Experiment with different texture wrapping methods by 
 *    specifying tex coords in the range [0,2] instead of [0,1]
 *    - see if you can display 4 smiley faces on a single container
 *      image clamped at its edge
 *    - experiment with other wrapping methods as well
 * 3. Display only the center pixels of the texture image on the
 *    rectangle so the individual pixels get visible.
 * 4. Use a uniform variable as the mix function's third parameter
 *    to vary the amount the two textures are visible.
 *    Use up and down arrow keys to change how much the container
 *    or the smiley face is visible
 */
#include <iostream>
#include <cmath>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stb_image.h>

#include "shader.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow * w, int k, int scancode, int action, int mods);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

Shader * GLOBAL_SHADER = NULL;
float GLOBAL_K_VAL = 0.5;

int load_image(const char * path, 
    unsigned int * out_gl_tex_id, 
    unsigned int input_channels=GL_RGB,
    bool flip_vertical=false,
    unsigned int s_tex_wrap = GL_REPEAT,
    unsigned int t_tex_wrap = GL_REPEAT,
    unsigned int tex_min_filter = GL_LINEAR,
    unsigned int tex_mag_filter = GL_LINEAR
) {
    int width, height, channels;

    // flip vertically if necessary
    stbi_set_flip_vertically_on_load(flip_vertical);

    unsigned char * data = stbi_load(
        path, 
        &width, &height, &channels,
        0 // unused param for desired number of channels
    );

    if (data) {
        // TODO: figure out how error handling in opengl 
        // is supposed to work
        unsigned int gl_tex_id;
        glGenTextures(1, &gl_tex_id);
        glBindTexture(GL_TEXTURE_2D, gl_tex_id);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, s_tex_wrap);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, t_tex_wrap);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, tex_min_filter);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, tex_mag_filter);

        glTexImage2D(GL_TEXTURE_2D, 
            0, //mipmap level
            GL_RGB, // storage format
            width, height, // width and height of the image
            0, // legacy?
            input_channels, GL_UNSIGNED_BYTE, data //input format and data
        );
        glGenerateMipmap(GL_TEXTURE_2D);

        stbi_image_free(data);

        *out_gl_tex_id = gl_tex_id;
        return 0;
    } else {
        std::cout << "Failed to load texture" << std::endl;
        return 1;
    }
}

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, key_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    Shader ourShader("gs-matrices.vs", "gs-matrices.fs");

    GLOBAL_SHADER = &ourShader;

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float vertices[] = {
        // positions          // colors           // texture coords
         0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
         0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
        -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
    };
    unsigned int indices[] = {  // note that we start from 0!
        0, 1, 3,   // first triangle
        1, 2, 3    // second triangle
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
    unsigned int VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // the last parameter fo glBufferData is a enum for usage, to hint the
    // driver about where to store the buffer:
    // GL_STREAM_DRAW, GL_STREAM_READ, GL_STREAM_COPY, GL_STATIC_DRAW, 
    // GL_STATIC_READ, GL_STATIC_COPY, GL_DYNAMIC_DRAW, GL_DYNAMIC_READ, or GL_DYNAMIC_COPY
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    size_t offset = 0;
    unsigned int stride = (3 + 3 + 2) * sizeof(float);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*) offset);
    glEnableVertexAttribArray(0);
    offset += 3 * sizeof(float);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*) offset);
    glEnableVertexAttribArray(1);
    offset += 3 * sizeof(float);

    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, (void *) offset);
    glEnableVertexAttribArray(2);
    offset += 2 * sizeof(float);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    // glBindVertexArray(0);


    // bind the VAO (it was already bound, but just to demonstrate): seeing as we only have a single VAO we can 
    // just bind it beforehand before rendering the respective triangle; this is another approach.
    glBindVertexArray(VAO);

    unsigned int door_tex_id, door2_tex_id, face_tex_id;
    if (load_image("textures/container.jpg", &door_tex_id, 
        GL_RGB, false, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE
    )){
        return -1;
    }

    if (load_image("textures/container.jpg", &door2_tex_id, 
        GL_RGB, false, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE, GL_NEAREST, GL_NEAREST
    )){
        return -1;
    }

    if (load_image("textures/awesomeface.png", &face_tex_id, GL_RGBA, true)){
        return -1;
    }

    // be sure to activate the shader before any calls to glUniform
    // (TODO: what happens if we don't?)
    ourShader.use();
    ourShader.setInt("texture1", 0);
    ourShader.setInt("texture2", 1);
    ourShader.setInt("texture3", 2);

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        processInput(window);
        ourShader.setFloat("t", glfwGetTime());
        ourShader.setFloat("k_val", GLOBAL_K_VAL);

        // render
        // ------
        // I think the clear color is the background color
        // here, we will change this from .2 .3 .3 to .0 .0 .0
        // to see what all black looks like
        glClearColor(.0f, .0f, .0f, 1.0f);

        // This clears the buffers indicated with a bitmask
        // buffers are COLOR, DEPTH, ACCUM, STENCIL
        glClear(GL_COLOR_BUFFER_BIT);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, door_tex_id);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, face_tex_id);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, door2_tex_id);

        // render the rectangle
        glBindVertexArray(VAO);

        {
            // Make a transformation matrix for our vertex shader
            // This one rotates around the center
            glm::mat4 trans = glm::mat4(1.0f);
            trans = glm::translate(trans, glm::vec3(.5, -.5, 0.));
            trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(.0,.0,1.));
            glUniformMatrix4fv(
                glGetUniformLocation(ourShader.ID, "transform"),
                1,
                GL_FALSE,
                glm::value_ptr(trans)
            );
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        }

        {
            glm::mat4 trans = glm::mat4(1.0f);
            trans = glm::translate(trans, glm::vec3(-.5, .5, 0.));
            float s = sin(glfwGetTime());
            trans = glm::scale(trans, glm::vec3(s,s,s));
            glUniformMatrix4fv(
                glGetUniformLocation(ourShader.ID, "transform"),
                1,
                GL_FALSE,
                &trans[0][0]
            );
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        }

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

#define MAX(a, b) ((a > b) ? (a) : (b))
#define MIN(a, b) ((a < b) ? (a) : (b))

void key_callback(GLFWwindow * w, int k, int scancode, int action, int mods) {
    if (action != GLFW_PRESS) return;

    switch (k) {
        case GLFW_KEY_UP:
            GLOBAL_K_VAL = MIN(GLOBAL_K_VAL+.125, 1.0);
            break;
        case GLFW_KEY_DOWN:
            GLOBAL_K_VAL = MAX(GLOBAL_K_VAL-.125, 0.0);
            break;
    }
    std::cout << GLOBAL_K_VAL << std::endl;
}
