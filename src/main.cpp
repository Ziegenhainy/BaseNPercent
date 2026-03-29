#include "Geode/loader/Mod.hpp"
#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>

using namespace geode::prelude;

const char* BASE_CHARS = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz&$";

std::string turnBaseN(float percent, float n, bool isDecimal) {
    std::string baseNString;

    percent /= 100;
    percent *= n * n;

    if (isDecimal) percent *= n * n;

    baseNString.clear();
    

    if (percent < 1) {
        if (isDecimal) return "0.00%";
        return "0%";
    };
    
    int place = 0;

    while (percent >= 1 || (place < 3 && isDecimal)) {
        baseNString += BASE_CHARS[static_cast<int>(std::fmod(percent, n))];
        percent /= n;
        place += 1;
        if (place == 2 && isDecimal) baseNString += ".";
    };

    std::reverse(baseNString.begin(), baseNString.end());
    
    baseNString += "%";
    
    return baseNString;
}

class $modify(hookedPlayLayer, PlayLayer) {
    struct Fields {
        int m_baseN;
        bool m_isPlatformer;
    };

    bool init(GJGameLevel* level, bool useReplay, bool dontCreateObjects) {
        if (!PlayLayer::init(level, useReplay, dontCreateObjects)) return false;
        
        m_fields->m_baseN = Mod::get()->getSettingValue<int64_t>("base-n");
        m_fields->m_isPlatformer = m_level->isPlatformer();

        return true;
    }

    void updateProgressbar() {
        PlayLayer::updateProgressbar();
        if (m_fields->m_baseN == 10 || m_fields->m_isPlatformer) return;
        m_percentageLabel->setString(turnBaseN(this->getCurrentPercent(),m_fields->m_baseN, m_decimalPercentage).c_str());
    }
};