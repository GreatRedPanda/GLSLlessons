#include <glm.hpp>
#include <gl/glew.h>
#include <GLFW/glfw3.h>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <cmath>
#include "fluid.h"


#define clamp(x,y,z) ((x) < (y) ? (y) : ((x) > (z) ? (z) : (x)))


fluid_c::fluid_c(int size, int diffusion, int viscosity, float dt)
{
	this->dt = dt;
	this->size = size;
	this->diff = diffusion;
	this->visc = viscosity;
	N = size;
	for (size_t i = 0; i < size*size; i++)
	{
		s.push_back(0);
		density.push_back(0);
		Vx0.push_back(0);
		Vy0.push_back(0);
		Vx.push_back(0);
		Vy.push_back(0);
	}
}

int fluid_c::IX(int x, int y)
{
    x = (x < 0) ? 0 : x;
    y= (y < 0) ? 0 : y;
    x = (x >=N) ? N-1 : x;
    y = (y >= N) ? N - 1 : y;

	return x+y*size;
}

void fluid_c::addDensity(int x, int y, float amount)
{
	density[IX(x, y)] += amount;
}

void fluid_c::addVelocity(int x, int y, float amountX, float amountY)
{
	this->Vx[IX(x, y)] += amountX;
    this->Vy[IX(x, y)] += amountY;
}

void fluid_c::step()
{
    diffuse(1, Vx0, Vx, visc, dt, 4);
    diffuse(2, Vy0, Vy, visc, dt, 4);


    project(Vx0, Vy0, Vx, Vy, 4);

    advect(1, Vx, Vx0, Vx0, Vy0, dt);
    advect(2, Vy, Vy0, Vx0, Vy0, dt);


    project(Vx, Vy, Vx0, Vy0, 4);

    diffuse(0, s, density, diff, dt, 4);
    advect(0, density, s, Vx, Vy, dt);
}

void fluid_c::advect(int b, std::vector<float>& d, std::vector<float>& d0, std::vector<float>& velocX, std::vector<float>& velocY, float dt)
{

    float i0, i1, j0, j1;

    float dtx = dt * (N - 2);
    float dty = dt * (N - 2);
  

    float s0, s1, t0, t1, u0, u1;
    float tmp1, tmp2,  x, y;

    float Nfloat = N;
    float ifloat, jfloat, kfloat;
    int i, j, k;


        for (j = 1, jfloat = 1; j < N - 1; j++, jfloat++) {
            for (i = 1, ifloat = 1; i < N - 1; i++, ifloat++) {
                tmp1 = dtx * velocX[IX(i, j)];
                tmp2 = dty * velocY[IX(i, j)];
                x = ifloat - tmp1;
                y = jfloat - tmp2;

                if (x < 0.5f) x = 0.5f;
                if (x > Nfloat + 0.5f) x = Nfloat + 0.5f;
                i0 = floorf(x);
                i1 = i0 + 1.0f;
                if (y < 0.5f) y = 0.5f;
                if (y > Nfloat + 0.5f) y = Nfloat + 0.5f;
                j0 = floorf(y);
                j1 = j0 + 1.0f;
            

                s1 = x - i0;
                s0 = 1.0f - s1;
                t1 = y - j0;
                t0 = 1.0f - t1;

                int i0i = i0;
                int i1i = i1;
                int j0i = j0;
                int j1i = j1;


                d[IX(i, j)] =

                    s0 * (t0 *  d0[IX(i0i, j0i)] + (t1 *  d0[IX(i0i, j1i)]  ))
                    + s1 * (t0 * d0[IX(i1i, j0i)]+ (t1*  d0[IX(i1i, j1i)] ));
            }
        }
    
    set_bnd(b, d);
}

void fluid_c::diffuse(int b, std::vector<float>& x, std::vector<float>& x0, float diff, float dt, int iter)
{
    float a = dt * diff * (N - 2) * (N - 2);
    lin_solve(b, x, x0, a, 1 + 6 * a, iter);
}

void fluid_c::project(std::vector<float>& velocX, std::vector<float>& velocY,
    std::vector<float>& p, std::vector<float>& div, int iter)
{


        for (int j = 1; j < N - 1; j++) {
            for (int i = 1; i < N - 1; i++) {
                div[IX(i, j)] = -0.5f * (
                    velocX[IX(i + 1, j)]
                    - velocX[IX(i - 1, j)]
                    + velocY[IX(i, j + 1)]
                    - velocY[IX(i, j - 1)]                   
                    ) / N;
                p[IX(i, j)] = 0;
            }
        }
    
    set_bnd(0, div);
    set_bnd(0, p);
    lin_solve(0, p, div, 1, 6, iter);

   
        for (int j = 1; j < N - 1; j++) {
            for (int i = 1; i < N - 1; i++) {

                velocX[IX(i, j)] -= 0.5f * (p[IX(i + 1, j)]- p[IX(i - 1, j)]) * N;
                velocY[IX(i, j)] -= 0.5f * (p[IX(i, j + 1)]  - p[IX(i, j - 1)]) * N;
            }
        }

    set_bnd(1, velocX);
    set_bnd(2, velocY);



}

void fluid_c::set_bnd(int b, std::vector<float> &x)
{

        for (int i = 1; i < N - 1; i++) {
            x[IX(i, 0)] = (b == 2) ? ( x[IX(i, 1)]) : x[IX(i, 1)];
            x[IX(i, N - 1)] = (b == 2) ? -x[IX(i, N - 2)] : x[IX(i, N - 2)];
        }
    
   
        for (int j = 1; j < N - 1; j++) {
            x[IX(0, j)] = b == 1 ? -x[IX(1, j)] : x[IX(1, j)];
            x[IX(N - 1, j)] = b == 1 ? -x[IX(N - 2, j)] : x[IX(N - 2, j)];
        }
    
    x[IX(0, 0)] = 0.5 * (x[IX(1, 0)] + x[IX(0, 1)]);
    x[IX(0, N - 1)] = 0.5 * (x[IX(1, N - 1)] + x[IX(0, N - 2)]);
    x[IX(N - 1, 0)] = 0.5 * (x[IX(N - 2, 0)] + x[IX(N - 1, 1)]);
    x[IX(N - 1, N - 1)] = 0.5 * (x[IX(N - 2, N - 1)] + x[IX(N - 1, N - 2)]);
  

}

void fluid_c::lin_solve(int b, std::vector<float>& x, std::vector<float>& x0, float a, float c, int iter)
{

    float cRecip = 1.0 / c;
    for (int k = 0; k < iter; k++) {
            for (int j = 1; j < N - 1; j++) {
                for (int i = 1; i < N - 1; i++) {
                    x[IX(i, j)] =
                        (x0[IX(i, j)]
                            + a * (x[IX(i + 1, j)]
                                + x[IX(i - 1, j)]
                                + x[IX(i, j + 1)]
                                + x[IX(i, j - 1)]
                               
                                )) * cRecip;
                }
            }
        
        set_bnd(b, x);
    }
}
