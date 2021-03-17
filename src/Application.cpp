/*This code uses Modern OpenGL to render geometry, 
    and uses an abstraction of the VertexBuffer, VertexArray, and Shader.
  The code is based on The Cherno OpenGL tutorial in https://youtube.com/playlist?list=PLlrATfBNZ98foTJPJ_Ev03o2oq3-GGOS2
*/
#include <GL/glew.h>       
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>  
#include <string>
#include <sstream>

#include "Renderer.h"

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include <chrono>

int main(void)
{
    /* Take time at beginning of execution */
    auto t_start = std::chrono::high_resolution_clock::now();

    /* Declare window */
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    // to create the vertex array
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Rendering a textured cube", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    glfwSwapInterval(1);  //to synchronize the animation

    /* Initialize GLEW */
    if (glewInit() != GLEW_OK)
        std::cout << "Error!" << std::endl;

    std::cout << glGetString(GL_VERSION) << std::endl;
    {
        // include the texture positions to be mapped
        float positions[] = {

            /* Normalized Device Coordinates (NDC) [-1, 1]*/            
            // X      Y     Z     U     V
            -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,    // vertex 0
             0.5f, -0.5f, -0.5f, 1.0f, 0.0f,    // vertex 1
             0.5f,  0.5f, -0.5f, 1.0f, 1.0f,    // vertex 2
            -0.5f,  0.5f, -0.5f, 0.0f, 1.0f,    // vertex 3        

            -0.5f, -0.5f,  0.5f, 0.0f, 0.0f,    // vertex 4
             0.5f, -0.5f,  0.5f, 1.0f, 0.0f,    // vertex 5
             0.5f,  0.5f,  0.5f, 1.0f, 1.0f,    // vertex 6         
            -0.5f,  0.5f,  0.5f, 0.0f, 1.0f,    // vertex 7        

            -0.5f,  0.5f,  0.5f, 1.0f, 0.0f,    // vertex 8
            -0.5f,  0.5f, -0.5f, 1.0f, 1.0f,    // vertex 9
            -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,    // vertex 10        

             0.5f,  0.5f,  0.5f, 1.0f, 0.0f,    // vertex 11         
             0.5f, -0.5f, -0.5f, 0.0f, 1.0f,    // vertex 12         
             0.5f, -0.5f,  0.5f, 0.0f, 0.0f,    // vertex 13         
        
             0.5f, -0.5f, -0.5f, 1.0f, 1.0f,    // vertex 14         
        
            -0.5f,  0.5f,  0.5f, 0.0f, 0.0f,    // vertex 15        
        };

        /* Data sent to Index Buffer */
        unsigned int indices[] = {
            0, 1, 2,    // 1st triangle has vertices 0, 1, 2
            2, 3, 0,    // 2nd triangle has vertices 2, 3, 0

            4, 5, 6,    // 3rd triangle has vertices 4, 5, 6
            6, 7, 4,    // 4th triangle has vertices 6, 7, 4

            8, 9, 10,   // 5th triangle has vertices 8, 9, 10
            10, 4, 8,   // 6th triangle has vertices 10, 4, 8

            11, 2, 12,  // 7th triangle has vertices 11, 2, 12
            12, 13, 11, // 8th triangle has vertices 12, 13, 11

            10, 14, 5,  // 9th triangle has vertices 10, 14, 5
            5, 4, 10,   // 10th triangle has vertices 5, 4, 10

            3, 2, 11,   // 11th triangle has vertices 3, 2, 11
            11, 15, 3   // 12th triangle has vertices 11, 15, 3
        };

        //defining how openGL is going to blend alpha
        GLCall(glEnable(GL_BLEND));                                 
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));  //src alpha = 0; dest = 1 - 0 = 0
       

        /* Declare vertex array, and send data to it */
        VertexArray va;        
        VertexBuffer vb(positions, 36 * 5 * sizeof(float)); // 36 vertices, each with 5 elements: 3 for position (x, y, z), 2 for texture coordinates
        
        /* Configure Vertex Buffer 
           Add data to be taken from the buffer, and from where.
         */
        VertexBufferLayout layout;
        layout.Push<float>(3);  // position attributes (x, y, z)        
        layout.Push<float>(2);  // texture coordinate attributes
        va.AddBuffer(vb, layout);

        /* Declare index buffer, specifying its elements */
        IndexBuffer ib(indices, 36);

        /* Specify shader's path, bind it, and define uniforms */
        Shader shader("res/shaders/Basic.shader");
        shader.Bind();
        shader.SetUniform4f("u_Color", 0.2f, 0.3f, 0.8f, 1.0f);


        /* VIEW MATRIX */                   
        glm::mat4 view = glm::lookAt(       // lookAt function simulates moving camera.
            glm::vec3(1.2f, 1.2f, 1.2f),    // camera position
            glm::vec3(0.0f, 0.0f, 0.0f),    // point to be centered on-screen
            glm::vec3(0.0f, 0.0f, 1.0f)     // up axis (z-axis)
        );
        
        shader.SetUniformMat4f("u_V", view);
        

        /* PROJECTION MATRIX
         * Using transformations      
         * Generar matriz de proyecci�n, y ajustarlo a un cuadrado.
         * Specify boundaries of window: any position outside them will not appear.
         * Multiplies array of vertex positions => Converted to [-1, 1] space
         * Change it to be pixel space (1:1 mapping), based on window resolution
         */
        glm::mat4 proj = glm::perspective(      // create perspective projection matrix
            glm::radians(45.0f),                // vertical field-of-view
            800.0f / 600.0f,                    // aspect ration of the screen
            1.0f,                               // near plane
            10.0f                               // far plane
        );
        
        shader.SetUniformMat4f("u_P", proj);    // link projection matrix to shader's projection uniform

        /* Specify texture's path, bind it, and define uniform */
        Texture texture("res/textures/doge.jpg");
        texture.Bind();
        /* Identifica lo que est� en el buffer (u_Texture), es lo que se usar� en el fragment shader.*/
        /* buscar� ese texture en el slot de las texturas especificado */
        /* => encontrar en el shader el nombre especificado, y asignar ese valor*/
        /* A�s�gnale a u_Texture lo que se encuentra en el slot 0*/
        shader.SetUniform1i("u_Texture", 0);  //the slot is 0

        /* Clean memory up */
        va.Unbind();
        vb.Unbind();
        ib.Unbind();
        shader.UnBind();
        
        /* Declare renderer */
        Renderer renderer;

        // to create the animation that change the color 
        float r = 0.0f;
        float increment = 0.05f;
        GLCall(glEnable(GL_DEPTH_TEST));

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {            
            /* Render here */
            GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
            //renderer.Clear();

            // use the shader and bind the buffer and ibo each time in case that the buffer change
            shader.Bind();
            shader.SetUniform4f("u_Color", r, 0.3f, 0.8f, 1.0f);

            /* Draw what's sent through vertex array, index buffer, and shader */
            renderer.Draw(va, ib, shader);

            /* MODEL MATRIX */
            auto t_now = std::chrono::high_resolution_clock::now();
            float time = std::chrono::duration_cast<std::chrono::duration<float>>(t_now - t_start).count();
            
            glm::mat4 model = glm::mat4(1.0f);      // Model matrix initialized as 4x4 identity matrix
            model = glm::rotate(
                model,                
                time * glm::radians(180.0f),        // rotates 180�
                glm::vec3(0.0f, 0.0f, 1.0f)         // around z-axis
            );
            
            shader.SetUniformMat4f("u_M", model);   // perform model transformation in vertex shader

            /* DRAW CALL */
            GLCall(glDrawElements(
                GL_TRIANGLES,       // drawing triangles
                36,                 // 36 indices in index buffer
                GL_UNSIGNED_INT,    // type of data in index buffer
                nullptr)            // pointer to index buffer (null because already bound)
            );

            //part of the animation
            if (r > 1.0f)
                increment = -0.05f;
            else if (r < 0.0f)
                increment = 0.05f;

            r += increment;

            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            /* Poll for and process events */
            glfwPollEvents();
        }

    }
    glfwTerminate();
    return 0;
}



