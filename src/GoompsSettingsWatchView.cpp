#include "GoompsSettingsWatchView.hpp"
#include "config.hpp"
#include "monkecomputer/shared/ViewLib/MonkeWatch.hpp"
#include "monkecomputer/shared/ViewLib/CustomComputer.hpp"
#include "HideCollidingRigs.hpp"

DEFINE_TYPE(GOOMPS, SettingsWatchView);

using namespace GorillaUI;

// 15 possible values
float values[15] = {
    0.10f,
    0.20f,
    0.30f,
    0.40f,
    0.50f,
    0.75f,
    1.00f,
    1.25f,
    1.50f,
    1.75f,
    2.00f,
    2.50f,
    3.00f,
    3.50f,
    4.00f
};

extern Logger& getLogger();

namespace GOOMPS
{
    void SettingsWatchView::Awake()
    {
        numberSelectionHandler = new UISelectionHandler(EKeyboardKey::Left, EKeyboardKey::Right, EKeyboardKey::Enter, true, false);
        numberSelectionHandler->max = 15;

        optionSelectionHandler = new UISelectionHandler(EKeyboardKey::Up, EKeyboardKey::Down, EKeyboardKey::Enter, true, false);
        optionSelectionHandler->max = 2;
    }

    void SettingsWatchView::DidActivate(bool firstActivation)
    {
        std::function<void(int)> fun = std::bind(&SettingsWatchView::OnSelect, this, std::placeholders::_1);
        numberSelectionHandler->selectionCallback = fun;
        optionSelectionHandler->selectionCallback = fun;

        int index = -1;
        if (config.radius < values[0]) index = 0; 
        else if (config.radius > values[14]) index = 14;
        else
        {
            for (int i = 0; i < 14; i++)
            {
                float lower = values[i];
                float upper = values[i + 1];
                

                if (config.radius >= lower && config.radius < upper) 
                {
                    index = i;
                    break;
                }
            }
        }

        if (index == -1)
            index = 6;
        numberSelectionHandler->currentSelectionIndex = index; 

        Redraw();
    }

    void SettingsWatchView::Redraw()
    {
        text = "";

        DrawHeader();
        DrawBody();

        if (watch)
            watch->Redraw();
        else if (computer)
            computer->Redraw();
    }

    void SettingsWatchView::DrawHeader()
    {
        text += "<color=#ffff00>== <color=#fdfdfd>GOOMPS</color> ==</color>\n";
    }

    void SettingsWatchView::DrawBody()
    {
        text += string_format("\n%sGOOMPS is:\n", computer ? "  " : "");
        text += optionSelectionHandler->currentSelectionIndex == 0 ? "<color=#ed6540>></color> " : "  ";
        text += config.enabled ? "<color=#00ff00>enabled</color>" : "<color=#ff0000>disabled</color>";

        text += string_format("\n\n%sConfigure the disappear distance:\n", computer ? "  " : "");
        text += string_format("%sDistance: < %.2f >", (optionSelectionHandler->currentSelectionIndex == 1) ? "<color=#ed6540>></color> " : "  ", values[numberSelectionHandler->currentSelectionIndex]);
    }

    void SettingsWatchView::OnSelect(int number)
    {
        switch (optionSelectionHandler->currentSelectionIndex)
        {
            case 0:
                config.enabled ^= 1;
                break;
            case 1:
                config.radius = values[numberSelectionHandler->currentSelectionIndex];
                HideCollidingRigs::set_radius(config.radius);
                break;
            default:
                break;
        }

        SaveConfig();
    }

    void SettingsWatchView::OnKeyPressed(int key)
    {
        optionSelectionHandler->HandleKey((EKeyboardKey)key);

        if (optionSelectionHandler->currentSelectionIndex == 1)
        {
            numberSelectionHandler->HandleKey((EKeyboardKey)key);
        }
        Redraw();
    }
}