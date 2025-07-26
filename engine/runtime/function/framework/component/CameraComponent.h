#pragma once
#include "Component.h"

class CameraComponent : public Component {
   public:
    CameraComponent(/* args */) = default;
    virtual ~CameraComponent() {};

    void tick(float delta_time) override;

   private:
    /* data */
};
