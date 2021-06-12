#include "modloader/shared/modloader.hpp"
#include "beatsaber-hook/shared/utils/logging.hpp"

#include "GorillaLocomotion/Player.hpp"

#include "UnityEngine/SphereCollider.hpp"
#include "UnityEngine/CapsuleCollider.hpp"
#include "UnityEngine/Transform.hpp"
#include "UnityEngine/GameObject.hpp"
#include "GlobalNamespace/TransformFollow.hpp"

#include "HideCollidingRigs.hpp"
#include "GoompsSettingsWatchView.hpp"

#include "custom-types/shared/register.hpp"

#include "monkecomputer/shared/Register.hpp"
#include "monkecomputer/shared/GorillaUI.hpp"
#include "config.hpp"
ModInfo modInfo;

using namespace UnityEngine;
using namespace GOOMPS;

Logger& getLogger()
{
    static Logger* logger = new Logger({ID, VERSION}, LoggerOptions(false, true));
    return *logger;
}

MAKE_HOOK_OFFSETLESS(Player_Awake, void, GorillaLocomotion::Player* self)
{
    Player_Awake(self);

    GameObject* newObj = GameObject::New_ctor();
    SphereCollider* col = newObj->AddComponent<SphereCollider*>();
    GlobalNamespace::TransformFollow* follow = newObj->AddComponent<GlobalNamespace::TransformFollow*>();
    Transform* transform = newObj->get_transform();
    newObj->AddComponent<HideCollidingRigs*>();

    follow->transformToFollow = self->bodyCollider->get_transform();
}

extern "C" void setup(ModInfo& info)
{
    info.id = ID;
    info.version = VERSION;
    modInfo = info;
}

extern "C" void load()
{
    getLogger().info("Loading mod...");
    
    GorillaUI::Init();
    
    if (!LoadConfig())
        SaveConfig();

    getLogger().info("Installing hooks...");
    auto awakefun = il2cpp_utils::FindMethodUnsafe("GorillaLocomotion", "Player", "Awake", 0);
    INSTALL_HOOK_OFFSETLESS(getLogger(), Player_Awake, awakefun);
    getLogger().info("Installed hooks!");
    
    getLogger().info("Registering custom types...");
    custom_types::Register::RegisterType<HideCollidingRigs>();
    custom_types::Register::RegisterType<SettingsWatchView>();
    getLogger().info("Registered custom types!");
    
    GorillaUI::Register::RegisterWatchView<SettingsWatchView*>("GOOMPS", VERSION);
    GorillaUI::Register::RegisterSettingsView<SettingsWatchView*>("GOOMPS", VERSION);
    getLogger().info("Mod Loaded!");
}