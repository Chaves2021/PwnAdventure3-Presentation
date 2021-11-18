#include <iostream>
#include <sstream>
#include <dlfcn.h>
#include "classes.h"

void Player::Chat(const char *msg)
{
    std::stringstream ss(msg);
    std::string cmd;

    ss >> cmd;
    if(cmd[0] == 't' && cmd[1] == 'p')
    {
        float x, y, z;
        ss >> x >> y >> z;
        
        Vector3 newPos = Vector3(x, y, z);
        if(cmd[2] == 'r')
        {
            this->SetPosition(this->GetPosition() + newPos);
        }
        if(cmd[2] == 'a')
        {
            this->SetPosition(newPos);
        }
    }
}

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
    std::cout << this->GetPlayerName() << std::endl;
    return true;
}