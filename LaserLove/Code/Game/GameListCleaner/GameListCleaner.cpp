#include "Game/GameListCleaner/GameListCleaner.hpp"
#include "Engine/Core/StringUtils.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"

std::vector<GameListCleaner, TUntrackedAllocator<GameListCleaner>>* GameListCleaner::s_entityCleanerFuncs = nullptr;
std::vector<GameListCleaner, TUntrackedAllocator<GameListCleaner>>* GameListCleaner::s_libraryCleanerFuncs = nullptr;


GameListCleaner::GameListCleaner(const CleanerType& type, const std::string& name, ListCleanerFunc* func)
	: m_cleanerFunc(func),
	m_name(name),
	m_type(type)
{
	if (func == nullptr)
	{
		return;
	}
	if (type == ENTITY_CLEANER && s_entityCleanerFuncs == nullptr)
	{
		s_entityCleanerFuncs = new std::vector<GameListCleaner, TUntrackedAllocator<GameListCleaner>>();
	}
	else if (type == LIBRARY_CLEANER && s_libraryCleanerFuncs == nullptr)
	{
		s_libraryCleanerFuncs = new std::vector<GameListCleaner, TUntrackedAllocator<GameListCleaner>>();
	}

	if (type == ENTITY_CLEANER)
	{
		bool alreadyExists = false;
		for (size_t i = 0; i < s_entityCleanerFuncs->size(); i++)
		{
			if (SimpleStrCmp(s_entityCleanerFuncs->at(i).m_name, name) == true)
			{
				alreadyExists = true;
				break;
			}
		}
		if (alreadyExists == false)
		{
			s_entityCleanerFuncs->push_back(*this);
		}
		else
		{
			GUARANTEE_RECOVERABLE(false, name + " GameListCleaner already exists!");
		}
	}
	else if (type == LIBRARY_CLEANER)
	{
		bool alreadyExists = false;
		for (size_t i = 0; i < s_libraryCleanerFuncs->size(); i++)
		{
			if (SimpleStrCmp(s_libraryCleanerFuncs->at(i).m_name, name) == true)
			{
				alreadyExists = true;
				break;
			}
		}
		if (alreadyExists == false)
		{
			s_libraryCleanerFuncs->push_back(*this);
		}
		else
		{
			GUARANTEE_RECOVERABLE(false, name + " GameListCleaner already exists!");
		}
	}

}
GameListCleaner::~GameListCleaner()
{
}

void GameListCleaner::CleanAllEntities()
{
	if (s_entityCleanerFuncs == nullptr)
	{
		return;
	}
	for (size_t i = 0; i < s_entityCleanerFuncs->size(); i++)
	{
		s_entityCleanerFuncs->at(i).m_cleanerFunc();
	}
}
void GameListCleaner::CleanAllLibraries()
{
	if (s_libraryCleanerFuncs == nullptr)
	{
		return;
	}
	for (size_t i = 0; i < s_libraryCleanerFuncs->size(); i++)
	{
		s_libraryCleanerFuncs->at(i).m_cleanerFunc();
	}
}
void GameListCleaner::ClearCleanerFuncs()
{
	if (s_entityCleanerFuncs != nullptr)
	{
		delete s_entityCleanerFuncs;
		s_entityCleanerFuncs = nullptr;
	}
	if (s_libraryCleanerFuncs != nullptr)
	{
		delete s_libraryCleanerFuncs;
		s_libraryCleanerFuncs = nullptr;
	}
}