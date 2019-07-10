#include <iostream>
#include <memory>
#include <sstream>

#include "Time.h"



Time::Time():
	m_initialTime(0.0), m_finalTime(1.0), m_deltaTime(1.0E-1),
	m_actualTime(0.0), m_previousTime(m_actualTime),
	m_nbMeasuresTotal(static_cast<unsigned int>((m_finalTime - m_initialTime)/m_deltaTime)),
	m_nbMeasuresActual(0), m_plannedStop(false)
{
	std::cout<<"Creation of a time object : initial = " << m_initialTime << " s, final = "<< m_finalTime <<" s, step = " << m_deltaTime << " s. Nb measures : " << m_nbMeasuresTotal<<std::endl;
}

Time::Time(double const& iTime, double const& fTime, double const& delta):
	m_initialTime(iTime), m_finalTime(fTime), m_deltaTime(delta),
	m_actualTime(iTime), m_previousTime(m_actualTime),
	m_nbMeasuresTotal(static_cast<unsigned int>((m_finalTime - m_initialTime)/m_deltaTime)),
	m_nbMeasuresActual(0), m_plannedStop(true)
{
	std::cout<<"Creation of a time object : initial = " << m_initialTime << " s, final = "<< m_finalTime <<" s, step = " << m_deltaTime << " s. Nb measures : " << m_nbMeasuresTotal<<std::endl;
}

Time::Time(double const& iTime, double const& delta):
	m_initialTime(iTime), m_finalTime(-1), m_deltaTime(delta),
	m_actualTime(iTime), m_previousTime(m_actualTime),
	m_nbMeasuresTotal(0), m_nbMeasuresActual(0), m_plannedStop(false)
{
	std::cout<<"Creation of a time object : initial = " << m_initialTime << " s, step = " << m_deltaTime <<" s"<<std::endl;
}

Time::Time(double const& iTime) :
	m_initialTime(iTime), m_finalTime(-1), m_deltaTime(0),
	m_actualTime(iTime), m_previousTime(m_actualTime),
	m_nbMeasuresTotal(0), m_nbMeasuresActual(0), m_plannedStop(false)
{
	std::cout<<"Creation of a time object : initial = " << m_initialTime << " s"<<std::endl;
}


Time::~Time() {
}


double Time::getInitialTime() const {return m_initialTime;}
double Time::getPreviousTime() const {return m_previousTime;}
double Time::getActualTime() const {return m_actualTime;}
double Time::getFinalTime() const {return m_finalTime;}
double Time::getDeltaTime() const {return m_deltaTime;}
unsigned int Time::getNbMeasuresTotal() const {return m_nbMeasuresTotal;}
unsigned int Time::getNbMeasuresActual() const {return m_nbMeasuresActual;}
bool Time::getPlannedStop() const { return m_plannedStop; }



void Time::setPreviousTime(double const& timeP) {m_previousTime = timeP;}
void Time::setActualTime(double const& timeA) {m_actualTime = timeA;}
void Time::setPlannedStop(bool const& stop) { m_plannedStop = stop; }



double Time::nextTime() {
	if ((m_nbMeasuresActual>=m_nbMeasuresTotal) && (m_plannedStop)){
		//If time reached end-time
		return m_actualTime;
	}

	//One time step ahead
	setPreviousTime(m_actualTime);
	setActualTime(m_actualTime + m_deltaTime);

	m_nbMeasuresActual++;
	return m_actualTime;
}

double Time::nextTime(double const& step) {
	if ((m_nbMeasuresActual>=m_nbMeasuresTotal) && (m_plannedStop)) {
		//If time reached end-time
		return m_actualTime;
	}

	//One time step ahead
	setPreviousTime(m_actualTime);
	setActualTime(m_actualTime + step);

	m_nbMeasuresActual++;
	return m_actualTime;
}