#include "main.hpp"
#include "parsing/parsing.hpp"
#include "classes/Shader.hpp"
#include "classes/Texture.hpp"
#include "classes/Camera.hpp"
#include "classes/Time.hpp"
#include "classes/Object.hpp"

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
void updateLoop(GLFWwindow* window, const Object &object, unsigned int texture1, unsigned int texture2)
{
    Shader ourShader("shaders/shader.vs", "shaders/shader.fs");
    Camera camera(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f, 0.0f, 45.0f, 2.5f);
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    
    glEnable(GL_DEPTH_TEST);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetWindowUserPointer(window, &camera);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    while(!glfwWindowShouldClose(window))
    {
        Time::updateTime();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        processInput(window);
        
        glm::vec3 direction;
        direction.x = cos(glm::radians(camera.getYaw())) * cos(glm::radians(camera.getPitch()));
        direction.y = sin(glm::radians(camera.getPitch()));
        direction.z = sin(glm::radians(camera.getYaw())) * cos(glm::radians(camera.getPitch()));
        camera.setFrontDirection(glm::normalize(direction));
        camera.setRightDirection(glm::normalize(glm::cross(camera.getFrontDirection(), camera.getUpDirection())));

        
        glm::mat4 view = glm::lookAt(camera.getPosition(), camera.getPosition() + camera.getFrontDirection(), camera.getUpDirection());
        ourShader.use();
        ourShader.setInt("texture1", 0);
        ourShader.setInt("texture2", 1);

        glm::mat4 model = glm::mat4(1.0f); //uniform matrice
        //model = glm::rotate(model, Time::getTime() * glm::radians(-55.0f), glm::vec3(0.5f,1.0f,0.0f));
        ourShader.setMat4("model", model);
        ourShader.setMat4("view", view);
        glm::mat4 projection;
        projection = glm::perspective(glm::radians(camera.getFov()), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 100.0f);
        ourShader.setMat4("projection", projection);
        
        float mixPercentage = (sin(Time::getTime()) / 2.0f) + 0.5f;
        ourShader.setFloat("mixPercentage", mixPercentage);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);
    
        glBindVertexArray(object.getVAO());
        glDrawElements(GL_TRIANGLES, object.getFaces().size() * 3, GL_UNSIGNED_INT, 0);
        
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

void start(GLFWwindow* window, std::vector<Object> objects)
{
    if (objects.size() == 0)
        return ;
    #ifdef DEBUG
        for (size_t i = 0; i < objects[1].getVertices().size() * 4; i++)
        {
            if (i % 4 == 0)
                std::cout << std::endl;
            std::cout << vertices[i] << " ";
        }
        std::cout << std::endl;
        for (size_t i = 0; i < 6; i++)
        {
            if (i % 3 == 0)
                std::cout << std::endl;
            std::cout << indices[i] << " ";
        }
        std::cout << std::endl;
    #endif
    
    objects[0].initVAO();
    Texture::initTexParameter();
    Texture wall("textures/wall.jpg");
    Texture smiley("textures/awesomeface.png");

    updateLoop(window, objects[0], wall.getID(), smiley.getID());
}

/**
 * main function.
 * init GLFW,
 * create the shader program,
 * and start the update loop
*/
int main(int argc, char **argv)
{
    if (argc != 2)
    {
        std::cerr << "no input file provided" << std::endl;
        return (-1);
    }
    std::vector<Object> objects = parseObjFile(argv[1]);
    GLFWwindow* window = initGLFW();
    if (window == NULL)
        return (-1);
    start(window, objects);
    glfwTerminate();
    return (0);
}