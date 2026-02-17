#include "CustomPlatControl.hpp"
#include <Geode/utils/Keyboard.hpp>
#include <cstdint>
#include <initializer_list>

using namespace geode::prelude;

std::unordered_map<int, bool> customplatcontrol::g_keyStates;

namespace {
    bool s_initialized = false;

    bool hasModifier(geode::Keybind::Modifiers value, geode::Keybind::Modifiers flag) {
        return (static_cast<uint8_t>(value) & static_cast<uint8_t>(flag)) != 0;
    }

    bool isAnyDown(std::initializer_list<cocos2d::enumKeyCodes> keys) {
        for (auto const key : keys) {
            if (customplatcontrol::isKeyDown(key)) {
                return true;
            }
        }
        return false;
    }

    bool areRequiredModifiersDown(geode::Keybind::Modifiers modifiers) {
        if (hasModifier(modifiers, geode::Keybind::Mods_Control) && !isAnyDown({
            cocos2d::KEY_Control,
            cocos2d::KEY_LeftControl,
            cocos2d::KEY_RightControl,
        })) {
            return false;
        }

        if (hasModifier(modifiers, geode::Keybind::Mods_Shift) && !isAnyDown({
            cocos2d::KEY_Shift,
            cocos2d::KEY_LeftShift,
            cocos2d::KEY_RightShift,
        })) {
            return false;
        }

        if (hasModifier(modifiers, geode::Keybind::Mods_Alt) && !isAnyDown({
            cocos2d::KEY_Alt,
            cocos2d::KEY_LeftMenu,
            cocos2d::KEY_RightMenu,
        })) {
            return false;
        }

        if (hasModifier(modifiers, geode::Keybind::Mods_Super) && !isAnyDown({
            cocos2d::KEY_LeftWindowsKey,
            cocos2d::KEY_RightWindowsKey,
        })) {
            return false;
        }

        return true;
    }
}

void customplatcontrol::initializeInputManager() {
    if (s_initialized) {
        return;
    }
    s_initialized = true;

    KeyboardInputEvent().listen([](KeyboardInputData& data) {
        if (data.key == cocos2d::KEY_Unknown) {
            return;
        }

        g_keyStates[static_cast<int>(data.key)] =
            data.action != KeyboardInputData::Action::Release;
    }).leak();
}

bool customplatcontrol::isKeyDown(cocos2d::enumKeyCodes key) {
    if (auto const it = g_keyStates.find(static_cast<int>(key)); it != g_keyStates.end()) {
        return it->second;
    }
    return false;
}

bool customplatcontrol::isControlPressed(ControlAction action) {
    for (auto const& bind : getControlBinds(action)) {
        if (!isKeyDown(bind.key)) {
            continue;
        }
        if (!areRequiredModifiersDown(bind.modifiers)) {
            continue;
        }
        return true;
    }
    return false;
}
