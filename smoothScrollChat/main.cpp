#include "main.h"

HRESULT __stdcall wndProc(HWND hWnd, UINT message, WPARAM wParam, unsigned int lParam) {
	if (message == WM_KEYDOWN && !*(bool*)(0xB7CB49)) {
		switch (wParam)
		{
		case VK_PRIOR:
			CChat__PageUp();
			break;
		case VK_NEXT:
			CChat__PageDown();
			break;
		default:
			break;
		}
	}

	return wndProcHook->call<HWND, UINT, WPARAM, unsigned int>(hWnd, message, wParam, lParam);
}

void __cdecl CTimer__Update() {
	static bool init;
	if (!init && SAMP::IsSAMPInitialized()) {
		switch (SAMP::GetSAMPVersion())
		{
		case SAMP::sampVersion::R1:
			CChat__ScrollToBottomAddr = { 0x637C0 };
			setCursorPositionAddr = { 0x80D00 };
			CChat__PageUpAddr = { 0x5D908 };
			CChat__PageDownAddr = { 0x5D943 };
			CChat = *(void**)(SAMP::GetSAMPHandle() + 0x21A0E4);
			break;
		case SAMP::sampVersion::R2:
			CChat__ScrollToBottomAddr = { 0x63890 };
			setCursorPositionAddr = { 0x80DA0 };
			CChat__PageUpAddr = { 0x5D9D8 };
			CChat__PageDownAddr = { 0x5DA13 };
			CChat = *(void**)(SAMP::GetSAMPHandle() + 0x21A0EC);
			break;
		case SAMP::sampVersion::R3:
			CChat__ScrollToBottomAddr = { 0x66C10 };
			setCursorPositionAddr = { 0x84C10 };
			CChat__PageUpAddr = { 0x60CA8 };
			CChat__PageDownAddr = { 0x60CE3 };
			CChat = *(void**)(SAMP::GetSAMPHandle() + 0x26E8C8);
			break;
		case SAMP::sampVersion::R4:
			CChat__ScrollToBottomAddr = { 0x67350 };
			setCursorPositionAddr = { 0x85350 };
			CChat__PageUpAddr = { 0x613D8 };
			CChat__PageDownAddr = { 0x61413 };
			CChat = *(void**)(SAMP::GetSAMPHandle() + 0x26E9F8);
			break;
		case SAMP::sampVersion::R5:
			CChat__ScrollToBottomAddr = { 0x67390 };
			setCursorPositionAddr = { 0x85320 };
			CChat__PageUpAddr = { 0x61418 };
			CChat__PageDownAddr = { 0x61453 };
			CChat = *(void**)(SAMP::GetSAMPHandle() + 0x26EB80);
			break;
		case SAMP::sampVersion::DL:
			CChat__ScrollToBottomAddr = { 0x66E00 };
			setCursorPositionAddr = { 0x84DA0 };
			CChat__PageUpAddr = { 0x60E98 };
			CChat__PageDownAddr = { 0x60ED3 };
			CChat = *(void**)(SAMP::GetSAMPHandle() + 0x2ACA10);
			break;
		default:
			break;
		}
		pScrollbar = *(void**)((uint32_t)CChat + 0x11E);
		memwrapper::fill_memory(SAMP::GetSAMPHandle() + CChat__PageUpAddr, 0x90, 11);
		memwrapper::fill_memory(SAMP::GetSAMPHandle() + CChat__PageDownAddr, 0x90, 11);

		wndProcHook = std::make_unique<memwrapper::memhook<wndProcPrototype>>(0x00747EB0, &wndProc);
		wndProcHook->install();
		CChat__ScrollToBottomHook = std::make_unique<memwrapper::memhook<CChat__ScrollToBottomPrototype>>(SAMP::GetSAMPHandle() + CChat__ScrollToBottomAddr, &CChat__ScrollToBottom);
		CChat__ScrollToBottomHook->install();
	}
	CTimer__UpdateHook->call();
}

AsiPlugin::AsiPlugin() {
	CTimer__UpdateHook = std::make_unique<memwrapper::memhook<CTimer__UpdatePrototype>>(0x561B10, &CTimer__Update);
	CTimer__UpdateHook->install();
}

AsiPlugin::~AsiPlugin() {
	CTimer__UpdateHook->remove();
	CChat__ScrollToBottomHook->remove();
	wndProcHook->remove();
}