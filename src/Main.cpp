#include <Geode/modify/MenuLayer.hpp>

using namespace geode::prelude;

class $modify(MenuLayer) {
    bool init() {
        if (!MenuLayer::init())
            return false;
        
        log::info("Hi from my mod!");

        return true;
    }
};
