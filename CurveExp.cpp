#include "CurveExp.h"
namespace Daemon
{
    namespace Model
    {
        float CurveExpErratic::p(int x)
        {
            switch (x)
            {
            case 0:
                return 0;
            case 2:
                return 0.014;
            default:
                return 0.008;
            }
        }

        int CurveExpErratic::GetNeededExp(int level)
        {
            if (0 < level && level <= 50) {
                return round(pow(level, 3) * ((100 - level) / 50));
            }
            else if (51 <= level && level <= 68) {
                return round(pow(level, 3) * ((150 - level) / 50));
            }
            else if (69 <= level && level <= 98) {
                return round(1.274f - ((1 / 50) * (level / 3)) - p(level % 3));
            }
            else if (level >= 99) {
                return round(pow(level, 3) * ((160 - level) / 100));
            }
            else {
                return 0;
            }
        }
        int CurveExpFluctuating::GetNeededExp(int level)
        {
            if (0 < level && level <= 15) {
                return round(pow(level, 3) * ((24 + ((level + 1) / 3) / 50)));
            }
            else if (16 <= level && level <= 35) {
                return round(pow(level, 3) * ((14 + level) / 50));
            }
            else if (level >= 36) {
                return round(pow(level, 3) * ((32 + (level / 2)) / 50));
            }
            else {
                return 0;
            }
        }
        int CurveExpSlow::GetNeededExp(int level)
        {
            return round(1.25f * pow(level, 3));
        }
        int CurveExpNormal::GetNeededExp(int level)
        {
            return round(pow(level, 3));
        }
        int CurveExpParabolic::GetNeededExp(int level)
        {
            return round(1.2f * pow(level, 3) - 15 * pow(level, 2) + (100 * level) - 140);
        }
        int CurveExpQuick::GetNeededExp(int level)
        {
            return round(0.8f * pow(level, 3));
        }
    }
}
