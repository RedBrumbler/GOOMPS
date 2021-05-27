#pragma once

#include "custom-types/shared/macros.hpp"
#include "UnityEngine/Collider.hpp"
#include "UnityEngine/MonoBehaviour.hpp"

DECLARE_CLASS_CODEGEN(GOOMPS, HideCollidingRigs, UnityEngine::MonoBehaviour,
    DECLARE_METHOD(void, Awake);
    DECLARE_METHOD(void, OnTriggerEnter, UnityEngine::Collider* collider);
    DECLARE_METHOD(void, OnTriggerExit, UnityEngine::Collider* collider);
    DECLARE_METHOD(static void, set_radius, float radius);

    REGISTER_FUNCTION(
        REGISTER_METHOD(Awake);
        REGISTER_METHOD(OnTriggerEnter);
        REGISTER_METHOD(OnTriggerExit);
        REGISTER_METHOD(set_radius);
    )
)