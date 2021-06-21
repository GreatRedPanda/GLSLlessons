#include "SoftBody.h"

#define clamp(x,y,z) (((x<y)?y:x)>z)?z:((x<y)?y:x)

#define distance(a,b) (a.x-b.x)*(a.x-b.x)+(a.y-b.y)*(a.y-b.y)
std::vector<SoftBody *> SoftBody::bodies = std::vector<SoftBody *>();

SoftBody::SoftBody():SoftBody(glm::vec2(0,0))
{
   // velocity.x = -0.01;
}

SoftBody::SoftBody(glm::vec2 center)
{
    bodies.push_back(this);
    centerPosition = center;
    vel = -0.0001;
    float radius = bodyRadius;
    for (int i = 0; i < 360; i += 10)
    {
        float angle = i * 3.1415 / 180;
        float angle2 = (i - 1) * 3.1415 / 180;

        float x2 = centerPosition.x + radius * cos(angle);
        float y2 = centerPosition.y + radius * sin(angle);

        positions.push_back(glm::vec2(x2, y2));
        velocities.push_back(glm::vec2(0, 0));
    }


    for (size_t i = 0; i < positions.size(); i++)
    {
        for (size_t j = 0; j < positions.size(); j++)
        {
            sprLen[i][j] = sqrt(distance(positions[i], positions[j]));
        }


    }
    radius = 0.02;

    for (int i = 0; i < positions.size(); i++)
    {
        float angle = i * 10 * 3.1415 / 180;
        float angle2 = (i * 10 - 1) * 3.1415 / 180;

        float x2 = centerPosition.x + radius * cos(angle);
        float y2 = centerPosition.y + radius * sin(angle);

        positions[i] = (glm::vec2(x2, y2));
    }
  
}

void SoftBody::simulate(float deltaTime)
{

    velocity.y += -0.00001;

    glm::vec2 centerOfmass = glm::vec2(0, 0);
    for (size_t i = 0; i < positions.size(); i++)
    {
        centerOfmass += positions[i];
    }
    centerPosition =centerOfmass/ glm::vec2(positions.size(), positions.size());
   
    for (size_t i = 0; i < positions.size(); i++)
    {
        velocities[i] = velocity / glm::vec2(positions.size(), positions.size());           
        for (size_t j = 0; j < positions.size(); j++)
        {

            if (i == j)
                continue;
            glm::vec2 velDif = velocities[i] - velocities[j];
            glm::vec2  dir = positions[j] - positions[i];
            float t = atan2(dir.y, dir.x);
            float dist= sqrt(distance(positions[i], positions[j]));
            float disper = dist-sprLen[i][j];
           
            velocities[i] += glm::vec2(disper * cos(t) * k, disper * sin(t) * k);
         
           // sumVel -= glm::vec2(velDif.x*dist  *k, velDif.y*dist * k);
        }
        checkCollisions(i, bodies);   
        if (positions[i].y + velocities[i].y <= -1 || positions[i].y + velocities[i].y >= 1)
        {
           
            velocity.y += -velocities[i].y/positions.size();
            velocities[i].y = -velocities[i].y;
            velocity.x -= velocity.x * 0.001;
        }
        if (positions[i].x + velocities[i].x <= -1 || positions[i].x + velocities[i].x >= 1)
        {
            velocity.x += -velocities[i].x / positions.size();
            velocities[i].x = -velocities[i].x;
            velocity.x -= velocity.x * 0.001;
           
        }
        checkCollisionsStatic(i, lineTest);
        positions[i] += velocities[i];

    }
}

void SoftBody::draw()
{
  //  glClearColor(1.0, 1.0, 1.0, 1.0);

    glPointSize(4.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glBegin(GL_POINTS);
    glColor3f(1.0, 1.0, 0.0);
    glVertex2f(centerPosition.x, centerPosition.y);
    for (size_t i = 0; i < positions.size(); i++)
    {
        glColor3f(color.x, color.y, color.z);
        glVertex2f(positions[i].x, positions[i].y);

    }
    glEnd();
    glFlush();
}

void SoftBody::checkCollisions(int position, const std::vector<SoftBody *> others)
{
    for (size_t i = 0; i < others.size(); i++)
    {
        if (others[i] == this)
            continue;
        glm::vec2 localVel = glm::vec2(0, 0);

        glm::vec2 nearestOther = checCollision(positions[position] + velocities[position], *others[i], localVel);
        if (nearestOther.x != 0 || nearestOther.y != 0)
        {
            glm::vec2  dir = nearestOther - centerPosition;
            float t = atan2(dir.y, dir.x);
            float dist = sqrt(distance(positions[i], nearestOther));
            velocities[position] -= glm::vec2(dist * dir.x * k * 100, dist * dir.y * k * 100);
            velocity += velocities[position] * glm::vec2(0.5, 0.5) /
                glm::vec2(positions.size(), positions.size());
            velocity.x -= velocity.x * 0.00001;
        }
    }
}
void SoftBody::checkCollisionsStatic(int position,  Line l)
{
    
    bool belongs = l.thc(positions[position].x + velocities[position].x, 
        positions[position].y + velocities[position].y);
    if (belongs)
    {

        velocity.y += -velocities[position].y / positions.size();
        velocities[position].y = -velocities[position].y;
        velocity.x -= velocity.x * 0.001;
    }

}
void SoftBody::checkBounds()
{

}

glm::vec2 SoftBody::checCollision(glm::vec2 curPos, const SoftBody& other, glm::vec2& closestPoint)
{

    for (size_t i = 0; i < other.positions.size(); i++)
    {
        glm::vec2 fp = other.positions[i];
      //  glm::vec2 sp = other.positions[i+ other.positions.size() / 2];
        glm::vec2   sp = other.centerPosition;
        if (curPos.x<=fp.x && curPos.x>=sp.x && curPos.y <= fp.y && curPos.y >= sp.y
            ||
            curPos.x <= fp.x && curPos.x >= sp.x && curPos.y >= fp.y && curPos.y <= sp.y
            
            
            || curPos.x<=sp.x && curPos.x>=fp.x && curPos.y <= fp.y && curPos.y >= sp.y
            || curPos.x <= sp.x && curPos.x >= fp.x && curPos.y >= fp.y && curPos.y <= sp.y
            )
        {
                closestPoint = fp;
       
                return  other.centerPosition;
        }
    }

    return glm::vec2(0, 0);
}
