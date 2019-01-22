/* Exercises:
 *
 * The easiest of these is 3 because they are all rotating right now
 * Done:
 * 1. experiment with FoV and aspect-ration parameters of 
 *    GLM's projection function
 * 2. Play with the view matrix to understand how it's like a camera object
 * 3. Make every 3rd container rotate over time (including the 1st)
 * DH 1. See what happens if we don't clear the depth buffer
 *   - the black background goes in front of the faces
 *     - i believe this is because the color is cleared to black, but the
 *       depth is still there from the previous cube render, so only parts
 *       of the animation that bring faces of the cube farther than they've
 *       been before show up as non-black
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
void key_callback(GLFWwindow * w, 
    int k, int scancode, int action, int mods);
void processInput(GLFWwindow *window);

// settings
float GLOBAL_K_VAL = 0.5;
    // Used to adjust the mix of face texture to door texture

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
    int screenWidth = 800, screenHeight = 600;
    GLFWwindow* window = glfwCreateWindow(
        screenWidth, screenHeight, "LearnOpenGL", NULL, NULL);

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

    // GL mode enabling:
    glEnable(GL_DEPTH_TEST); // this makes OpenGL do depth testing
        // which makes nearer faces overwrite further ones, but not vice-versa

    Shader ourShader("gs-camera.vs", "gs-camera.fs");

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    /* square: */ /*
    float vertices[] = {
        // positions         // texture coords
         0.5f,  0.5f, 0.0f,  1.0f, 1.0f,   // top right
         0.5f, -0.5f, 0.0f,  1.0f, 0.0f,   // bottom right
        -0.5f, -0.5f, 0.0f,  0.0f, 0.0f,   // bottom left
        -0.5f,  0.5f, 0.0f,  0.0f, 1.0f    // top left 
    }; */
    /* cube: */
    float vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
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
    unsigned int stride = (3 + 2) * sizeof(float);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*) offset);
    glEnableVertexAttribArray(0);
    offset += 3 * sizeof(float);


    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void *) offset);
    glEnableVertexAttribArray(1);
    offset += 2 * sizeof(float);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    // glBindVertexArray(0);


    // bind the VAO (it was already bound, but just to demonstrate): seeing as we only have a single VAO we can 
    // just bind it beforehand before rendering the respective triangle; this is another approach.
    glBindVertexArray(VAO);

    unsigned int door_tex_id, face_tex_id;
    if (load_image("textures/container.jpg", &door_tex_id, 
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

    // New stuff for coordinate systems:
    // glm::mat4 model = glm::mat4(1.0f);
    // model = glm::rotate(model, glm::radians(-55.f), glm::vec3(1.f, 0.f, 0.f));



    glm::vec3 cubePositions[] = {
      glm::vec3( 0.0f,  0.0f,  0.0f), 
      glm::vec3( 2.0f,  5.0f, -15.0f), 
      glm::vec3(-1.5f, -2.2f, -2.5f),  
      glm::vec3(-3.8f, -2.0f, -12.3f),  
      glm::vec3( 2.4f, -0.4f, -3.5f),  
      glm::vec3(-1.7f,  3.0f, -7.5f),  
      glm::vec3( 1.3f, -2.0f, -2.5f),  
      glm::vec3( 1.5f,  2.0f, -2.5f), 
      glm::vec3( 1.5f,  0.2f, -1.5f), 
      glm::vec3(-1.3f,  1.0f, -1.5f)  
    };
    
// for the below to work, need to have the same name in C++ and the shader
#define SEND_MAT4(mat_name) ourShader.setMat4(#mat_name, mat_name)

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
        // the Depth buffer stores how far away each pixel is
#define PI 3.14159
#define SINUSOID(in_s) sin(glfwGetTime()*2*PI/in_s)
        if (1) {
        // if (SINUSOID(3.) > 0.) {
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        } else {
            glClear(GL_COLOR_BUFFER_BIT);
        }

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, door_tex_id);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, face_tex_id);

        // render the rectangle
        glBindVertexArray(VAO);

        for (unsigned int i = 0; i < 10; ++i) {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            float angle = 20.0f * i + glfwGetTime() * 50.;
            if (i % 3 == 0) {
                model = glm::rotate(
                    model, 
                    glm::radians(angle),
                    glm::vec3(1.f, 0.3f, 0.5f)
                );
            }
            SEND_MAT4(model);

            // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        {
            // X = left-right
            float x_offset = 4*SINUSOID(7.);
            // Y = up-down
            float y_offset = 2.5*SINUSOID(11.);
            // Z = in-out
            float z_offset = -5.f + 3 * SINUSOID(5.);

            float radius = 10.0f;
            float camX = sin(glfwGetTime()) * radius;
            float camZ = cos(glfwGetTime()) * radius;
            glm::mat4 view = glm::lookAt(
                glm::vec3(camX, 0.0, camZ),
                glm::vec3(0., 0., 0.),
                glm::vec3(0., 1., 0.)
            );
            SEND_MAT4(view);
        }

        {
            // FOV changes are a lot like moving forward and backward
            float fov_degrees = 45.f+20.f * SINUSOID(9.);
            // aspect ratio can make it contracted either horizontally
            // or vertically
            float aspect_ratio = (float) screenWidth / screenHeight
                + (0.5 * SINUSOID(15.));

            glm::mat4 projection;
            projection = glm::perspective(
                glm::radians(fov_degrees), aspect_ratio,
                .1f, 100.f
            );
            SEND_MAT4(projection);
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
