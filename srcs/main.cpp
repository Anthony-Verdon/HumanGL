#include "main.hpp"
#include "parsing/parsing.hpp"
#include "classes/Shader.hpp"
#include "classes/Texture.hpp"
#include "classes/Camera.hpp"
#include "classes/Time.hpp"
#include "classes/Object.hpp"
#include "classes/Matrix.hpp"
#include "classes/Utils.hpp"

Camera initCamera()
{
    Matrix position(3, 1);
    Matrix upDirection(3, 1);

    float positionValues[] = {
        0.0f,
        0.0f,
        3.0f
    };

    float upDirectionValues[] = {
        0.0f,
        1.0f,
        0.0f
    };

    position.setData(positionValues, 3);
    upDirection.setData(upDirectionValues, 3);
    Camera camera(position, upDirection, -90.0f, 0.0f, 0.0f, 45.0f, 2.5f);
    return (camera);
}
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
void updateLoop(GLFWwindow* window, const Object &object, unsigned int texture)
{
    Shader ourShader("shaders/shader.vs", "shaders/shader.fs");
    Camera camera(initCamera());
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
        
        Matrix direction(3, 1);
        float directionValues[] = {
            cosf(Utils::DegToRad(camera.getYaw())) * cosf(Utils::DegToRad(camera.getPitch())),
            sinf(Utils::DegToRad(camera.getPitch())),
            sinf(Utils::DegToRad(camera.getYaw())) * cosf(Utils::DegToRad(camera.getPitch()))
        };
        direction.setData(directionValues, 3);
        camera.setFrontDirection(Matrix::normalize(direction));
        camera.setRightDirection(Matrix::normalize(Matrix::crossProduct(camera.getFrontDirection(), camera.getUpDirection())));

        ourShader.use();
        ourShader.setInt("texture1", 0);

        Matrix rotation(4, 4);
        Matrix vector(3, 1);
        float values[] = {
            0.0f,
            1.0f,
            0.0f
        };
        vector.setData(values, 3);
        rotation.uniform(1.0f);
        rotation = Matrix::rotate(rotation, Time::getTime() * Utils::DegToRad(90.0f), Matrix::normalize(vector));
        ourShader.setMat4("model", rotation);
        Matrix projection = Matrix::perspective(camera.getFov(), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 100.0f);
        ourShader.setMat4("projection", projection);
        Matrix view = Matrix::lookAt(camera.getPosition(), camera.getPosition() + camera.getFrontDirection(), camera.getUpDirection());
        ourShader.setMat4("view", view);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
    
        glBindVertexArray(object.getVAO());
        glDrawElements(GL_TRIANGLES, object.getFaces().size() * 3, GL_UNSIGNED_INT, 0);
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
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
    Texture wall("textures/wall.ppm");
    
    updateLoop(window, objects[0], wall.getID());
}

/**
 * main function.
 * init GLFW,
 * create the shader program,
 * and start the update loop
*/
int main(int argc, char **argv)
{
    try
    {
        if (argc != 2)
            throw(Utils::Exception("MAIN::NO_INPUT_FILE"));
        std::vector<Object> objects = parseObjFile(argv[1]);
        GLFWwindow* window = initGLFW();
        if (window == NULL)
            throw(Utils::Exception("MAIN::WINDOW_NOT_CREATED"));
        start(window, objects);
        glfwTerminate();
        return (0);
    }
    catch(const Utils::Exception& e)
    {
        std::cerr << std::endl << e.what() << std::endl;
        return (-1);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        return (-1);
    }
}