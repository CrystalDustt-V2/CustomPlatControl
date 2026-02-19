#pragma once

#include "CompilerCompat.hpp"
#include <Geode/Geode.hpp>
#include <span>
#include <string_view>
#include <unordered_map>
#include <vector>

namespace customplatcontrol {
    enum class ControlAction : size_t {
        Player1Left,
        Player1Jump,
        Player1Right,
        Player2Left,
        Player2Jump,
        Player2Right,
        Count
    };

    extern std::unordered_map<int, bool> g_keyStates;

    void initializeInputManager();
    bool isKeyDown(cocos2d::enumKeyCodes key);
    bool isControlPressed(ControlAction action);

    void initializeSettings();
    std::span<geode::Keybind const> getControlBinds(ControlAction action);
    std::string_view getSettingKey(ControlAction action);
}
