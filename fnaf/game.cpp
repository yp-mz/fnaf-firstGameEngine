#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

#include"Anim.h"
#include"Shader.h"
#include"Renderer.h"
#include"AnimState.h"
#include"AnimGroup.h"
#include"Camera.h"

void processInput(GLFWwindow* window);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);

GLFWwindow* loadContext();

double mousX,mousY;
long long view[4];

int main()
{
    GLFWwindow* window = loadContext();
    if (window == NULL)
        return -1;

    Camera camera;

    Renderer ren(&camera);
    Shader s("image.v", "image.f");
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);

    /*Shader g("group.v", "group.f");
    g.setInt("Room", 1);
    g.setInt("Flash", 2);*/
    /*glm::mat4 x = glm::mat4(1.0f);
    glm::translate(x, glm::vec3(0.1, 0.2, 0));
    g.setMatrix("trans1", &x);
    /*Anim ani(21, "anim/Animation-0 Direction-0 Frame-");
    Anim ani2(3, "anim/fan");
    Anim ani3(1, "anim/room");
    ren.bind(&s);
    ren.add(&ani2);
    ren.add(&ani);
    ren.add(&ani3);*/
    AnimState a;
    a.addAnim("room", 1, "anim/world/room");
    a.addAnim("roomBu", 1, "anim/world/roomBu");
    a.setAnim("room");
   /* a.addAnim("fan", 3, "anim/fan");
    a.addAnim("freddy", 21, "anim/Animation-0 Direction-0 Frame-");
    a.setAnim("room");*/
    //Anim b(1, "anim/room");
    Anim c(3, "anim/item/fan");
    Anim d(16, "anim/item/Animation-12 Direction-0 Frame-");
    //ren.bind(&s);


    //ren.setMode(GROUP);
    //ren.add(&a);
    //ren.add(&b);
    //ren.add(&c);

    AnimGroup g("group.v", "group.f");
    //AnimGroup g("image.v", "image.f");
    /*g.shader->setInt("Room", 1);
    g.shader->setInt("Flash", 2);*/
    g.add(&a);
    g.add(&c);
    g.add(&d);
    ren.bind(&s);
    ren.add(&g); 
    //ren.add(&c);
    //ren.add(&c);
    //c.setSize(0.5, 0.5);0.08700930.278009
    c.setPos(0.0308,- 0.0589);
    d.setPos(-0.3858 ,0);
    d.isLoop = false;
    //c.setSize(0.087, 0.278);

    while (!glfwWindowShouldClose(window))
    {
        //std::cout << "\033c";
        processInput(window);
        //if (glfwGetKey(window, GLFW_KEY_0) == GLFW_PRESS)
        //    a.setAnim("freddy");
        //if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
        //    a.setAnim("fan");
       /* if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
            d.setSize(d.trans[2] + 0.001, d.trans[3]);
        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
            d.setSize(d.trans[2] - 0.001, d.trans[3]);
        if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
            d.setSize(d.trans[2], d.trans[3] - 0.001);
        if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
            d.setSize(d.trans[2], d.trans[3] + 0.001);*/
        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
            c.setPos(c.trans[0], c.trans[1] + 0.0001);
        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
            c.setPos(c.trans[0], c.trans[1] - 0.0001);
        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
            c.setPos(c.trans[0] + 0.0001, c.trans[1]);
        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
            c.setPos(c.trans[0] - 0.0001, c.trans[1]);
        if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) {
            d.isFlip = true;
            d.isPlay = true;
        }
        if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) {
            d.isFlip = false;
            d.isPlay = true;
        }
        if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) {
            a.setAnim("roomBu");
        }
        if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS) {
            a.setAnim("room");
        }


        glGetInteger64v(GL_VIEWPORT, view);

        float edgeLs = view[2] * 3 / 7;
        float edgeRs = view[2] * 4 / 7;
        float edgeLf = view[2] * 1 / 3;
        float edgeRf = view[2] * 2 / 3;
        std::cout << mousX << "  " << mousY << std::endl;
        if (mousX < edgeLs) {
            if (mousX < edgeLf)
                camera.x -= (edgeLs - mousX) / 40000;
            else
                camera.x -= (edgeLs - mousX) / 100000;
        }
        else if (mousX > edgeRs) {
            if (mousX > edgeRf)
                camera.x += (mousX - edgeRs) / 40000;
            else
                camera.x += (mousX - edgeRs) / 100000;
        }

        if (camera.x < -0.22)
            camera.x = -0.22;
        if (camera.x > 0.22)
            camera.x = 0.22;

        GLenum err;
        while ((err = glGetError()) != GL_NO_ERROR) {
            std::cout << "OpenGL error: " << err << std::endl;
        }


        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        ren.update();
        ren.render();
        while ((err = glGetError()) != GL_NO_ERROR) {
            std::cout << "OpenGL error: " << err << std::endl;
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    std::cout << "yes";
    glViewport(0, 0, width, height);
    /*float w = float(width);  
    float h = float(height);*/
    /*if (w > h / 9 * 16)
        glViewport(int((w- h / 9 * 16)/2), 0, int(h / 9 * 16), h);
    else
        glViewport(0, int((h - w / 16 * 9) / 2), w, int(w / 16 * 9));*/
}
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
    mousX = xpos;
    mousY = ypos;
}