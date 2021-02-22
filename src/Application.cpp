// This code open a window and draw a triangle; using Modern OpenGL
// based on the code of The Cherno OpenGL tutorial

#include <GL/glew.h>       // first of all
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>  // to read the shaders file
#include <string>
#include <sstream>


struct ShaderProgramSource
{
    std::string VertexSource;
    std::string FragmentSource;
};

static ShaderProgramSource ParseShader(const std::string& filepath)
{
    std::ifstream stream(filepath);  //open the file

    enum class ShaderType
    {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };

    std::string line;
    std::stringstream ss[2];
    ShaderType type = ShaderType::NONE;

    // read line by line and find the beginning of each shader
    while (getline(stream, line))
    {
        if (line.find("#shader") != std::string::npos)
        {
            if (line.find("vertex") != std::string::npos)
                type = ShaderType::VERTEX;       //set mode to vertex
            else if (line.find("fragment") != std::string::npos)
                type = ShaderType::FRAGMENT;    // set mode to fragment
        }
        else
        {
            ss[(int)type] << line << '\n';
        }
    }
    return { ss[0].str(), ss[1].str() };
}

static unsigned int CompileShader(unsigned int type, const std::string& source)
{
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str(); //pointer to the begining of the array as source[0]
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    //Error handling
    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE)
    {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)alloca(length * sizeof(char)); // to identify the error
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << "shader!" << std::endl;
        std::cout << message << std::endl;
        glDeleteShader(id);
        return 0;
    }

    return id;
}

/* to create the shader*/
static int CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    unsigned int program = glCreateProgram();  //create the object for the shader
                                               // and go to Compile Shader function
                                               // and back here
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);  //to detach the intermediates
    glDeleteShader(fs);

    return program;
}

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;


    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    /* 1 */
    //glewInit();
    if (glewInit() != GLEW_OK)
        std::cout << "Error!" << std::endl;

    std::cout << glGetString(GL_VERSION) << std::endl;
    /*end 1*/

    /* Create the vertex buffer */
    /*float positions[6] = {
        -0.5f, -0.5f,
         0.0f,  0.5f,
         0.5f, -0.5f
    };*/
    float positions[] = {
        -0.5f, -0.5f,
         0.5f, -0.5f,
         0.5f,  0.5f,

         0.5f,  0.5f,
        -0.5f,  0.5f,
        -0.5f, -0.5f
    };


    //first, create the buffer with the size and provide the data or given it later
    unsigned int buffer;        // the buffer ID   see docs.gl
    glGenBuffers(1, &buffer);   // generate the buffer(s) object(s) 
    glBindBuffer(GL_ARRAY_BUFFER, buffer); //bind (select) a named buffer object 
    glBufferData(GL_ARRAY_BUFFER, 6 * 2 * sizeof(float), positions, GL_STATIC_DRAW); //creates and initialize a buffer's object data store

    //third make the draw call inside the loop 

    //fourth pointer to the attributes & enable the attributes of eache vertex
    glEnableVertexAttribArray(0);  //the index of the vertex attribute to be enabled
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);  //define an array of generic vertex attribute data


    //unsigned int shader = CreateShader(vertexShader, fragmentShader);
    //glUseProgram(shader); // to run the program
    ShaderProgramSource source = ParseShader("res/shaders/Basic.shader");
    /*these lines are to check if the reading is correct but can be deleted*/
    std::cout << "VERTEX" << std::endl;
    std::cout << source.VertexSource << std::endl;
    std::cout << "FRAGMENT" << std::endl;
    std::cout << source.FragmentSource << std::endl;
    //also comment the glDeleteProgram(shader);
    /* the end of test */

    unsigned int shader = CreateShader(source.VertexSource, source.FragmentSource);
    glUseProgram(shader); // to run the program

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        glDrawArrays(GL_TRIANGLES, 0, 6); // the draw call will use the vertex buffer to draw
                          // bind or select everything before access to that in the state machine

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glDeleteProgram(shader);

    glfwTerminate();
    return 0;
}