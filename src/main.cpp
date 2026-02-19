#include "CompilerCompat.hpp"
#include <Geode/Geode.hpp>
#include "CustomPlatControl.hpp"

using namespace geode::prelude;

$execute {
    customplatcontrol::initializeSettings();
    customplatcontrol::initializeInputManager();
}
