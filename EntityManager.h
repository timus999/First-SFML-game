#ifndef ENTITYMANAGER_H
#define ENTITYMANAGER_H
#include "entity.h"
#include <vector>
#include <map>

typedef std::vector<std::shared_ptr<Entity>> EntityVec;
typedef std::map <std::string, EntityVec> EntityMap;

class EntityManager{
  EntityVec m_entities;
  EntityMap m_entityMap;
  size_t    m_totalEntities = 0;
  EntityVec m_toAdd;
  public:
    EntityManager();
    void update();
    std::shared_ptr<Entity> addEntity(const std::string& tag);
    void removeDeadEntities(EntityVec& vec);
    const EntityVec& getEntities();
    const EntityVec& getEntities(const std::string& tag);

};

#endif
