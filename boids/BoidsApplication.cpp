/*
 * BoidsApplication.cpp
 *
 *  Created on: Dec 10, 2011
 *      Author: Guillaume Chatelet
 */

#include "BoidsApplication.h"

#include <stein/Object.hpp>
#include <stein/Tools.hpp>
#include <stein/Builders.hpp>
#include <stein/math/StreamUtils.h>

#include <iostream>

#include <vector>

using namespace std;
using namespace stein;

static inline float frand() {
    return float(rand()) / RAND_MAX;
}

BoidsApplication::BoidsApplication() :
    Application(800, 800), m_Simulation(200) {

    const float size = .06;
    m_Scene.camera.setPerspectiveProjection(-size, size, -size, size, .1, 100);
    m_Scene.camera.setPosition(Vector3f(0, 0, 55));
    m_Scene.setDefaultShaderID(loadProgram("../shaders/simpleShader.glsl"));

    Object &object = m_Scene.createObject(GL_TRIANGLES);
    buildSquare(object);
    
    for (size_t i = 0; i < m_Simulation.boids().size(); ++i) {
        m_Scene.addObjectToDraw(object.id);
        m_Scene.setDrawnObjectColor(i, Color(frand(), frand(), frand()));
    }
}

static inline Matrix4f computeRotation(const Vector3f& velocity, Vector3f up) {
    Vector3f vectZ(velocity);
    vectZ.normalize();
    up.normalize();
    Vector3f vectX(up.crossP(vectZ));
    vectX.normalize();
    Vector3f vectY(vectZ.crossP(vectX));
    vectY.normalize();

    return Matrix4f(vectX.x, vectX.y, vectX.z, 0, //
        vectY.x, vectY.y, vectY.z, 0, //
        vectZ.x, vectZ.y, vectZ.z, 0, //
        0, 0, 0, 1);
}

void BoidsApplication::animate() {
    m_Simulation.update();
    const vector<Boid> &boids(m_Simulation.boids());
    for (size_t i = 0; i < boids.size(); ++i) {
        const Boid &boid = boids[i];

        m_Scene.setDrawnObjectModel(i, translation(boid.position) * computeRotation(boid.velocity, Vector3f(1, 1, 0)));
    }
}
