#pragma once
#include <memory>
#include <string>

#include "Particle.h"

class FluidPart : public Particle
{
public:
	FluidPart();
	FluidPart(double const& PosX, double const& PosY, double const& PosZ);
	FluidPart(double const& PosX, double const& PosY, double const& PosZ,
			  double const& kc, double const& kr, double const& kf);
	virtual ~FluidPart();

	unsigned int getNumber() const;

	virtual std::string toString() const;
	
	double getEk() const;	//Returns Kinetic energy
	double getEp() const;	//Returns Potential energy

	double getKc() const;
	double getKr() const;


	void setEp(double const& Ep);

	void setKc(double const& kc);
	void setKr(double const& kr);


	//Cohesion
	Vector3 forceC3(std::shared_ptr<FluidPart> other, double const& r) const;
	
	//Repulsion
	Vector3 forceR3(std::shared_ptr<FluidPart> other, double const& r) const;

	//Friction, dampening
	Vector3 forceF3() const;


	void interaction(std::shared_ptr<FluidPart> other);

private:
	static unsigned int m_nbFluidPart;
	const unsigned int m_number;

	double m_Ep_sum;	//Sum of potential energies from other particles

	double m_kc;	//Cohesion constant
	double m_kr;	//Repulsion constant
	double m_kf;	//Friction coef
};

