

#include "pch.h"
#include <iostream>
#include <SFML/Graphics.hpp>
#include <list>
#include <SFML/OpenGL.hpp>
#include "GLU.h"
using namespace sf;
using namespace std;



void draw3d(GLfloat  *vertecies)
{
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0.f, 0.f, -200.f);
	glBegin(GL_TRIANGLES);
	
	glVertexPointer(3, GL_FLOAT, 0, vertecies);
	glEnableClientState(GL_VERTEX_ARRAY);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glDisableClientState(GL_VERTEX_ARRAY);
}
int main()
{
	
	sf::RenderWindow rw(sf::VideoMode(600, 600), "34243");

	RectangleShape rr = RectangleShape(Vector2f(600	, 600));
	Sprite s= Sprite();
	sf::
	Shader sh;
	Shader fallShader;
	Shader mixShader;
	Texture t;
	t.loadFromFile("231.png");
	sh.loadFromFile("Game2.frag", sf::Shader::Fragment);
	fallShader.loadFromFile("Input.frag", sf::Shader::Fragment);
	mixShader.loadFromFile("Mix1.frag", sf::Shader::Fragment);

	
	sh.setUniform("iResolution", Glsl::Vec2(600, 600));
	fallShader.setUniform("iResolution", Glsl::Vec2(600, 600));
	mixShader.setUniform("iResolution", Glsl::Vec2(600, 600));
	RenderTexture rt;
	rt.create(600, 600);
	Clock clock;
	
	static GLfloat pVerts[] = { 0,0,0,
		100,0,0,
		100,100,0,
		0,100,0 };

	static GLushort pInds[][9] = { {0,1,2,3},{3,2,4},{7,3,4,5},{2,8,5,4},{5,8,7},{0,3,7,6},{0,6,9,1},{2,1,9,8},{6,7,8,9 } };
	
	float angleX = 0;
	float angleY = 0;
	float angleZ = 0;
	Texture shaderT;
	Texture prints;
	float space = 0;
	float inputs[4] = { 0,0,0,0 };

		while (rw.isOpen())
		{
	
			sf::Event e;
			while (rw.pollEvent(e))
			{
				if (e.type == sf::Event::Closed)
				{
					rw.close();
				}
				if (e.type == sf::Event::Resized)
				{
					sh.setUniform("iResolution", Glsl::Vec2(rw.getView().getSize().x, rw.getView().getSize().y));
					fallShader.setUniform("iResolution", Glsl::Vec2(rw.getView().getSize().x, rw.getView().getSize().y));
					mixShader.setUniform("iResolution", Glsl::Vec2(rw.getView().getSize().x, rw.getView().getSize().y));
				}
				if (e.type == sf::Event::KeyPressed)
				{
					if (e.key.code == sf::Keyboard::A)
					{
						inputs[0] += -0.01;
					}
					if (e.key.code == sf::Keyboard::D)
					{
						inputs[1] += 0.01;
					}
					if (e.key.code == sf::Keyboard::W)
					{
						inputs[2] += 0.01;
					}
					if (e.key.code == sf::Keyboard::S)
					{
						inputs[3] += -0.01;
					}
					if (e.key.code == sf::Keyboard::Backspace)
					{
						space = 1;
					}
				}
				if (e.type == sf::Event::KeyReleased)
				{
					if (e.key.code == sf::Keyboard::A)
					{
						inputs[0] -= -0.01;
					}
					if (e.key.code == sf::Keyboard::D)
					{
						inputs[1] -= 0.01;
					}
					if (e.key.code == sf::Keyboard::W)
					{
						inputs[2] -= 0.01;
					}
					if (e.key.code == sf::Keyboard::S)
					{
						inputs[3] -= -0.01;
					}
					if (e.key.code == sf::Keyboard::Backspace)
					{
						space = 0;
					}
				}
			}
			sf::Time elapsed2 = clock.getElapsedTime();
			float t = elapsed2.asSeconds();
			sh.setUniform("iSpace", space);
			fallShader.setUniform("iTime", t);
			fallShader.setUniform("iInput", Glsl::Vec4(inputs[0], inputs[1], inputs[2], inputs[3]));

			rt.draw(rr, &fallShader);
			rt.display();


			Texture tt = rt.getTexture();		
			sh.setUniform("iFallTexture", tt);
			sh.setUniform("iTexture", prints);
			rt.draw(rr, &sh);
			rt.display();
			prints = rt.getTexture();
			


			mixShader.setUniform("iFall", tt);
			mixShader.setUniform("iPrints", prints);
			rw.draw(rr, &mixShader);

			rw.display();
		}
}
