#pragma once
#include "math.h"

namespace Daemon 
{
	namespace Model 
	{
		class CurveExp
		{
		public:
			virtual int GetNeededExp(int level) = 0;
            virtual ~CurveExp() {};
		};

		class CurveExpErratic : public CurveExp
		{
		private:
			float p(int x);

		public:
			int GetNeededExp(int level);
		};

        class CurveExpFluctuating : public CurveExp {
        public:
            int GetNeededExp(int level);
        };

        class CurveExpSlow : public CurveExp {
        public:
            int GetNeededExp(int level);
        };

        class CurveExpNormal : public CurveExp {
        public:
            int GetNeededExp(int level);
        };

        class CurveExpParabolic : public CurveExp {
        public:
            int GetNeededExp(int level);
        };

        class CurveExpQuick : public CurveExp {
        public:
            int GetNeededExp(int level);
        };
	}
}

