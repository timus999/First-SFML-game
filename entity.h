#ifndef ENTITY_H
#define ENTITY_H

#include "Components.h"
#include <string>
#include <memory>


class Entity{


  std::string m_tag = "defualt";
  bool m_active = true;
  size_t m_id = 0;

  Entity( const size_t id, const std::string & tag);

  public:
    //component pointers;
  std::shared_ptr<CTransform>   cTransform;
  std::shared_ptr<CShape>       cShape;
  std::shared_ptr<CCollision>   cCollision;
  std::shared_ptr<CInput>       cInput;
  std::shared_ptr<CScore>       cScore;
  std::shared_ptr<CLifespan>    cLifespan;

  //private member access function
  bool isActive() const;
  const std::string & tag() const;
  const size_t id() const;
  void destroy();
  friend class EntityManager;
};

#endif
