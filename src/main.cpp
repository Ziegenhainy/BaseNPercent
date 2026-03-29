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
    void updateProgressbar() {
        PlayLayer::updateProgressbar();

        int baseN = Mod::get()->getSettingValue<int64_t>("base-n");
        if (baseN == 10) return;
        m_percentageLabel->setString(turnBaseN(this->getCurrentPercent(), baseN, m_decimalPercentage).c_str());
    }
};