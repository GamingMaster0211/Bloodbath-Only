#include <Geode/Geode.hpp>

using namespace geode::prelude;

#include <Geode/modify/MenuLayer.hpp>
#include <Geode/modify/LevelInfoLayer.hpp>
#include <Geode/modify/PlayLayer.hpp>

const int BLOODBATH_ID = 10565740;

class $modify(BloodbathMenuLayer, MenuLayer) {
    bool init() {
        if (!MenuLayer::init())
			return false;

        FLAlertLayer::create(
            "Warning",
            "You can ONLY play Bloodbath.\nEvery other level will be redirected.",
            "OK"
        )->show();

        GameLevelManager::sharedState()->downloadLevel(BLOODBATH_ID, false, 0);

        return true;
    }
};

class $modify(BloodbathLevelInfoLayer, LevelInfoLayer) {
    static LevelInfoLayer* create(GJGameLevel* level, bool challenge) {
        if (level && level->m_levelID == BLOODBATH_ID) {
            return LevelInfoLayer::create(level, challenge);
        }

        auto glm = GameLevelManager::sharedState();
        auto bloodbath = glm->getSavedLevel(BLOODBATH_ID);

        if (!bloodbath) {
            glm->downloadLevel(BLOODBATH_ID, false, 0);

            FLAlertLayer::create(
                "Redirecting...",
                "Nice try...\nDownloading Bloodbath...",
                "OK"
            )->show();

            return nullptr;
        }

        FLAlertLayer::create(
            "Redirected",
            "Only Bloodbath is allowed.",
            "OK"
        )->show();

        return LevelInfoLayer::create(bloodbath, false);
    }
};

class $modify(BloodbathPlayLayer, PlayLayer) {
    bool init(GJGameLevel* level, bool useReplay, bool dontCreateObjects) {
        if (!level) return false;

        if (level->m_levelID != BLOODBATH_ID) {
            auto glm = GameLevelManager::sharedState();
            auto bloodbath = glm->getSavedLevel(BLOODBATH_ID);

            if (bloodbath) {
                return PlayLayer::init(bloodbath, useReplay, dontCreateObjects);
            }

            glm->downloadLevel(BLOODBATH_ID, false, 0);

            FLAlertLayer::create(
                "Blocked",
                "You literally cannot escape Bloodbath",
                "OK"
            )->show();

            return false;
        }

        return PlayLayer::init(level, useReplay, dontCreateObjects);
    }
};
