/* SIMPLE MATHS LIBRARY
PREPARED BY GIZEM KAYAR FOR COMPUTER GRAPHICS COURSE/NEW YORK UNIVERSITY
YEAR: 2022
*/

//#include "pch.h"
#include "Simulation.h"
#define SIGN(x) (x >= 0 ? 1.0 : -1.0)
#include <stdlib.h>
#include "math3d.h"


void Simulation::initializeParticles()
{
	srand(0);

	float x, y, z;
	for (int i = 0; i < noPt; i++)
	{
		Particle  particle;

		//TO DO: Set particle positions and velocities using srand and world positions
        x = float(rand()) / RAND_MAX * 20 - 10;
        y = float(rand()) / RAND_MAX * 10;
        z = float(rand()) / RAND_MAX * 20 - 10;
        particle.setPosition(vector3f(x, y, z));
        
        particle.setVelocity(vector3f(float(rand()) / RAND_MAX*10-5, float(rand()) / RAND_MAX*5, float(rand()) / RAND_MAX*10-5));


		particle.clearForce();

		//TO DO: Compute particle's old position for Verlet integration scheme
        vector3f xt = particle.getPosition();
        vector3f vt = particle.getVelocity();
        vector3f f = particle.getForce();
        
        vector3f oldPosition = xt - vt*timestep;
        particle.setOldPosition(oldPosition);

		particle.setIntegration(EulerCromer);
		particle.setColor(vector3f(0, 0, 255));
//		particle.setPosition(vector3f(particle.getPosition().getX(), (particle.getPosition().getY() + 0.1, 0), particle.getPosition().getZ()));


		particleList.push_back(particle);

	}
}

void Simulation::simulateEuler(Particle* p)
{
	//TO DO
    vector3f xt = p->getPosition();
    vector3f vt = p->getVelocity();
    vector3f f = p->getForce();
    
    vector3f new_x = xt + vt*timestep;
    vector3f new_v = vt + f*(timestep/commonMass);
	
    p->setVelocity(new_v);
    p->setPosition(new_x);
}

void Simulation::simulateEulerCromer(Particle* p)
{
	//TO DO
    vector3f xt = p->getPosition();
    vector3f vt = p->getVelocity();
    vector3f f = p->getForce();
    
    vector3f new_v = vt + f*(timestep/commonMass);
    vector3f new_x = xt + new_v*timestep;
    
    p->setVelocity(new_v);
    p->setPosition(new_x);
}

void Simulation::simulateVerlet(Particle* p)
{
	//TO DO
    vector3f xt = p->getPosition();
    vector3f xold = p->getOldPosition();
    vector3f vt = p->getVelocity();
    vector3f f = p->getForce();

    vector3f new_x = xt*2 - xold + f*(timestep*timestep/commonMass);
    vector3f new_v = (new_x - xt) / timestep;
    if (new_x.getX() <= -halfWorld.getX() || new_x.getX() >= halfWorld.getX())
    {
        xt.setX(SIGN(xt.getX()) * halfWorld.getX() - worldColldConst*xt.getX() + worldColldConst*new_x.getX());
    }

    if (new_x.getY() <= -halfWorld.getY() || new_x.getY() >= halfWorld.getY())
    {
        xt.setY(SIGN(xt.getY()) * halfWorld.getY() - worldColldConst*xt.getY() + worldColldConst*new_x.getY());
    }

    if (new_x.getZ() <= -halfWorld.getZ() || new_x.getZ() >= halfWorld.getZ())
    {
        xt.setZ(SIGN(xt.getZ()) * halfWorld.getZ() - worldColldConst*xt.getZ() + worldColldConst*new_x.getZ());
    }
    
    p->setVelocity(new_v);
    p->setPosition(new_x);
    p->setOldPosition(xt);
}

void Simulation::clearForces()
{
	for (int i = 0; i < noPt; i++)
		particleList[i].clearForce();
}

void Simulation::destroyParticles()
{
	particleList.clear();
}

void Simulation::solveWorldCollision()
{
	vector3f tempVel;
	vector3f tempPos;

	for (int i = 0; i < noPt; i++)
	{
		tempVel = particleList[i].getVelocity();
		tempPos = particleList[i].getPosition();

		if (particleList[i].getPosition().getX() <= -halfWorld.getX() || particleList[i].getPosition().getX() >= halfWorld.getX())
		{
			tempVel.setX(tempVel.getX() * -worldColldConst);
			tempPos.setX(SIGN(tempPos.getX()) * halfWorld.getX());
		}

		if (particleList[i].getPosition().getY() <= -halfWorld.getY() || particleList[i].getPosition().getY() >= halfWorld.getY())
		{
			tempVel.setY(tempVel.getY() * -worldColldConst);
			tempPos.setY(SIGN(tempPos.getY()) * halfWorld.getY());
		}

		if (particleList[i].getPosition().getZ() <= -halfWorld.getZ() || particleList[i].getPosition().getZ() >= halfWorld.getZ())
		{
			tempVel.setZ(tempVel.getZ() * -worldColldConst);
			tempPos.setZ(SIGN(tempPos.getZ()) * halfWorld.getZ());
		}

		particleList[i].setVelocity(tempVel);
		particleList[i].setPosition(tempPos);
	}
}

void Simulation::computeSystemEnergies()
{
	//TO DO
    kinEn = 0.0;
    potEn = 0.0;
    for (int j = 0; j < noPt; j++) {
        Particle p = particleList[j];
        vector3f v = p.getVelocity();
        kinEn += 0.5*commonMass*v.lengthSquare();
        
        float h = p.getPosition().getY() + halfWorld.getY();
        potEn += commonMass*9.81*h;
    }
}
