#include "loadResources.h"

std::wstring LoadStringRrdStlW(UINT id) {
	LPWSTR buffer = NULL;
	int length = LoadStringW(
		GetModuleHandle(NULL),
		id,
		reinterpret_cast<LPWSTR>(&buffer),
		0
	);
	if (length <= 0) {length = 0;}
	return std::wstring(buffer, length);
}

std::string LoadStringRrdStlA(UINT id) {
	const int bufferLength(64);
	CHAR buffer[bufferLength];
	int resultLength = LoadStringA(
		GetModuleHandle(NULL),
		id,
		buffer,
		bufferLength
	);
	if (resultLength <= 0) {resultLength = 0;}
	return std::string(buffer, resultLength);
}

LPCTSTR LoadStringRrdLpcT(UINT id) {
	const int bufferLength(64);
	TCHAR buffer[bufferLength];
	int resultLength = LoadString(
		GetModuleHandle(NULL),
		id,
		buffer,
		bufferLength
	);
	if (resultLength <= 0) {
		buffer[0] = TEXT('\0');
	}
	return buffer;
}
