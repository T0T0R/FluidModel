#include <iostream>
#include <memory>
#include <string>
#include <sstream>

#include "../Vector3.h"
#include "Particle.h"
#include "FluidPart.h"



FluidPart::FluidPart():
	Particle(1.0, 0.0, 0.0, 0.0, 0.0, 0.0), m_number(m_nbFluidPart),
	m_kc(0.0), m_kr(0.0), m_kf(0.0)
{
	m_nbFluidPart++;
}

FluidPart::FluidPart(double const& PosX, double const& PosY, double const& PosZ) :
	Particle(1.0, 0.0, 0.0, PosX, PosY, PosZ), m_number(m_nbFluidPart),
	m_kc(0.0), m_kr(0.0), m_kf(0.0)
{
	m_nbFluidPart++;
}

FluidPart::FluidPart(double const& PosX, double const& PosY, double const& PosZ,
					 double const& kc, double const& kr, double const& kf):
	Particle(1.0, 0.0, 0.0, PosX, PosY, PosZ), m_number(m_nbFluidPart),
	m_kc(kc), m_kr(kr), m_kf(kf)
{
	m_nbFluidPart++;
}

unsigned int FluidPart::m_nbFluidPart {0};

FluidPart::~FluidPart() {
}



unsigned int FluidPart::getNumber() const { return m_number; }

std::string FluidPart::toString() const {
	//Description
	std::ostringstream streamObj;
	streamObj<< "\tFluidParticle nb " + std::to_string(m_number);
	return Particle::toString()+'\n'+streamObj.str();
}



double FluidPart::getEk() const { return pow(m_speed.length(), 2.0)* m_mass/2; }
double FluidPart::getEp() const { return m_Ep_sum; }

double FluidPart::getKc() const { return m_kc; }
double FluidPart::getKr() const { return m_kr; }



void FluidPart::setEp(double const& Ep) { m_Ep_sum = Ep; }

void FluidPart::setKc(double const& kc) { m_kc = kc; }
void FluidPart::setKr(double const& kr) { m_kr = kr; }


Vector3 FluidPart::forceC3(std::shared_ptr<FluidPart> other, double const& r) const {
	Vector3 dist = distance3(other);
	double o_kc = other->getKc();
	return Vector3{(-m_kc*o_kc * dist.x) / pow(r, 2),
					(-m_kc*o_kc * dist.y) / pow(r, 2),
					(-m_kc*o_kc * dist.z) / pow(r, 2)};
}


Vector3 FluidPart::forceR3(std::shared_ptr<FluidPart> other, double const& r) const {
	Vector3 dist = distance3(other);
	double o_kr = other->getKr();
	return Vector3 {(m_kr*o_kr * dist.x) / pow(r, 3),
					(m_kr*o_kr * dist.y) / pow(r, 3),
					(m_kr*o_kr * dist.z) / pow(r, 3)};
}



Vector3 FluidPart::forceF3() const {
	return Vector3 {-(m_speed.x)*m_kf,
					-(m_speed.y)*m_kf,
					-(m_speed.z)*m_kf};
}



void FluidPart::interaction(std::shared_ptr<FluidPart> other) {
	double r = distance3(other).length();

	Vector3 F {forceC3(other, r)+forceR3(other, r)};
	Vector3 Ff {forceF3()};	//friction forces

	//Computes and apply the different interactions.
	addForces(F+Ff);
	other->addForces((-1.0)*(F+Ff));

	//Potential energy :
	double Ep = -(m_kc*log(r)+m_kr/r);
	m_Ep_sum += Ep;
	other -> setEp(other->getEp() + Ep);
}