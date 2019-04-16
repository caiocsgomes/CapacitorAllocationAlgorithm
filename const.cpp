#include "stdafx.h"
#include "const.h"

//This could be done with a header and all components as statics inside it, 
//but this way we would have a definition of each component per translation unit,
//what is not good design due to unnecessary memory allocation

namespace capacitor {
	extern const float price300kVAr = 900;
	extern const float price300kVArInstalation = 1000;
}

namespace energy {
	extern const float pricekWh = 0.06;
}

namespace general {
	extern const int heavyLoadHours		= 1000;
	extern const int mediumLoadHours	= 6760;
	extern const int lightLoadHours		= 1000;
	extern const std::complex<float> voltageReference(11.4, 0);
	extern const float lossTolerance = 1;
	extern const int numberOfBars = 12;
}

namespace loadLevel {
	extern const float heavy = 1.5;
	extern const float medium = 1;
	extern const float light = 0.5;
}

namespace analysys {
	extern const int numberOfBussesAnalyzed = 20;
}