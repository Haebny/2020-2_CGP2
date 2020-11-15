#include "ODEclass.h"

ODEClass::ODEClass()
{
}

ODEClass::~ODEClass()
{
}

bool ODEClass::Initialize()
{
    bool result;

	dMass m;
	dMatrix3 R;// setup pointers to drawstuff callback functions

	result = dInitODE2(0);
	if (!result)
	{
		return false;
	}

	world = dWorldCreate();
	space = dHashSpaceCreate(0);
	contactgroup = dJointGroupCreate(0);
	
	dWorldSetGravity(world, 0, -9.8, 0);	// world의 중력 설정 (y축의 음의 방향으로 지구 중력 설정: -9.8)
	dWorldSetQuickStepNumIterations(world, 12);

	// Create a static world using a triangle mesh that we can collide with.
	int numv = sizeof(world_vertices) / (3 * sizeof(float));
	int numi = sizeof(world_indices) / sizeof(dTriIndex);
	printf("numv=%d, numi=%d\n", numv, numi);
	dTriMeshDataID Data = dGeomTriMeshDataCreate();

	dGeomTriMeshDataBuildSingle
	(
		Data,
		world_vertices,
		3 * sizeof(float),
		numv,
		world_indices,
		numi,
		3 * sizeof(dTriIndex)
	);

	world_mesh = dCreateTriMesh(space, Data, 0, 0, 0);
	dGeomSetPosition(world_mesh, 0, 0, 0.5);
	dRFromAxisAndAngle(R, 0, 1, 0, 0.0);
	dGeomSetRotation(world_mesh, R);

    return true;
}

void ODEClass::Update()
{
	dSpaceCollide(space, 0, &nearCallback);
	dWorldStep(world, 0.02);
	dJointGroupEmpty(contactgroup);
}

void ODEClass::Shutdown()
{
	dGeomDestroy(world_mesh);

	dSpaceDestroy(space);
	dWorldDestroy(world);
	dCloseODE();
	(void)world_normals; // get rid of compiler warnings
}

// this is called by dSpaceCollide when two objects in space are
// potentially colliding.
void ODEClass::nearCallback(void* data, dGeomID object1, dGeomID object2)
{
	//dBodyID body1 = dGeomGetBody(object1);
	//dBodyID body2 = dGeomGetBody(object2);

	assert(object1);
	assert(object2);

	if (dGeomIsSpace(object1) || dGeomIsSpace(object2))
	{
		// colliding a space with something
		dSpaceCollide2(object1, object2, data, &nearCallback);
		// Note we do not want to test intersections within a space,
		// only between spaces.
		return;
	}

	//// Return 1 if the two bodies are connected together by
	//// a joint that does not have type @arg{ joint_type }, otherwise return 0.
	//if (dAreConnectedExcluding(body1, body2, dJointTypeContact))
	//{
	//	return;
	//}

	const int N = 32;
	dContact contact[N];
	int n = dCollide(object1, object2, N, &(contact[0].geom), sizeof(dContact));
	if (n > 0)
	{
		for (int i = 0; i < n; i++)
		{
			contact[i].surface.slip1 = 0.7;
			contact[i].surface.slip2 = 0.7;
			contact[i].surface.mode = dContactSoftERP | dContactSoftCFM | dContactApprox1 | dContactSlip1 | dContactSlip2;
			contact[i].surface.mu = 50.0; // was: dInfinity
			contact[i].surface.soft_erp = 0.96;
			contact[i].surface.soft_cfm = 0.04;
			dJointID c = dJointCreateContact(world, contactgroup, &contact[i]);
			dJointAttach(c,
				dGeomGetBody(contact[i].geom.g1),
				dGeomGetBody(contact[i].geom.g2));
		}
	}
}

dGeomID ODEClass::GetGeomID(int index)
{
	ODEClass* ode = ODE;
	return ode->m_Geoms[index];
}

dReal* ODEClass::GetDirection(int index)
{
	ODEClass* ode = ODE;
	return ode->m_Direction[index];
}

void ODEClass::SetODE(ODEClass* ode)
{
	ODE = ode;
	return;
}

void ODEClass::SetGeoms(dGeomID gID, int index)
{
	m_Geoms[index] = gID;
	return;
}

void ODEClass::SetDirection(dReal* dir, int index)
{
	for (int i = 0; i < 3; i++)
	{
		m_Direction[index][i] = dir[i];
	}

	return;
}
