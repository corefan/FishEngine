#include "PhysicsSystem.hpp"
#include "Transform.hpp"
#include "Debug.hpp"
#include <PhysXSDK/Snippets/SnippetCommon/SnippetPrint.h>
#include <PhysXSDK/Snippets/SnippetCommon/SnippetPVD.h>
#include <PhysXSDK/Snippets/SnippetUtils/SnippetUtils.h>

using namespace FishEngine;
using namespace physx;

class FishEnginePhysxErrorCallback : public PxErrorCallback
{
public:
    virtual void reportError(PxErrorCode::Enum code,
                             const char* message,
                             const char* file,
                             int line) override
    {
        // error processing implementation
        Debug::LogWarning("PyhsX Error[file %s at line %d]: %s", file, line, message);
        //abort();
    }
};

//class FishEnginePhysxAllocator : public PxAllocatorCallback
//{
//public:
//    void* allocate(size_t size, const char*, const char*, int)
//    {
//        void* ptr = ::memalign(16, size);
//        PX_ASSERT((reinterpret_cast<size_t>(ptr) & 15)==0);
//        return ptr;
//    }
//    
//    void deallocate(void* ptr)
//    {
//        ::free(ptr);
//    }
//};

PxDefaultAllocator		gAllocator;
//FishEnginePhysxAllocator		gAllocator;
//PxDefaultErrorCallback	gErrorCallback;
FishEnginePhysxErrorCallback       gErrorCallback;

PxFoundation*			gFoundation = NULL;
PxPhysics*				gPhysics	= NULL;

PxDefaultCpuDispatcher*	gDispatcher = NULL;
PxScene*				gScene		= NULL;

PxMaterial*				gMaterial	= NULL;

PxVisualDebuggerConnection*
gConnection	= NULL;


void FishEngine::PhysicsSystem::Init()
{
    gFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, gAllocator, gErrorCallback);
    PxProfileZoneManager* profileZoneManager = &PxProfileZoneManager::createProfileZoneManager(gFoundation);
    PxTolerancesScale scale;
    gPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *gFoundation, scale,true,profileZoneManager);
    
    if(gPhysics->getPvdConnectionManager())
    {
        gPhysics->getVisualDebugger()->setVisualizeConstraints(true);
        gPhysics->getVisualDebugger()->setVisualDebuggerFlag(PxVisualDebuggerFlag::eTRANSMIT_CONTACTS, true);
        gPhysics->getVisualDebugger()->setVisualDebuggerFlag(PxVisualDebuggerFlag::eTRANSMIT_SCENEQUERIES, true);
        gConnection = PxVisualDebuggerExt::createConnection(gPhysics->getPvdConnectionManager(), PVD_HOST, 5425, 10);
    }
    
    PxSceneDesc sceneDesc(gPhysics->getTolerancesScale());
    sceneDesc.gravity = PxVec3(0.0f, -9.81f, 0.0f);
    gDispatcher = PxDefaultCpuDispatcherCreate(2);
    sceneDesc.cpuDispatcher	= gDispatcher;
    sceneDesc.filterShader	= PxDefaultSimulationFilterShader;
    //sceneDesc.flags |= physx::PxSceneFlag::eENABLE_ACTIVETRANSFORMS;
    gScene = gPhysics->createScene(sceneDesc);
    
    gMaterial = gPhysics->createMaterial(0.5f, 0.5f, 0.6f);
}

void FishEngine::PhysicsSystem::FixedUpdate()
{
    gScene->simulate(1.0f/30.f);
    gScene->fetchResults(true);
}

void FishEngine::PhysicsSystem::Clean()
{
    gScene->release();
    gDispatcher->release();
    PxProfileZoneManager* profileZoneManager = gPhysics->getProfileZoneManager();
    if(gConnection != NULL)
        gConnection->release();
    gPhysics->release();
    profileZoneManager->release();
    gFoundation->release();
    
    Debug::Log("Clean up PhysX.");
}
