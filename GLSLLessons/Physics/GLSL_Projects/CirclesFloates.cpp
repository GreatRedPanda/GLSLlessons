#include "CirclesFloates.h"
#include "SoftBody.h"
#include "Line.h"
void CirclesFloates::loop(GLFWwindow& window)
{

    Line l1 = Line(glm::vec2(-0.7, 0.8), glm::vec2(0.7, -1), 1);
    SoftBody sb = SoftBody(glm::vec2(0.5, 0.1));
    SoftBody sb2 = SoftBody(glm::vec2(0.5, -0.2));

    SoftBody sb3 = SoftBody(glm::vec2(0.5, -0.4));
    sb.color = glm::vec3(1, 0, 0);
    sb2.color = glm::vec3(0, 1,1);
    sb3.color= glm::vec3(1, 0, 1);

    sb.lineTest = l1;
    sb2.lineTest = l1;
    sb3.lineTest = l1;
    ShaderProgram baseShader = ShaderProgram();
    baseShader.addShader("shaders/circleVertex.glsl", GL_VERTEX_SHADER);
    baseShader.addShader("shaders/circleShader.glsl", GL_FRAGMENT_SHADER);
    float radius = 1;
    std::vector<glm::vec2> pos;

    pos.push_back(glm::vec2(-1, -1));
    pos.push_back(glm::vec2(1, -1));
    pos.push_back(glm::vec2(-1, 1));

    pos.push_back(glm::vec2(-1, 1));
    pos.push_back(glm::vec2(1, -1));
    pos.push_back(glm::vec2(1, 1));
    unsigned int VBO, VAO, VEL, TypeBO;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &VEL);
    glGenBuffers(1, &TypeBO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2)*6, pos.data(), GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), (void*)0);
    glm::vec3 circlecenter = glm::vec3(0.3, 0.4, 0);
    glm::mat4 cubeModel = glm::mat4(1.0f);
    cubeModel = glm::translate(cubeModel, circlecenter);
    cubeModel = glm::scale(cubeModel, glm::vec3(0.3, 0.3, 0.3));

    float time = 0;
    while (!glfwWindowShouldClose(&window))
    {

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        double now = glfwGetTime();
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        l1.draw();
        for (size_t i = 0; i < SoftBody::bodies.size(); i++)
        {
            SoftBody::bodies[i]->simulate(now-time);
            SoftBody::bodies[i]->draw();
        }
 /*       sb.simulate(sb2);
        sb2.simulate(sb);
        sb3.simulate(sb3);
        sb.draw();
        sb2.draw();
        sb3.draw();*/
     /*   glBindVertexArray(VAO);
        glUniformMatrix4fv(baseShader.getUniform("model"), 1, GL_FALSE, glm::value_ptr(cubeModel));
        baseShader.use();
        glDrawArrays(GL_TRIANGLES, 0,6);*/
        glfwSwapBuffers(&window);
        glfwPollEvents();
        time = now;
    }

    glfwTerminate();

}
