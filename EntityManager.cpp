#include "EntityManager.h"
#include <algorithm>

EntityManager:: EntityManager() {}

std::shared_ptr<Entity> EntityManager:: addEntity(const std::string&tag){
  std::shared_ptr<Entity> e(new Entity(m_totalEntities++, tag));

  m_toAdd.push_back(e);
  return e;
}

void EntityManager:: update(){

  for(const auto &  e : m_toAdd){
    m_entities.push_back(e);
    m_entityMap[e->tag()].push_back(e);
  }
  m_toAdd.clear();

  removeDeadEntities(m_entities);

  for ( auto & [tag, entityVec] : m_entityMap){
    removeDeadEntities(entityVec);
  }
}

void EntityManager:: removeDeadEntities(EntityVec & vec){

  auto newEnd = std::remove_if(vec.begin(), vec.end(), [] (const std::shared_ptr<Entity>& obj){ return !obj->isActive();});

  vec.erase(newEnd, vec.end());

}
const EntityVec& EntityManager:: getEntities(){
  return m_entities;
}

const EntityVec& EntityManager:: getEntities(const std::string& tag){
  return m_entityMap[tag];
}