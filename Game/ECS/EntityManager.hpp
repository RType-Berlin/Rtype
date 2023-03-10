#ifndef ECS_ENTITYMANAGER_HPP_
#define ECS_ENTITYMANAGER_HPP_

#include <vector>

#include "./ComponentPool.hpp"
#include "./DataTypesECS.hpp"
#include "./ECS.hpp"

class EntityManager {
 public:
  struct EntityDesc {
    EntityID id;
    ComponentMask mask;
  };

  EntityManager() {}
  ~EntityManager() {}

  EntityID createNewEntity(EntityID t_id = 0) {
    if (t_id != 0) {
      if (getEntityVersion(t_id) != 0) {
        if (!m_free_entities.empty()) {
          m_free_entities.back();
          m_free_entities.pop_back();
        }
        m_entities[getEntityIndex(t_id)].id = t_id;
        return m_entities[getEntityIndex(t_id)].id;
      }
      m_entities.push_back({t_id, ComponentMask()});
      return m_entities.back().id;
    }
    if (!m_free_entities.empty()) {
      EntityIndex newIndex = m_free_entities.back();
      m_free_entities.pop_back();
      EntityID newID =
        createEntityId(newIndex, getEntityVersion(m_entities[newIndex].id));
      m_entities[newIndex].id = newID;
      return m_entities[newIndex].id;
    }
    m_entities.push_back(
      {createEntityId(EntityIndex(m_entities.size()), 0), ComponentMask()});
    return m_entities.back().id;
  }
  void destroyEntity(EntityID t_id) {
    EntityID newID =
      createEntityId(EntityIndex(-1), getEntityVersion(t_id) + 1);
    EntityIndex index = getEntityIndex(t_id);
    m_entities[index].id = newID;
    m_entities[index].mask.reset();
    m_free_entities.push_back(index);
  }

  void resetManager() {
    m_entities.clear();
    m_component_pools.clear();
    m_free_entities.clear();
  }

  template<typename T>
  T *Assign(EntityID t_id, T t_value) {
    int componentId = getId<T>();

    if (m_component_pools.size() <= componentId)  // Not enough component pool
      m_component_pools.resize(componentId + 1);
    if (m_component_pools[componentId] ==
        nullptr)  // New component, make a new pool
      m_component_pools[componentId] = new ComponentPool(sizeof(T));

    // Looks up the component in the pool, and initializes it with placement new
    T *pComponent =
      new (m_component_pools[componentId]->get(getEntityIndex(t_id)))
        T(t_value);

    // Set the bit for this component to true and return the created component
    m_entities[getEntityIndex(t_id)].mask.set(componentId);
    return pComponent;
  }

  template<typename T>
  T *Get(EntityID t_id) {
    int componentId = getId<T>();
    if (!m_entities[getEntityIndex(t_id)].mask.test(componentId))
      return nullptr;

    T *pComponent = static_cast<T *>(
      m_component_pools[componentId]->get(getEntityIndex(t_id)));
    return pComponent;
  }
  template<typename T>
  void remove(EntityID t_id) {
    // ensures you're not accessing an entity that has been deleted
    if (m_entities[getEntityIndex(t_id)].id != t_id) return;

    int componentId = getId<T>();
    m_entities[getEntityIndex(t_id)].mask.reset(componentId);
  }

  std::vector<EntityDesc> getEntities() { return m_entities; }
  std::vector<ComponentPool *> getComponentPools() { return m_component_pools; }

 private:
  std::vector<EntityDesc> m_entities = {};
  std::vector<ComponentPool *> m_component_pools = {};
  std::vector<EntityIndex> m_free_entities = {};
};

#endif  // ECS_ENTITYMANAGER_HPP_
