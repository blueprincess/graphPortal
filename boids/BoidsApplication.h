/*
 * BoidsApplication.h
 *
 *  Created on: Dec 10, 2011
 *      Author: Guillaume Chatelet
 */

#ifndef BOIDSAPPLICATION_H_
#define BOIDSAPPLICATION_H_

#include "Boids.h"

#include <stein/Application.hpp>

class BoidsApplication : public stein::Application {
public:
    BoidsApplication();
    virtual void animate();
private:
    BoidSimulation m_Simulation;
};

#endif /* BOIDSAPPLICATION_H_ */
