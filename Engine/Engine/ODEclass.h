#pragma once

#ifndef _ODECLASS_H_
#define _ODECLASS_H_

#define dDOUBLE

#include <ode/odeconfig.h>
#include <assert.h>

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

#include <ode/ode.h>
#include <drawstuff/drawstuff.h>

//#ifndef DRAWSTUFF_TEXTURE_PATH
//#define DRAWSTUFF_TEXTURE_PATH "../ode-0.13/drawstuff/textures"
//#endif

#include "world_geom3.h" // this is our world mesh

// dynamics and collision objects (chassis, 3 wheels, environment)

static dWorldID world;
static dSpaceID space;

static dJointGroupID contactgroup;
static dGeomID world_mesh;


class ODEClass
{
public:
	ODEClass();
	~ODEClass();

	bool Initialize();
	void Update();
	void Shutdown();

	static void nearCallback(void* data, dGeomID object1, dGeomID object2);

	static dGeomID GetGeomID(int index);
	static dReal* GetDirection(int index);

	void SetODE(ODEClass* ode);
	void SetGeoms(dGeomID geomID, int index);
	void SetDirection(dReal* dir, int index);

private:
	dGeomID m_Geoms[8];
	dReal m_Direction[8][3];

};

static ODEClass* ODE;

#endif