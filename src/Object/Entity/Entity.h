//
// Created by saber on 1/14/2026.
//

#ifndef HEW_ENTITY_H
#define HEW_ENTITY_H
#include "../Object.h"


class Entity : public Object
{
public:
    void Start() override;
    void Update() override;
    void Draw() override;

protected:
    float currHp = 0;
    float maxHp = 0;
};


#endif //HEW_ENTITY_H