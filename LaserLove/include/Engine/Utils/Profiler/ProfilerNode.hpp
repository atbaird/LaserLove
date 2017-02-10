#pragma once

#ifndef PROFILERNODE_HPP
#define PROFILERNODE_HPP
#include <cstdint>

class ProfilerNode
{
private:
	//Variables
	ProfilerNode* m_parent;
	ProfilerNode* m_firstChild;
	ProfilerNode* m_lastChild;
	ProfilerNode* m_next;
	ProfilerNode* m_prev;

	int m_childCount;
	int m_id;
	size_t m_depth;
	const char* m_tag;
	bool m_deleteMyTag;
	uint64_t m_start, m_stop;
public:
	//Constructors
	ProfilerNode(const char* tag = "");
	virtual ~ProfilerNode();

	//Setters
	void SetParent(ProfilerNode* parent);
	void AddChild(ProfilerNode* child);
	void SetNext(ProfilerNode* next);
	void SetPrev(ProfilerNode* prev);
	const bool RemoveChild(ProfilerNode* child);
	void SetTag(const char* tag);
	void SetStart(uint64_t start);
	void SetStop(uint64_t stop);
	void SetDepth(size_t depth);
	void SetID(int id);
	void SetDeleteTag(bool tag);

	//Data Getters
	const char* GetTag() const;
	const uint64_t GetStart() const;
	const uint64_t GetStop() const;
	const int GetChildCount() const;
	const size_t GetDepth() const;
	const int GetID() const;
	const bool GetDeleteTag() const;

	//Getters Constant
	const ProfilerNode* GetParent() const;
	const ProfilerNode* GetFirstChild() const;
	const ProfilerNode* GetLastChild() const;
	const ProfilerNode* GetChildByIdx(int idx) const;
	const ProfilerNode* GetNext() const;
	const ProfilerNode* GetPrev() const;

	//Getters Editable
	ProfilerNode* GetEditableParent() const;
	ProfilerNode* GetEditableFirstChild() const;
	ProfilerNode* GetEditableLastChild() const;
	ProfilerNode* GetEditableChildByIdx(int idx) const;
	ProfilerNode* GetEditableNext() const;
	ProfilerNode* GetEditablePrev() const;

	//static methods
	static void UnhookProfilerNodeFromNeighborsAndParent(ProfilerNode* node);
};

//Macros
#define PROFILER_PRINT_SCOPE(tag) ProfilerNode _S_##__LINE__## ( tag );

#endif