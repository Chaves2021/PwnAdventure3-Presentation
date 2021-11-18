#include <iostream>
#include <dlfcn.h>
#include "classes.h"

void World::Tick(float delta)
{
    ClientWorld* world = *((ClientWorld**)(dlsym(RTLD_NEXT, "GameWorld")));
    for(ActorRef<IPlayer> p : world->m_players)
    {
        Player *player = (Player*)p.Get();
        Vector3 v = player->GetPosition();
        std::cout << v.x << ' ' << v.y << ' ' << v.z << std::endl;
        //player->SetPosition(Vector3(0, 0, 0));
    }
}

bool Player::CanJump()
{
    //std::cout << "CanJump" << std::endl;
    return true;
}