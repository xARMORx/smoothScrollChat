#ifndef _MAIN_H_
#define _MAIN_H_

#include <Windows.h>
#include <memory>
#include "memwrapper/memwrapper.h"
#include "SAMP.hpp"
#include <chrono>
#include <thread>

using CTimer__UpdatePrototype = void(__cdecl*)();
std::unique_ptr<memwrapper::memhook<CTimer__UpdatePrototype>> CTimer__UpdateHook;

using wndProcPrototype = HRESULT(__stdcall*)(HWND hWnd, UINT message, WPARAM wParam, unsigned int lParam);
std::unique_ptr<memwrapper::memhook<wndProcPrototype>> wndProcHook;

using CChat__ScrollToBottomPrototype = void(__thiscall*)(void*);
std::unique_ptr<memwrapper::memhook<CChat__ScrollToBottomPrototype>> CChat__ScrollToBottomHook;

int CChat__ScrollToBottomAddr{};
int CChat__PageDownAddr{};
int CChat__PageUpAddr{};
int setCursorPositionAddr{};

void* CChat{ nullptr };
void* pScrollbar{ nullptr };

bool enable{};

void setCursorScrollPosition() {
	((void(__thiscall*)(void*)) (SAMP::GetSAMPHandle() + setCursorPositionAddr))(pScrollbar);
}

uint8_t getPageSize() {
	return *(uint8_t*)((uint32_t)CChat);
}

uint8_t getChatScrollBarPos() {
	return *(uint8_t*)((uint32_t)pScrollbar + 0x8E);
}

void scrollChatScrollBar(uint8_t pos) {
	*(uint8_t*)((uint32_t)pScrollbar + 0x8E) = getChatScrollBarPos() + pos;
}

void CChat__PageUp() {
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

void CChat__PageDown() {
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

class AsiPlugin{
public:
	explicit AsiPlugin();
	virtual ~AsiPlugin();
} AsiPlugin;

#endif // !_MAIN_H_