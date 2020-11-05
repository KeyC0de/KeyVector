#include "check_for_leaks.h"
#include <iostream>

class LeakChecker
{
public:
	LeakChecker()
	{
		std::wcerr << L"Memory leak checker setup" << L'\n';
		setupLeakChecker();
	}
	~LeakChecker()
	{
		if (anyMemoryLeaks())
			std::wcerr << L"Leaking.." << L'\n';
		else
			std::wcerr << L"No leaks. : )\n";
	}
	static inline void setupLeakChecker()
	{
#if _DEBUG
		_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF || _CRTDBG_REPORT_FLAG || _CRTDBG_LEAK_CHECK_DF);
#endif
		return;
	}
};

LeakChecker leakChecker{};