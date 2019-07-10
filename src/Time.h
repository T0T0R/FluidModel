#pragma once

#include <memory>

class Time
{
public:
	Time();
	Time(double const& iTime, double const& fTime, double const& delta);
	Time(double const& iTime, double const& delta);
	Time(double const& iTime);
	~Time();

	double getInitialTime() const;
	double getPreviousTime() const;
	double getActualTime() const;
	double getFinalTime() const;
	double getDeltaTime() const;
	unsigned int getNbMeasuresTotal() const;
	unsigned int getNbMeasuresActual() const;
	bool getPlannedStop() const;

	void setPreviousTime(double const& tempsP);
	void setActualTime(double const& tempsA);
	void setPlannedStop(bool const& stop);

	double nextTime();
	double nextTime(double const& step);

protected:
	double m_initialTime;
	double m_finalTime;
	double m_deltaTime; //Timestep
	double m_actualTime;
	double m_previousTime;
	bool m_plannedStop;	//If true, stop time is defined and Time object wont evolve after

	const unsigned int m_nbMeasuresTotal;
	unsigned int m_nbMeasuresActual;
};

