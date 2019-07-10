#include <iostream>
#include <memory>
#include <random>
#include <vector>
#include <sstream>
#include <fstream>
#include <SFML/Graphics.hpp>
#include <chrono>

#include "main.h"
#include "Time.h"
#include "Vector3.h"
#include "./Particles/Particle.h"
#include "./Particles/FluidPart.h"

#define WIDTH 1280
#define HEIGHT 720
#define FONTSIZE 12

int main() {
	std::random_device rd {};

	int nbParticles {300};
	double kc {4.0e1};	//1.0e1
	double kr {4.0e2};	//1.2e2
	double kf {4.0e-2};	//1.0e-2

	double kc_0 {6.0e1};	//1.0e1
	double kr_0 {7.2e1};	//1.2e2

	std::vector<std::shared_ptr<FluidPart>> particles;	//Container for all particles

	// Declare and load a font
	sf::Font font;
	font.loadFromFile("arial.ttf");
	sf::Text textDuration("0.0 s", font); textDuration.setCharacterSize(FONTSIZE);
	sf::Text textSpeed("Speed : x 1", font); textSpeed.setCharacterSize(FONTSIZE);
	sf::Text textNbPart(std::to_string(nbParticles)+" particles", font); textNbPart.setCharacterSize(FONTSIZE);

	Time time = Time {0.0};

	/*****CREATE FLUID PARTICLES*****/
	for (int i {0}; i<nbParticles; i++) {
		//Random coordinates
		double nbAleaX {(rd()%10000) /50.0 +WIDTH/2};
		double nbAleaY {(rd()%10000) /50.0 +HEIGHT/2};
		double nbAleaZ {(rd()%10000) /50.0};

		//Add a fluid particle
		particles.push_back(std::shared_ptr<FluidPart>(new FluidPart {nbAleaX, nbAleaY, nbAleaZ, kc, kr, kf}));
	}
	particles[0]->setKc(kc_0);
	particles[0]->setKr(kr_0);


	sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Fluid Simulation");
	sf::CircleShape circle(2);	circle.setFillColor(sf::Color::Cyan);
	sf::CircleShape circleRef(2);	circleRef.setFillColor(sf::Color::White);
	

	int speedFactor(0);
	double Ek(0.0);	//Kinetic energy of one particle
	double Ep(0.0);	//Potential energy of one particle



	while (window.isOpen()) {
		/*****SIMULATION*****/
		auto prevTime = std::chrono::high_resolution_clock::now();
		analyse(particles, time); //Particles behavior simulation

		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window.close();
			if (event.type == sf::Event::KeyPressed){
				if (event.key.code == sf::Keyboard::Escape){
					window.close();
				}
				if (event.key.code == sf::Keyboard::Add) {
					if (speedFactor<9)speedFactor++;
				}
				if (event.key.code == sf::Keyboard::Subtract) {
					speedFactor--;
				}
			}
		}
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
			sf::Vector2i locPos = sf::Mouse::getPosition(window);
			if (locPos.x<WIDTH && locPos.x>0 && locPos.y>0 && locPos.y<HEIGHT){
				particles[0]->setPos(locPos.x, HEIGHT - locPos.y, particles[0]->getPos().z);
				particles[0]->resetForces();
				particles[0]->resetSpeed();
			}
		}



		/*** DRAWING ***/
		window.clear();
		textDuration.setString(std::to_string(time.getActualTime())+" s");	window.draw(textDuration);
		textSpeed.setString("Speed : x "+std::to_string(pow(10, speedFactor/3.0)));

		float tempLog;	//Store logarithmic speed of a particle, used to color the particle
		Vector3 tempPos;	//Store position of a particle

		sf::Color tempColor(0,0,255);
		
		for (unsigned int i {1}; i<particles.size(); i++) {
			tempPos = particles[i]->getPos();
			tempLog = log( particles[i]->getSpeed().length() )*40;
				if (tempLog<0){tempLog = 0;}	//tempLog must be always positive  :)
			tempColor.g = (int)tempLog;

			circle.setPosition((int)(tempPos.x), HEIGHT-(int)(tempPos.y));
			circle.setFillColor(tempColor);

			window.draw(circle);

			particles[i] ->setEp(0.0);	//Reset potential energy for the particle
		}

		//Ref particle:
		tempPos = particles[0]->getPos();
		circleRef.setPosition((int)(tempPos.x), HEIGHT-(int)(tempPos.y));
		window.draw(circleRef);
		particles[0]->setEp(0.0);


		textSpeed.setPosition(0, FONTSIZE);	window.draw(textSpeed);
		textNbPart.setPosition(0, 2*FONTSIZE);	window.draw(textNbPart);


		window.display();
		double elapsedTime = (std::chrono::duration_cast<std::chrono::milliseconds>(
			std::chrono::high_resolution_clock::now() -prevTime)).count();	//In millisec
		time.nextTime(elapsedTime/1000.0 * pow(10, speedFactor/3.0));   //Next timestep (in seconds)

	}

	return EXIT_SUCCESS;
}


void analyse(std::vector<std::shared_ptr<FluidPart>> const& particles, Time time) {
	//For each particle...
	for (unsigned int i {0}; i<particles.size(); i++) {
		//Apply changes between THIS particle and [every particle except THIS ONE]

		for (unsigned int j {i+1}; j<particles.size(); j++) {
			particles[i]->interaction(particles[j]);
		}
	}

	for (unsigned int i {0}; i<particles.size(); i++) {
		particles[i]->convertForceSpeed(time);
		particles[i]->convertSpeedPosition(time);

		//Reset forces
		particles[i]->resetForces();
	}
}