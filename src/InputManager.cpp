#include "CustomPlatControl.hpp"
#include <Geode/utils/Keyboard.hpp>
#include <cstdint>
#include <initializer_list>

using namespace geode::prelude;

std::unordered_map<int, bool> customplatcontrol::g_keyStates;

namespace {
    bool s_initialized = false;

    // [Geode v5.0.0-beta.3 Migration]
    // Keybind modifiers now use `KeyboardModifier` instead of `Keybind::Modifiers`.
    bool hasModifier(geode::KeyboardModifier value, geode::KeyboardModifier flag) {
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

    // [Geode v5.0.0-beta.3 Migration]
    bool areRequiredModifiersDown(geode::KeyboardModifier modifiers) {
        if (hasModifier(modifiers, geode::KeyboardModifier::Control) && !isAnyDown({
            cocos2d::KEY_Control,
            cocos2d::KEY_LeftControl,
            cocos2d::KEY_RightControl,
        })) {
            return false;
        }

        if (hasModifier(modifiers, geode::KeyboardModifier::Shift) && !isAnyDown({
            cocos2d::KEY_Shift,
            cocos2d::KEY_LeftShift,
            cocos2d::KEY_RightShift,
        })) {
            return false;
        }

        if (hasModifier(modifiers, geode::KeyboardModifier::Alt) && !isAnyDown({
            cocos2d::KEY_Alt,
            cocos2d::KEY_LeftMenu,
            cocos2d::KEY_RightMenu,
        })) {
            return false;
        }

        if (hasModifier(modifiers, geode::KeyboardModifier::Super) && !isAnyDown({
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

    // [Geode v5.0.0-beta.3 Migration]
    // Use explicit listener priority + propagation handling for the updated input event system.
    KeyboardInputEvent().listen([](KeyboardInputData& data) {
        if (data.key == cocos2d::KEY_Unknown || data.key == cocos2d::KEY_None) {
            return ListenerResult::Propagate;
        }

        // [Geode v5.0.0-beta.3 Migration]
        // Keep tracking real-time key state (press/repeat/release). Timestamp/modifiers are provided
        // by beta.3 and intentionally unused here because gameplay checks rely on held state only.
        g_keyStates[static_cast<int>(data.key)] =
            data.action != KeyboardInputData::Action::Release;
        (void)data.timestamp;
        (void)data.modifiers;
        return ListenerResult::Propagate;
    }, Priority::VeryEarly).leak();
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
