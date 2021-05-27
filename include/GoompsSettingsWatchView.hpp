#pragma once

#include "custom-types/shared/macros.hpp"
#include "monkecomputer/shared/ViewLib/View.hpp"
#include "monkecomputer/shared/InputHandlers/UISelectionHandler.hpp"

DECLARE_CLASS_CODEGEN(GOOMPS, SettingsWatchView, GorillaUI::Components::View, 
    DECLARE_METHOD(void, Awake);
    DECLARE_METHOD(void, DidActivate, bool firstActivation);
    DECLARE_METHOD(void, Redraw);
    DECLARE_METHOD(void, DrawHeader);
    DECLARE_METHOD(void, DrawBody);
    DECLARE_METHOD(void, OnSelect, int key);
    DECLARE_METHOD(void, OnKeyPressed, int key);
    
    public:
        GorillaUI::UISelectionHandler* numberSelectionHandler = nullptr;
        GorillaUI::UISelectionHandler* optionSelectionHandler = nullptr;

    REGISTER_FUNCTION(
        REGISTER_METHOD(Awake);
        REGISTER_METHOD(DidActivate);
        REGISTER_METHOD(Redraw);
        REGISTER_METHOD(DrawHeader);
        REGISTER_METHOD(DrawBody);
        REGISTER_METHOD(OnSelect);
        REGISTER_METHOD(OnKeyPressed);
    )
)