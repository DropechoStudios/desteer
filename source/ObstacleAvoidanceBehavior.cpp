#include "desteer/behavior/ObstacleAvoidanceBehavior.hpp"

using namespace desteer;
using namespace behavior;
using namespace entity;

using namespace irr;
using namespace core;
using boost::shared_ptr;

ObstacleAvoidanceBehavior::ObstacleAvoidanceBehavior(boost::shared_ptr<EntityGroup> obstacles)
{
    _obstacles = obstacles;
}

void ObstacleAvoidanceBehavior::SetMobile(shared_ptr<IMobileEntity> mob)
{
    _mob = mob;
}

void ObstacleAvoidanceBehavior::SetObstacles(boost::shared_ptr<EntityGroup> obstacles)
{
    _obstacles = obstacles;
}

vector3df ObstacleAvoidanceBehavior::Calculate()
{
    shared_ptr<IBaseEntity> closestHitObstacle = shared_ptr<IBaseEntity>();
    vector3df localPos = vector3df(0,0,0);
    float distToClosest = 16487654;
    float detectLength = 40 + (_mob->Velocity().getLength()/1.5); //_mob->MaxSpeed() / 5

    for(EntityIterator currentObs = _obstacles->begin(); currentObs != _obstacles->end(); ++currentObs)
    {
        localPos = _mob->transformWorldVectToLocal((*currentObs)->Position());

        bool inFront = localPos.Z >= 0;
        bool nearby = localPos.Z < detectLength;

        if(inFront && nearby)
        {
            bool intersecting = (fabs(localPos.X) - ((*currentObs)->Radius() * .75) < _mob->Radius());
            if(intersecting)
            {
                if(localPos.getLength() < distToClosest)
                {
                    shared_ptr<IBaseEntity> currentObsPtr(*currentObs);
                    closestHitObstacle = currentObsPtr;
                    distToClosest = localPos.getLength();
                }
            }
        }
    }

    if(closestHitObstacle.use_count())
    {
        vector3df steeringForce = vector3df(0,0,0);

        float multiplier = 1.0 + ( detectLength - localPos.Z ) / detectLength;
        float distToEdge = ( closestHitObstacle->Radius() - localPos.Z );
        steeringForce.X = distToEdge * multiplier;
        steeringForce.Z = distToEdge * .02;

        return _mob->transformLocalVectToWorld(steeringForce);
    }
    return vector3df(0,0,0);
}
