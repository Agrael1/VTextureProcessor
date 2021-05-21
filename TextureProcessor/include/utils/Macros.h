#pragma once
#ifdef _WIN32
#define NOVTABLE __declspec(novtable)
#else
#define NOVTABLE
#endif