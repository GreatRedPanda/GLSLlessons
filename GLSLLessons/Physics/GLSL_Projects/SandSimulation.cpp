#include "SandSimulation.h"

void SandSimulation::loop( GLFWwindow& window)
{
    ShaderProgram baseShader = ShaderProgram();
    baseShader.addShader("shaders/vertex.glsl", GL_VERTEX_SHADER);
    baseShader.addShader("shaders/frag.glsl", GL_FRAGMENT_SHADER);
    ShaderProgram sandShader = ShaderProgram();
    sandShader.addShader("shaders/fluid.glsl", GL_COMPUTE_SHADER);

    std::vector<glm::vec2> vel;
    std::vector<glm::vec2> pos;
    std::vector<float> type;
    int n = 100;
    int count = n * n;
    for (int k = 0; k < n; k++)
    {
        for (int i = 0; i < n; i++)
        {
           
            vel.push_back(glm::vec2((float)rand() / (float)RAND_MAX * (1 - (-1)) + (-1), -0.6));
            pos.push_back(glm::vec2(k / float(n) * 2.0 - 1 + 0.01, i / float(n) * 2.0 - 1 + 0.01));
            type.push_back(rand() % 2);
        }
    }
  
    type[667] = 1;
    type[27] = 1;
    type[16] = 1;
    vel[667] = glm::vec2(0.6, 0.4);

    float vertex[] = {

        -1, 1,
       1,1,
       1,-1,
        -1,-1

    };

    double d = signbit(-0.6);
    unsigned int VBO, VAO, VEL, TypeBO;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &VEL);
    glGenBuffers(1, &TypeBO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * count, pos.data(), GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), (void*)0);


    glBindBuffer(GL_ARRAY_BUFFER, VEL);
    glBufferData(GL_ARRAY_BUFFER, count * sizeof(glm::vec2), vel.data(), GL_DYNAMIC_DRAW);


    glBindBuffer(GL_ARRAY_BUFFER, TypeBO);
    glBufferData(GL_ARRAY_BUFFER, count * sizeof(float), type.data(), GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, sizeof(float), (void*)0);



    //glViewport(-2, -2, (GLsizei)2, (GLsizei)2);
   // int timeLoc = glGetUniformLocation(shaderFProgram, "deltaT");
    float dt = 0;
    while (!glfwWindowShouldClose(&window))
    {

        dt += 0.1;
        double now = glfwGetTime();
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        //glUniform1f(timeLoc, now / 100);

        if (dt > 60)
        {
            glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, VBO);
            glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, VEL);
            glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, TypeBO);
            sandShader.use();
            // glUseProgram(shaderFProgram);
            glDispatchCompute(1000, 1, 1);
            glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
            dt = 0;
        }



        glBindVertexArray(VAO);
        //  glUseProgram(shaderVProgram);
        baseShader.use();

        glPointSize(7.2 + 0.4);
        glDrawArrays(GL_POINTS, 0, count);

        glfwSwapBuffers(&window);
        glfwPollEvents();

    }

    glfwTerminate();
    //return 0;

}
