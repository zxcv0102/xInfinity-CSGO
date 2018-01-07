#pragma once
#include "HookIncludes.h"

class CNetworkStringTableContainer
{
public:
	INetworkStringTable* FindTable(const char *tableName)
	{
		typedef INetworkStringTable*(__thiscall* oFindTable)(PVOID, const char*);
		return call_vfunc< oFindTable >(this, 3)(this, tableName);
	}
};