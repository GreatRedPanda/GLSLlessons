#pragma once
#ifndef FLUID_HPP
#define FLUID_HPP

#include <vector>
#include <glm.hpp>
#include <gl/glew.h>
#include <GLFW/glfw3.h>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

class fluid_c {
private:
	


public:
    fluid_c(int size, int diffusion, int viscosity, float dt);
    int size;
    int N;
    float dt;
    float diff;
    float visc;

    std::vector<float> s = std::vector<float>();
    std::vector<float> density = std::vector<float>();

    std::vector<float> Vx = std::vector<float>();
    std::vector<float> Vy = std::vector<float>();

    std::vector<float> Vx0 = std::vector<float>();
    std::vector<float> Vy0 = std::vector<float>();
   
    int IX(int x, int y);
    void addDensity(int x, int y, float amount);
    void addVelocity(int x, int y,  float amountX, float amountY);

    void step();
    void advect(int b, std::vector<float>& d, std::vector<float>& d0, std::vector<float>& velocX, std::vector<float>& velocY, float dt);
    void diffuse(int b, std::vector<float>& x, std::vector<float>& x0, float diff, float dt, int iter);
    void project(std::vector<float>& velocX, std::vector<float>& velocY,  std::vector<float>& p, std::vector<float>& div, int iter);
    void set_bnd(int b, std::vector<float> &x);
    void lin_solve(int b, std::vector<float>& x, std::vector<float>& x0, float a, float c, int iter);

    void draw()
    {
        glClearColor(1.0, 1.0, 1.0, 1.0);
        glColor3f(1.0, 0.0, 0.0);
        glPointSize(8.0);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();

        glBegin(GL_POINTS);

    
        for (size_t i = 0; i < N; i++)
        {
            for (size_t j = 0; j < N; j++)
            {
                float d = this->density[IX(i, j)];
              
                glColor3f(d,d,d);
                glVertex2f(i/ float(N)*2.0-1, j / float(N) * 2.0 - 1);
            }
        }
        glEnd();
        glFlush();
    }
};

#endif