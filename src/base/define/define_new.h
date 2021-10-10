#pragma once

#ifdef _DEBUG
#define NEW new(__FILE__, __LINE__)
#else
#define NEW new
#endif

#define DELETE(p) do { delete p; p = nullptr; } while(false)
#define DELETE_VEC(p) do { delete[] p; p = nullptr; } while(false)
#define DELETE_ONLY(p) do { delete p; } while(false)


