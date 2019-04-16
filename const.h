#pragma once
#include <complex>

namespace capacitor {
	extern const float price300kVAr;
	extern const float price300kVArInstalation;
}

namespace energy {
	extern const float pricekWh;
}

namespace general {
	extern const int lightLoadHours;
	extern const int mediumLoadHours;
	extern const int heavyLoadHours;
	extern const std::complex<float> voltageReference;
	extern const float lossTolerance;
	extern const int numberOfBars;
}

namespace loadLevel {
	extern const float light;
	extern const float medium;
	extern const float heavy;
}

namespace analysys {
	extern const int numberOfBussesAnalyzed;
}