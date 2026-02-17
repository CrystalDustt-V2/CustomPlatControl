#include "CustomPlatControl.hpp"
#include <Geode/loader/SettingV3.hpp>
#include <array>

using namespace geode::prelude;

namespace {
    using Action = customplatcontrol::ControlAction;

    constexpr size_t kActionCount = static_cast<size_t>(Action::Count);

    constexpr std::array<std::string_view, kActionCount> kSettingKeys = {{
        "Player1_Left",
        "Player1_Jump",
        "Player1_Right",
        "Player2_Left",
        "Player2_Jump",
        "Player2_Right",
    }};

    std::array<std::vector<Keybind>, kActionCount> s_binds;
    bool s_initialized = false;

    size_t toIndex(Action action) {
        return static_cast<size_t>(action);
    }

    Keybind getDefaultBind(Action action) {
        switch (action) {
            case Action::Player1Left:
                return Keybind(cocos2d::KEY_A, Keybind::Mods_None);
            case Action::Player1Jump:
                return Keybind(cocos2d::KEY_W, Keybind::Mods_None);
            case Action::Player1Right:
                return Keybind(cocos2d::KEY_D, Keybind::Mods_None);
            case Action::Player2Left:
                return Keybind(cocos2d::KEY_Left, Keybind::Mods_None);
            case Action::Player2Jump:
                return Keybind(cocos2d::KEY_Up, Keybind::Mods_None);
            case Action::Player2Right:
                return Keybind(cocos2d::KEY_Right, Keybind::Mods_None);
            case Action::Count:
                break;
        }
        return Keybind(cocos2d::KEY_None, Keybind::Mods_None);
    }

    bool isKeyboardBind(Keybind const& bind) {
        auto const key = static_cast<int>(bind.key);
        if (key == static_cast<int>(cocos2d::KEY_None) || key == static_cast<int>(cocos2d::KEY_Unknown)) {
            return false;
        }

        return key < 1000 || key > 2000;
    }

    std::vector<Keybind> sanitizeBinds(Action action, std::vector<Keybind> const& input) {
        std::vector<Keybind> out;
        out.reserve(input.size());

        for (auto const& bind : input) {
            if (isKeyboardBind(bind)) {
                out.push_back(bind);
            }
        }

        if (out.empty()) {
            out.push_back(getDefaultBind(action));
        }

        return out;
    }

    void updateCachedBinds(Action action, std::vector<Keybind> const& binds) {
        s_binds[toIndex(action)] = sanitizeBinds(action, binds);
    }

    void refreshFromSetting(Action action) {
        auto const settingKey = std::string(kSettingKeys[toIndex(action)]);
        auto value = Mod::get()->getSettingValue<std::vector<Keybind>>(settingKey);
        updateCachedBinds(action, value);
    }
}

std::string_view customplatcontrol::getSettingKey(ControlAction action) {
    return kSettingKeys[toIndex(action)];
}

std::span<Keybind const> customplatcontrol::getControlBinds(ControlAction action) {
    return s_binds[toIndex(action)];
}

void customplatcontrol::initializeSettings() {
    if (s_initialized) {
        return;
    }
    s_initialized = true;

    for (size_t i = 0; i < kActionCount; ++i) {
        auto const action = static_cast<Action>(i);
        refreshFromSetting(action);

        listenForSettingChanges<std::vector<Keybind>>(
            std::string(kSettingKeys[i]),
            [action](std::vector<Keybind> const& value) {
                updateCachedBinds(action, value);
            }
        );
    }
}
