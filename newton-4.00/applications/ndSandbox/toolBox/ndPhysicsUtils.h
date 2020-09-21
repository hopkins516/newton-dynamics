/* Copyright (c) <2003-2019> <Newton Game Dynamics>
* 
* This software is provided 'as-is', without any express or implied
* warranty. In no event will the authors be held liable for any damages
* arising from the use of this software.
* 
* Permission is granted to anyone to use this software for any purpose,
* including commercial applications, and to alter it and redistribute it
* freely
*/

#ifndef __PHYSICS_UTIL__
#define __PHYSICS_UTIL__

#define DEMO_GRAVITY  dFloat32(-10.0f)
//#define DEMO_GRAVITY  dFloat32(0.0f)

#ifdef DEMO_CHECK_ASYN_UPDATE
extern int g_checkAsyncUpdate;
#endif

#if 0
enum ndPrimitiveType
{
	_NULL_PRIMITIVE,
	_SPHERE_PRIMITIVE,
	_BOX_PRIMITIVE,
	_CAPSULE_PRIMITIVE,
	_CYLINDER_PRIMITIVE,
	_CONE_PRIMITIVE,
	_CHAMFER_CYLINDER_PRIMITIVE,
	_RANDOM_CONVEX_HULL_PRIMITIVE,
	_REGULAR_CONVEX_HULL_PRIMITIVE,
	_COMPOUND_CONVEX_CRUZ_PRIMITIVE,
};


class ndMakeViualMesh : public dScene::dSceneExportCallback
{
	public:
	ndMakeViualMesh(NewtonWorld* const world);
	NewtonMesh* CreateVisualMesh(NewtonBody* const body, char* const name, int maxNameSize) const;
	NewtonWorld* m_world;
};

void ExportScene (NewtonWorld* const world, const char* const fileName);


class ndDemoMesh;
class ndDemoEntity;
class ndDemoEntityManager;

void GetContactOnBody (NewtonBody* const body);
void HandlecollisionPoints (NewtonJoint* const contactjoint);
NewtonJoint* CheckIfBodiesCollide (NewtonBody* const body0, NewtonBody* const body1);

dCustomJoint* FindJoint(const NewtonBody* const body0, const NewtonBody* const body1);
dVector FindFloor (const NewtonWorld* world, const dVector& origin, dFloat32 dist, dVector* const normal = NULL);

void PhysicsBodyDestructor (const NewtonBody* body);
void PhysicsApplyGravityForce (const NewtonBody* body, dFloat32 timestep, int threadIndex);

void SetAutoSleepMode (NewtonWorld* const world, int mode);
void CalculateAABB (const NewtonCollision* const collision, const dMatrix& matrix, dVector& minP, dVector& maxP);

void GenericContactProcess (const NewtonJoint* contactJoint, dFloat32 timestep, int threadIndex);

bool GetLastHit (dVector& posit, dVector& normal);
NewtonCollision* CreateCollisionTree (NewtonWorld* const world, ndDemoEntity* const entity, int materialID, bool optimize);
NewtonCollision* CreateConvexCollision (NewtonWorld* const world, const dMatrix& offsetMatrix, const dVector& size, ndPrimitiveType type, int materialID);

NewtonBody* CreatePLYMesh (ndDemoEntityManager* const scene, const char* const name, bool optimized);
NewtonBody* CreateLevelMeshBody (NewtonWorld* const world, ndDemoEntity* const ent, bool optimize);
NewtonBody* CreateSimpleBody(NewtonWorld* const world, void* const userData, dFloat32 mass, const dMatrix& matrix, NewtonCollision* const collision, int materialId, bool generalInertia = false);
NewtonBody* CreateSimpleSolid (ndDemoEntityManager* const scene, ndDemoMesh* const mesh, dFloat32 mass, const dMatrix& matrix, NewtonCollision* const collision, int materialId, bool generalInertia = false);
void AddPrimitiveArray (ndDemoEntityManager* const scene, dFloat32 mass, const dVector& origin, const dVector& size, int xCount, int zCount, dFloat32 spacing, ndPrimitiveType type, int materialID, const dMatrix& shapeOffsetMatrix, dFloat32 findFloorElevation = 1000.0f, dFloat32 offsetHigh = 5.0f);

NewtonBody* CreateInstancedSolid(ndDemoEntityManager* const scene, ndDemoEntity* const parent, dFloat32 mass, const dMatrix& matrix, NewtonCollision* const collision, int materialId, bool generalInertia = false);

NewtonBody* AddFloorBox(ndDemoEntityManager* const scene, const dVector& origin, const dVector& size);
NewtonBody* CreateLevelMesh (ndDemoEntityManager* const scene, const char* const levelName, bool optimized);
NewtonBody* MousePickBody (NewtonWorld* const nWorld, const dVector& origin, const dVector& end, dFloat32& paramter, dVector& positionOut, dVector& normalOut);

void LoadLumberYardMesh(ndDemoEntityManager* const scene, const dVector& location, int shapeid);
//void SerializationWorld (const char* const name, NewtonWorld* const world);
#endif

#endif