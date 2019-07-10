#include <iostream>
#include <memory>
#include <string>
#include <sstream>
#include <cmath>

#include "../Time.h"
#include "../Vector3.h"
#include "Particle.h"


Particle::Particle():
	m_mass(1.0), m_charge(0.0), m_spin(0.0),
	m_pos{0.0, 0.0, 0.0}, m_speed{0.0, 0.0, 0.0}, m_forces {0.0, 0.0, 0.0},
	m_idParticle(m_nbParticles)
{
	//std::cout<<m_posX<<" ; "<<m_posY<<" ; "<<m_posZ<<'\t';
	m_nbParticles++;
}

Particle::Particle(double mass, double charge, double spin, double posX, double posY, double posZ) :
	m_mass(mass), m_charge(charge), m_spin(spin),
	m_pos{posX, posY, posZ}, m_speed {0.0, 0.0, 0.0}, m_forces {0.0, 0.0, 0.0},
	m_idParticle(m_nbParticles)
{
	//std::cout<<m_posX<<" ; "<<m_posY<<" ; "<<m_posZ<<'\t';
	m_nbParticles++;
}

unsigned int Particle::m_nbParticles {0};


Particle::~Particle() {
}



Vector3 Particle::getPos() const { return m_pos; }
std::string Particle::getPosS() const {
	std::ostringstream streamObj;
	streamObj<<m_pos.x<<";"<<m_pos.y<<";"<<m_pos.z;
	return streamObj.str();
}

Vector3 Particle::getSpeed() const {return m_speed;}
double Particle::getGamma() const {return m_gamma;}

Vector3 Particle::getForces() const {return m_forces;}
double Particle::getCharge() const {return m_charge;}
double Particle::getMass() const {return m_mass;}
double Particle::getSpin() const {return m_spin;}

unsigned int Particle::getNbParticles() const {return m_nbParticles;}
unsigned int Particle::getID() const { return m_idParticle; }



void Particle::setPos(Vector3 pos) {m_pos = pos;}
void Particle::setPos(double X, double Y, double Z) {
	m_pos.x = X;
	m_pos.y = Y;
	m_pos.z = Z;
}
void Particle::setSpeed(Vector3 speed) { m_speed = speed; }
void Particle::setSpeed(double X, double Y, double Z) {
	m_speed.x = X;
	m_speed.y = Y;
	m_speed.z = Z;
}

void Particle::setGamma(double g) {m_gamma = g;}
void Particle::setSpin(float spin) {m_spin = spin;}

void Particle::addForces(Vector3 forces) {
	m_forces.x = m_forces.x + forces.x;
	m_forces.y = m_forces.y + forces.y;
	m_forces.z = m_forces.z + forces.z;
}
void Particle::addForces(double X, double Y, double Z) {
	m_forces.x = m_forces.x+X; 
	m_forces.y = m_forces.y+Y;
	m_forces.z = m_forces.z+Z;
}



/******METHODES******/


Vector3 Particle::distance3(std::shared_ptr<Particle> other) const {
	Vector3 posB(other->getPos());
	return Vector3{m_pos.x-posB.x, m_pos.y-posB.y, m_pos.z-posB.z,};
}


Vector3 Particle::forceGravitational3(std::shared_ptr<Particle> other) const {
	const double constGravitation {-6.67384e-11};
	double otherMass = other->getMass();
	Vector3 dist3 = distance3(other);
	double r = dist3.length();
	double R = pow(r, 3.0);
	return Vector3{(constGravitation * m_mass*otherMass * dist3.x) / R,
					(constGravitation * m_mass*otherMass * dist3.y) / R,
					(constGravitation * m_mass*otherMass * dist3.z) / R,};
}



Vector3 Particle::forceCoulomb3(std::shared_ptr<Particle> other) const {
	const double kCoulomb {8.9875517873681764e9};
	double otherCharge = other->getCharge();
	Vector3 dist3 = distance3(other);
	double r = dist3.length();
	double R = pow(r, 3.0);
	return Vector3 {(kCoulomb * m_mass*otherCharge * dist3.x) / R,
					(kCoulomb * m_mass*otherCharge * dist3.y) / R,
					(kCoulomb * m_mass*otherCharge * dist3.z) / R,};
}




void Particle::interaction(std::shared_ptr<Particle> other) {


	Vector3 F {forceCoulomb3(other)+forceGravitational3(other)};

	//Computes and apply the different interactions.
	addForces(F);
	other->addForces((-1.0)*(F));
}

void Particle::convertForceSpeed(Time const& time) {
	//Euler method
	if (m_mass!=0.0){
		m_speed.x = ((time.getActualTime()-time.getPreviousTime())*m_forces.x/m_mass)+m_speed.x;
		m_speed.y = ((time.getActualTime()-time.getPreviousTime())*m_forces.y/m_mass)+m_speed.y;
		m_speed.z = ((time.getActualTime()-time.getPreviousTime())*m_forces.z/m_mass)+m_speed.z;
	}
}

void Particle::convertSpeedPosition(Time const& time) {
	//Euler again...
	m_pos.x += (time.getActualTime()-time.getPreviousTime())*m_speed.x;
	m_pos.y += (time.getActualTime()-time.getPreviousTime())*m_speed.y;
	m_pos.z += (time.getActualTime()-time.getPreviousTime())*m_speed.z;
}


void Particle::resetForces(){
	m_forces.reset();
}
void Particle::resetSpeed() {
	m_speed.reset();
}


void Particle::newGamma() {
	//Calculate mass according to speed
	setGamma(1/(sqrt(1-(pow(m_speed.length()/299792458.0 , 2)))));
}

std::string Particle::toString() const {
	//Description
	std::ostringstream streamObj;
	//Add double to stream
	streamObj <<"Particle nb "+std::to_string(m_idParticle)+
		" : position("  <<m_pos.x<< ", " << m_pos.y<< ", " << m_pos.z<<
		"), speed(" << m_speed.x <<", " <<m_speed.y<< ", "<<m_speed.z<<
		"), force(" <<m_forces.x<< ", " << m_forces.y<< ", " << m_forces.z<<");";

	// Get string from output string stream
	return streamObj.str();
}
