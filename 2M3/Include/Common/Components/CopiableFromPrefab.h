#pragma once
#include <Prefabs/Prefabs.h>

struct CopiableFromPrefab : public IdentifiableComponent<CopiableFromPrefab>
{
    const Prefab::Type type;

    explicit CopiableFromPrefab(const Prefab::Type prefabType);
};