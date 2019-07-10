#pragma once

#include <memory>
#include <vector>

#include "Time.h"
#include "./Particles/Particle.h"
#include "./Particles/FluidPart.h"


void analyse(std::vector<std::shared_ptr<FluidPart>> const& particles, Time time);
