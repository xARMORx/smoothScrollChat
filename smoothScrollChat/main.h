#ifndef _MAIN_H_
#define _MAIN_H_

#include <Windows.h>
#include <memory>
#include "memwrapper/memwrapper.h"
#include "SAMP.hpp"

using CTimer__UpdatePrototype = void(__cdecl*)();
std::unique_ptr<memwrapper::memhook<CTimer__UpdatePrototype>> CTimer__UpdateHook;

using CChat__PageUpPrototype = void(__thiscall*)(void*);
std::unique_ptr<memwrapper::memhook<CChat__PageUpPrototype>> CChat__PageUpHook;

using CChat__PageDownPrototype = void(__thiscall*)(void*);
std::unique_ptr<memwrapper::memhook<CChat__PageDownPrototype>> CChat__PageDownHook;

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

class AsiPlugin{
public:
	explicit AsiPlugin();
	virtual ~AsiPlugin();
} AsiPlugin;

#endif // !_MAIN_H_