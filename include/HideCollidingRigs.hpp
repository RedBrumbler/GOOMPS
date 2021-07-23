#pragma once

#include "custom-types/shared/macros.hpp"
#include "UnityEngine/Collider.hpp"
#include "UnityEngine/MonoBehaviour.hpp"

DECLARE_CLASS_CODEGEN(GOOMPS, HideCollidingRigs, UnityEngine::MonoBehaviour,
    DECLARE_INSTANCE_METHOD(void, Awake);
    DECLARE_INSTANCE_METHOD(void, OnTriggerEnter, UnityEngine::Collider* collider);
    DECLARE_INSTANCE_METHOD(void, OnTriggerExit, UnityEngine::Collider* collider);
    DECLARE_STATIC_METHOD(void, set_radius, float radius);
)