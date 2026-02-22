#include <Geode/Geode.hpp>
#include <Geode/modify/GJBaseGameLayer.hpp>
#include "CustomPlatControl.hpp"

using namespace geode::prelude;

namespace {
    bool isPlatformerLevel(GJBaseGameLayer* layer) {
        return layer && layer->m_levelSettings && layer->m_levelSettings->m_platformerMode;
    }

    void setButtonState(PlayerObject* player, PlayerButton button, bool isDown) {
        if (!player) {
            return;
        }

        if (isDown) {
            player->pushButton(button);
        }
        else {
            player->releaseButton(button);
        }
    }

    void applyControlState(
        PlayerObject* player,
        customplatcontrol::ControlAction leftAction,
        customplatcontrol::ControlAction jumpAction,
        customplatcontrol::ControlAction rightAction
    ) {
        setButtonState(
            player,
            PlayerButton::Left,
            customplatcontrol::isControlPressed(leftAction)
        );
        setButtonState(
            player,
            PlayerButton::Jump,
            customplatcontrol::isControlPressed(jumpAction)
        );
        setButtonState(
            player,
            PlayerButton::Right,
            customplatcontrol::isControlPressed(rightAction)
        );
    }
}

class $modify(CPCPlatformerHook, GJBaseGameLayer) {
    void processCommands(float dt, bool isHalfTick, bool isLastTick) {
        if (isPlatformerLevel(this)) {
            applyControlState(
                this->m_player1,
                customplatcontrol::ControlAction::Player1Left,
                customplatcontrol::ControlAction::Player1Jump,
                customplatcontrol::ControlAction::Player1Right
            );

            applyControlState(
                this->m_player2,
                customplatcontrol::ControlAction::Player2Left,
                customplatcontrol::ControlAction::Player2Jump,
                customplatcontrol::ControlAction::Player2Right
            );
        }

        GJBaseGameLayer::processCommands(dt, isHalfTick, isLastTick);
    }
};
