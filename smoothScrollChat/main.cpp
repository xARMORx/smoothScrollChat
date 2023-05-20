#include "main.h"
#include <chrono>
#include <thread>

void __fastcall CChat__PageUp(void* this_, void* EDX) {
	std::thread([=]() {
		if (!enable) {
			enable = true;
			int repeats = getChatScrollBarPos() - getPageSize();
			if (repeats < 1) repeats = 1;
			while (getChatScrollBarPos() != repeats) {
				scrollChatScrollBar(-1);
				setCursorScrollPosition();
				std::this_thread::sleep_for(std::chrono::milliseconds(20));
			}
			enable = false;
		}
	}).detach();
}

void __fastcall CChat__PageDown(void* this_, void* EDX) {
	std::thread([=]() {
		if (!enable) {
			enable = true;
			int repeats{};
			if (getChatScrollBarPos() != 100 - getPageSize()) {
				if (getChatScrollBarPos() == 1)
					repeats = getPageSize();
				else if (getChatScrollBarPos() >= 100 - (getPageSize() * 2))
					repeats = 100 - getPageSize();
				else
					repeats = getPageSize() + getChatScrollBarPos();
				if (repeats > 100) repeats = 100;
				while (getChatScrollBarPos() != repeats) {
					scrollChatScrollBar(1);
					setCursorScrollPosition();
					std::this_thread::sleep_for(std::chrono::milliseconds(20));
				}
			}
			enable = false;
		}
	}).detach();
}

void __fastcall CChat__ScrollToBottom(void* this_, void* EDX) {
	std::thread([=]() {
		if (!enable) {
			enable = true;
			while (getChatScrollBarPos() != 100 - getPageSize()) {
				scrollChatScrollBar(1);
				setCursorScrollPosition();
				std::this_thread::sleep_for(std::chrono::milliseconds(5));
			}
			enable = false;
		}
	}).detach();
}

void __cdecl CTimer__Update() {
	static bool init;
	if (!init && SAMP::IsSAMPInitialized()) {
		switch (SAMP::GetSAMPVersion())
		{
		case SAMP::sampVersion::R1:
			CChat__ScrollToBottomAddr = { 0x637C0 };
			CChat__PageDownAddr = { 0x63760 };
			CChat__PageUpAddr = { 0x63700 };
			setCursorPositionAddr = { 0x80D00 };
			CChat = *(void**)(SAMP::GetSAMPHandle() + 0x21A0E4);
			break;
		case SAMP::sampVersion::R2:
			CChat__ScrollToBottomAddr = { 0x63890 };
			CChat__PageDownAddr = { 0x63830 };
			CChat__PageUpAddr = { 0x637D0 };
			setCursorPositionAddr = { 0x80DA0 };
			CChat = *(void**)(SAMP::GetSAMPHandle() + 0x21A0EC);
			break;
		case SAMP::sampVersion::R3:
			CChat__ScrollToBottomAddr = { 0x66C10 };
			CChat__PageDownAddr = { 0x66BB0 };
			CChat__PageUpAddr = { 0x66B50 };
			setCursorPositionAddr = { 0x84C10 };
			CChat = *(void**)(SAMP::GetSAMPHandle() + 0x26E8C8);
			break;
		case SAMP::sampVersion::R4:
			CChat__ScrollToBottomAddr = { 0x67350 };
			CChat__PageDownAddr = { 0x672F0 };
			CChat__PageUpAddr = { 0x67290 };
			setCursorPositionAddr = { 0x85350 };
			CChat = *(void**)(SAMP::GetSAMPHandle() + 0x26E9F8);
			break;
		case SAMP::sampVersion::R5:
			CChat__ScrollToBottomAddr = { 0x67390 };
			CChat__PageDownAddr = { 0x67330 };
			CChat__PageUpAddr = { 0x672D0 };
			setCursorPositionAddr = { 0x85320 };
			CChat = *(void**)(SAMP::GetSAMPHandle() + 0x26EB80);
			break;
		case SAMP::sampVersion::DL:
			CChat__ScrollToBottomAddr = { 0x66E00 };
			CChat__PageDownAddr = { 0x66DA0 };
			CChat__PageUpAddr = { 0x66D40 };
			setCursorPositionAddr = { 0x84DA0 };
			CChat = *(void**)(SAMP::GetSAMPHandle() + 0x2ACA10);
			break;
		default:
			break;
		}
		pScrollbar = *(void**)((uint32_t)CChat + 0x11E);
		CChat__PageUpHook = std::make_unique<memwrapper::memhook<CChat__PageUpPrototype>>(SAMP::GetSAMPHandle() + CChat__PageUpAddr, &CChat__PageUp);
		CChat__PageUpHook->install();
		CChat__PageDownHook = std::make_unique<memwrapper::memhook<CChat__PageDownPrototype>>(SAMP::GetSAMPHandle() + CChat__PageDownAddr, &CChat__PageDown);
		CChat__PageDownHook->install();
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
	CChat__PageUpHook->remove();
	CChat__PageDownHook->remove();
	CChat__ScrollToBottomHook->remove();
}