#include "Line.h"

Line::Line()
{}
Line::Line(glm::vec2 start, glm::vec2 end, float pointSize)
{
	this->start = start;
	this->end = end;
	this->pointSize = pointSize;
}
float Line::max(float x, float y)
{
    if (x < y) {
        return x;
    }
    return x;
}

float Line::min(float x, float y)
{
    if (x > y) {
        return y;
    }
    return x;
}


bool Line::thc(float x, float y)
{
    float z,  w,  a,  b;
    z = start.x;
    w = start.y;
    a = end.x;
    b = end.y;
    float k, c;
    
    if (z == x) {
        return (a == x && b >= min(y, w) && x <= max(y, w));
    }

    k = (w - y) / (z - x);
    c = y - k * x;


    float l1 = (x - z) / (a - z);
    float l2 = (y - w) / (b - w);
    if (abs(l1 - l2) < pointSize / 720)
        return true;
    else
        return false;
   // return b == a * k + c;
}
void Line::draw()
{
    glPointSize(pointSize);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glBegin(GL_POINTS);
    glColor3f(0.0, 0.0, 0.0);

    glm::vec2 currentPoint = start;
    float dx = abs(end.x - currentPoint.x);
    float stepX = (currentPoint.x < end.x) ? pointSize / 720 : -pointSize / 720;
    float dy = abs(end.y - currentPoint.y);
    float stepY = (currentPoint.y < end.y) ? pointSize / 720 : -pointSize / 720;

    float error = (dx > dy ? dx : -dy) / 2;
    while (true)
    {
        if (abs(currentPoint.x - end.x) < pointSize / 720 && abs(currentPoint.y - end.y) < pointSize / 720)
            break;
     /*   float dx = end.x - currentPoint.x;
        float dy = end.y - currentPoint.y;*/

        float e2 = error;
        if (e2 < dy)
        {

            error += dx;
            currentPoint.y += stepY;
        }

        if (e2 > -dx)
        {
            error -= dy;
            currentPoint.x += stepX;

        }
        glVertex2f(currentPoint.x, currentPoint.y);
    }

    //for (size_t i = 0; i < positions.size(); i++)
    //{
    //    
    //    //glVertex2f(positions[i].x, positions[i].y);

    //}
    glEnd();
    glFlush();
}
