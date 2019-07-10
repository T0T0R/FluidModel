#pragma once

#include <memory>

#include "../Vector3.h"
#include "../Time.h"
class Particle
{
public:
	Particle();
	Particle(double mass, double charge, double spin, double posX, double posY, double posZ);
	virtual ~Particle();


	double getCharge() const;
	double getMass() const;
	double getSpin() const;

	Vector3 getPos() const;
	std::string getPosS() const;
	void setPos(Vector3 pos);
	void setPos(double X, double Y, double Z);

	Vector3 getSpeed() const;
	void setSpeed(Vector3 speed);
	void setSpeed(double X, double Y, double Z);
	double getGamma() const;

	Vector3 getForces() const;



	unsigned int getNbParticles() const;
	unsigned int getID() const;



	void addForces(Vector3 forces);
	void addForces(double X, double Y, double Z);


	Vector3 distance3(std::shared_ptr<Particle> other) const;

	Vector3 forceGravitational3(std::shared_ptr<Particle> other) const;

	Vector3 forceCoulomb3(std::shared_ptr<Particle> other) const;


	void interaction(std::shared_ptr<Particle> other);

	void convertForceSpeed(Time const& time);
	void convertSpeedPosition(Time const& time);

	void resetForces();
	void resetSpeed();

	void newGamma();

	virtual std::string toString() const;

protected:
	void setGamma(double g);

	void setSpin(float spin);

	const double m_mass;
	double m_charge;
	double m_spin;

	Vector3 m_pos;
	Vector3 m_speed;
	Vector3 m_forces;

	double m_gamma;	//Lorentz factor

	static unsigned int m_nbParticles;
	const unsigned int m_idParticle;
};
