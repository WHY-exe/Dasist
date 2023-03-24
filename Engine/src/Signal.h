#pragma once
#define SIGNAL(signal, function)\
	if (signal)\
	{\
		function;\
		signal = false;\
	}
#define SIGNAL_CONDITIONAL(signal, conditional, function)\
	if (signal && conditional)\
	{\
		function;\
		signal = false;\
	}