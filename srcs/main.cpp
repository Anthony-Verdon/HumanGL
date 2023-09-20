#include "main.hpp"
#include "classes/Shader.hpp"
/**
 * update loop of the window.
 * 
 * define the base color of the window,
 * start the loop
 * and stop it when the window should close (red cross or ESC button),
 * 
 * in the loop :
 * clear the screen, 
 * process input of the user,
 * choose which elements we need to draw (VAO) with which shader, here we got only one for both
 * draw our elements,
 * swap buffer,
 * meaning the one which is displayed is replaced by the one we drew,
 * and update event like input or callback function
*/
void updateLoop(GLFWwindow* window, unsigned int VAO, unsigned int texture1, unsigned int texture2)
{
    Shader ourShader("shaders/shader.vs", "shaders/shader.fs");
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    
    glm::vec4 vec(1.0f, 0.0f, 0.0f, 1.0f);
    while(!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);
        processInput(window);

        ourShader.use();
        ourShader.setInt("texture1", 0);
        ourShader.setInt("texture2", 1);

        glm::mat4 trans = glm::mat4(1.0f); //uniform matrice
        trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));
        trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0,0.0,1.0));
        trans = glm::scale(trans, glm::vec3(0.5,0.5,0.5));
        unsigned int transformLoc = glGetUniformLocation(ourShader.getID(), "transform");
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));

        float timeValue = glfwGetTime();
        float mixPercentage = (sin(timeValue) / 2.0f) + 0.5f;
        ourShader.setFloat("mixPercentage", mixPercentage);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);
    
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

/**
 * suppress all vertex array object,
 * vertex buffer object,
 * element buffer object,
 * the shaderProgram
 * and terminate GLFW.
*/
void endProgram(unsigned int VAO, unsigned int VBO, unsigned int EBO)
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    glfwTerminate();
}

/**
 * initialize everything before the update loop.
 * 
 * initialize vertices of the element we draw.
 * initialize elements we draw with vertex.
 * 
 * create Vertex Array Object (VAO)
 * create Vertex Buffer Object (VBO) and Element Buffer Object (EBO)
 * 
 * create an ID for each one.
 * 
 * bind the VAO so next VBO and EBO created are stored in it.
 * bind a type of buffer for VBO and EBO.
 * 
 * copy data into the buffer bind to the type of buffer specified.
 * 
 * explain to OpenGL how to read the data and enable it.
 * the 0 is the index of your VAO, which we set into the vertex shader.
 * 
 * unbind everything. Remember : always unbind the VAO first
 * (even if unbind isn't really necessary
 * because we will bind everytime before modifying something).
 * 
 * start the update loop
 * and end the program when the loop end. 
*/
void start(GLFWwindow* window)
{
    const float vertices[] = {
        // positions          // colors           // texture coords
         0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
         0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
        -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
    };

    const unsigned int indices[] = {
        0, 1, 2, //first triangle
        0, 2, 3 //second triangle
    };

    unsigned int VAO;
    unsigned int VBO;
    unsigned int EBO;

    glGenVertexArrays(1, &VAO); 
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO); 
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW); 

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    //------------

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_set_flip_vertically_on_load(true);

    int width;
    int height;
    int nrChannels;
    unsigned char *data = stbi_load("textures/wall.jpg", &width, &height, &nrChannels, 0);

    unsigned int texture1;
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);

    data = stbi_load("textures/awesomeface.png", &width, &height, &nrChannels, 0);

    unsigned int texture2;
    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);

    //------------

    updateLoop(window, VAO, texture1, texture2);
    endProgram(VAO, VBO, EBO);

}

/**
 * main function.
 * init GLFW,
 * create the shader program,
 * and start the update loop
*/
int main(void)
{
    GLFWwindow* window = initGLFW();
    if (window == NULL)
        return (-1);
    start(window);
    return (0);
}