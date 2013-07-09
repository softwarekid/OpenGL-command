#ifdef	TXMATH_DLL_EXPORTS
	#define TXMATH_API __declspec(dllexport)
#else
	#define TXMATH_API __declspec(dllimport)
#endif