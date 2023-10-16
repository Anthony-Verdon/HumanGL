#include "parsing/parsing.hpp"
#include "classes/Shader/Shader.hpp"
#include "classes/Texture/Texture.hpp"
#include "classes/Camera/Camera.hpp"
#include "classes/Time/Time.hpp"
#include "classes/Object/Object.hpp"
#include "classes/Matrix/Matrix.hpp"
#include "classes/Utils/Utils.hpp"
#include "input.hpp"
#include "scene.hpp"
#include "init.hpp"
#include <iostream>


static Matrix initAxisVector(float X, float Y, float Z)
{
    Matrix axisVector(3, 1);
    float axisVectorValues[] = {
        X, Y, Z
    };
    axisVector.setData(axisVectorValues, 3);
    return (axisVector);
}

static void updateCamera(Camera &camera)
{
    Matrix direction(3, 1);
    float directionValues[] = {
        cosf(Utils::DegToRad(camera.getYaw())) * cosf(Utils::DegToRad(camera.getPitch())),
        sinf(Utils::DegToRad(camera.getPitch())),
        sinf(Utils::DegToRad(camera.getYaw())) * cosf(Utils::DegToRad(camera.getPitch()))
    };
    direction.setData(directionValues, 3);
    camera.setFrontDirection(Matrix::normalize(direction));
    camera.setRightDirection(Matrix::normalize(Matrix::crossProduct(camera.getFrontDirection(), camera.getUpDirection())));
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
static void updateLoop(GLFWwindow* window, const std::vector<std::unique_ptr<Object>> &objects)
{
    Shader shader("srcs/shaders/shader.vs", "srcs/shaders/shader.fs");
    Texture::initTexParameter();
    Texture wall("textures/wall.ppm");
    Matrix position = initAxisVector(0.0f, 0.0f, 3.0f);
    Matrix upDirection = initAxisVector(0.0f, 1.0f, 0.0f);
    Camera camera(position, upDirection, -90.0f, 0.0f, 0.0f, 45.0f, 2.5f);
    t_scene scene;
    scene.camera = &camera;
    scene.displayColor = true;
    scene.mixValue = 0;
    for (size_t i = 0; i < 3; i++)
        scene.rotation[i] = 0;
    float angleX = 0;
    float angleY = 0;
    float angleZ = 0;
    Matrix axisX = initAxisVector(1.0f, 0.0f, 0.0f);
    Matrix axisY = initAxisVector(0.0f, 1.0f, 0.0f);
    Matrix axisZ = initAxisVector(0.0f, 0.0f, 1.0f);

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glEnable(GL_DEPTH_TEST);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetWindowUserPointer(window, &scene);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    for (size_t i = 0; i < objects.size(); i++)
        objects[i]->initVAO();

    while(!glfwWindowShouldClose(window))
    {
        Time::updateTime();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        processInput(window);
        updateCamera(camera);

        for (size_t i = 0; i < objects.size(); i++)
        {
            shader.use();

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, wall.getID());
            shader.setInt("texture1", 0);
            std::array<float, 3> color = objects[i]->getMaterial(0).getColor(AMBIANT_COLOR);
            shader.setVec3("aColor", color[0], color[1], color[2]);
            shader.setFloat("aMixValue", scene.mixValue);

            Matrix rotation(4, 4);
            rotation.uniform(1);
            angleX += scene.rotation[X_AXIS] * Time::getDeltaTime();
            angleY += scene.rotation[Y_AXIS] * Time::getDeltaTime();
            angleZ += scene.rotation[Z_AXIS] * Time::getDeltaTime();
            rotation = Matrix::rotate(rotation, angleX, Matrix::normalize(axisX)) * Matrix::rotate(rotation, angleY, Matrix::normalize(axisY)) * Matrix::rotate(rotation, angleZ, Matrix::normalize(axisZ));
            shader.setMat4("rotation", rotation);

            Matrix projection = Matrix::perspective(camera.getFov(), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 100.0f);
            shader.setMat4("projection", projection);
            
            Matrix view = Matrix::lookAt(camera.getPosition(), camera.getPosition() + camera.getFrontDirection(), camera.getUpDirection());
            shader.setMat4("view", view);

            glBindVertexArray(objects[i]->getVAO());

            glDrawElements(GL_TRIANGLES, objects[i]->getFaces().size() * 3, GL_UNSIGNED_INT, 0);

            /*
            //if we want to draw face by face for multiples materials
            for (size_t i = 0; i < object.getFaces().size(); i++)
            {
                std::array<float, 3> color;
                if (object.getMaterialDictionnary().size() > 0)
                    color = object.getMaterial(i).getColor(AMBIANT_COLOR);
                else
                {
                    for (size_t i = 0; i < 3; i++)
                        color[i] = 0.5;
                }
                ourShader.setVec3("aColor", color[0], color[1], color[2]);
                glDrawArrays(GL_TRIANGLES, i * 3, 3);
            }
            */
        }
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
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
        std::vector<std::unique_ptr<Object>> objects = parseObjFile(argv[1]);
        initGLFW();
        GLFWwindow* window = initWindow();
        initOpenGL();
        updateLoop(window, objects);
        glfwTerminate();
        return (0);
    }
    catch(const Utils::Exception& e)
    {
        std::cerr << std::endl << e.what() << std::endl;
        glfwTerminate();
        return (-1);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        glfwTerminate();
        return (-1);
    }
}