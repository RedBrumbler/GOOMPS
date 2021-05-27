#include "HideCollidingRigs.hpp"
#include "UnityEngine/SphereCollider.hpp"
#include "UnityEngine/GameObject.hpp"
#include "UnityEngine/Transform.hpp"
#include "UnityEngine/Renderer.hpp"
#include "UnityEngine/SkinnedMeshRenderer.hpp"

#include "GlobalNamespace/VRRig.hpp"

#include "Photon/Pun/PhotonNetwork.hpp"
#include "Photon/Pun/PhotonView.hpp"
#include "Photon/Realtime/Player.hpp"
#include "config.hpp"
#include "beatsaber-hook/shared/utils/il2cpp-utils.hpp"
DEFINE_TYPE(GOOMPS::HideCollidingRigs);

using namespace UnityEngine;
using namespace Photon::Pun;
using namespace Photon::Realtime;

namespace GOOMPS
{
    static Il2CppString* body = nullptr;
    void HideCollidingRigs::Awake()
    {
        SphereCollider* collider = GetComponent<SphereCollider*>();
        collider->set_isTrigger(true);
        collider->set_radius(config.radius);
        get_gameObject()->set_layer(20);
    }
    void HideCollidingRigs::OnTriggerEnter(Collider* collider)
    {
        if (!config.enabled) return;
        if (!body) body = il2cpp_utils::createcsstr("Body", il2cpp_utils::StringType::Manual);

        if (collider->get_gameObject()->get_name()->Contains(body))
        {
            GlobalNamespace::VRRig* rig = collider->get_transform()->get_parent()->get_parent()->get_parent()->get_gameObject()->GetComponent<GlobalNamespace::VRRig*>();
            if (rig)
            {
                // using reflection to get the field so that a game update is unlikely to break it
                (*il2cpp_utils::GetFieldValue<SkinnedMeshRenderer*>(rig, "mainSkin"))->set_enabled(false);
            }
        }
    }

    void HideCollidingRigs::OnTriggerExit(Collider* collider)
    {
        if (!body) body = il2cpp_utils::createcsstr("Body", il2cpp_utils::StringType::Manual);

        if (collider->get_gameObject()->get_name()->Contains(body))
        {
            GlobalNamespace::VRRig* rig = collider->get_transform()->get_parent()->get_parent()->get_parent()->get_gameObject()->GetComponent<GlobalNamespace::VRRig*>();

            if (rig)
            {
                (*il2cpp_utils::GetFieldValue<SkinnedMeshRenderer*>(rig, "mainSkin"))->set_enabled(true);
            }
        }
    }

    void HideCollidingRigs::set_radius(float value)
    {
        HideCollidingRigs* hider = Object::FindObjectOfType<HideCollidingRigs*>();
        SphereCollider* collider = hider->GetComponent<SphereCollider*>();
        collider->set_radius(config.radius);
    }
}