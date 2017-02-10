#pragma once

#ifndef GAMELISTCLEANER_HPP
#define GAMELISTCLEANER_HPP
#include <vector>
#include "Engine/Utils/AllocationsSpecial/untrackedallocations.hpp"


typedef void (ListCleanerFunc)();

enum CleanerType
{
	LIBRARY_CLEANER,
	ENTITY_CLEANER
};

class GameListCleaner
{
private:
	CleanerType m_type;
	std::string m_name;
	ListCleanerFunc* m_cleanerFunc;
public:
	GameListCleaner(const CleanerType& type, const std::string& name, ListCleanerFunc* func);
	virtual ~GameListCleaner();

	static void CleanAllEntities();
	static void CleanAllLibraries();
	static void ClearCleanerFuncs();

protected:
	static std::vector<GameListCleaner, TUntrackedAllocator<GameListCleaner>>* s_entityCleanerFuncs;
	static std::vector<GameListCleaner, TUntrackedAllocator<GameListCleaner>>* s_libraryCleanerFuncs;
};
#endif