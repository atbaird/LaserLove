#include "Engine/Utils/Profiler/ProfilerNode.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"


//---------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------
//Constructors
ProfilerNode::ProfilerNode(const char* tag)
	: m_parent(nullptr),
	m_firstChild(nullptr),
	m_lastChild(nullptr),
	m_prev(nullptr),
	m_next(nullptr),
	m_childCount(0),
	m_depth(0),
	m_id(0),
	m_tag(tag),
	m_deleteMyTag(false)
{

}

ProfilerNode::~ProfilerNode()
{
	ProfilerNode* current = m_firstChild;
	while (current != m_lastChild)
	{
		if (current == nullptr)
		{
			return;
		}
		ProfilerNode* temp = current->GetEditableNext();
		delete current;
		current = nullptr;
		current = temp;
	}
	if (current != nullptr)
	{
		delete current;
	}
	m_firstChild = nullptr;
	m_lastChild = nullptr;
}

//---------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------
//Setters
void ProfilerNode::SetParent(ProfilerNode* parent)
{
	m_parent = nullptr;
	m_parent = parent;
}

void ProfilerNode::AddChild(ProfilerNode* child)
{
	if (child == nullptr)
	{
		return;
	}
	m_childCount++;
	child->SetDepth(m_depth + 1);
	if (m_firstChild == nullptr && m_lastChild == nullptr)
	{
		m_firstChild = child;
		m_lastChild = child;
	}
	else
	{
		m_lastChild->SetNext(child);
		child->SetPrev(m_lastChild);
		m_lastChild = nullptr;
		m_lastChild = child;
	}
}

void ProfilerNode::SetNext(ProfilerNode* next)
{
	m_next = nullptr;
	m_next = next;
}

void ProfilerNode::SetPrev(ProfilerNode* prev)
{
	m_prev = nullptr;
	m_prev = prev;
}

const bool ProfilerNode::RemoveChild(ProfilerNode* child)
{
	m_childCount--;
	bool found = false;
	ProfilerNode* node = m_firstChild;
	while (node != m_lastChild)
	{
		if (node == nullptr)
		{
			break;
		}
		if (node == child)
		{
			found = true;
			break;
		}
		ProfilerNode* temp = node->GetEditableNext();
		node = nullptr;
		node = temp;
	}
	if (found == false && node == child)
	{
		found = true;
	}

	if (node != nullptr && found == true)
	{
		ProfilerNode* prev = node->GetEditablePrev();
		ProfilerNode* next = node->GetEditableNext();
		if (prev != nullptr)
		{
			prev->SetNext(next);
		}
		if (next != nullptr)
		{
			next->SetPrev(prev);
		}

		if (node == m_firstChild && node == m_lastChild)
		{
			m_firstChild = nullptr;
			m_lastChild = nullptr;
		}
		else if (node == m_lastChild)
		{
			m_lastChild = nullptr;
			m_lastChild = prev;
		}
		else if (node == m_firstChild)
		{
			m_firstChild = nullptr;
			m_firstChild = next;
		}
	}
	return found;
}

void ProfilerNode::SetTag(const char* tag)
{
	m_tag = nullptr;
	m_tag = tag;
}

void ProfilerNode::SetStart(uint64_t start)
{
	m_start = start;
}

void ProfilerNode::SetStop(uint64_t stop)
{
	m_stop = stop;
}

void ProfilerNode::SetDepth(size_t depth)
{
	m_depth = depth;
}

void ProfilerNode::SetID(int id)
{
	m_id = id;
}

void ProfilerNode::SetDeleteTag(bool tag)
{
	m_deleteMyTag = tag;
}

//---------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------
//Data Getters
const char* ProfilerNode::GetTag() const
{
	return m_tag;
}

const uint64_t ProfilerNode::GetStart() const
{
	return m_start;
}

const uint64_t ProfilerNode::GetStop() const
{
	return m_stop;
}

const int ProfilerNode::GetChildCount() const
{
	return m_childCount;
}

const size_t ProfilerNode::GetDepth() const
{
	return m_depth;
}

const int ProfilerNode::GetID() const
{
	return m_id;
}


const bool ProfilerNode::GetDeleteTag() const
{
	return m_deleteMyTag;
}

//---------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------
//Getters Constant
const ProfilerNode* ProfilerNode::GetParent() const
{
	return m_parent;
}

const ProfilerNode* ProfilerNode::GetFirstChild() const
{
	return m_firstChild;
}

const ProfilerNode* ProfilerNode::GetLastChild() const
{
	return m_lastChild;
}

const ProfilerNode* ProfilerNode::GetChildByIdx(int idx) const
{
	const ProfilerNode* node = m_firstChild;
	for (int i = 0; i < idx; i++)
	{
		if (node == nullptr)
		{
			return nullptr;
		}
		const ProfilerNode* temp = node->GetNext();
		node = nullptr;
		node = temp;
	}
	return node;
}

const ProfilerNode* ProfilerNode::GetNext() const
{
	return m_next;
}

const ProfilerNode* ProfilerNode::GetPrev() const
{
	return m_prev;
}

//---------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------
//Getters Editable
ProfilerNode* ProfilerNode::GetEditableParent() const
{
	return m_parent;
}

ProfilerNode* ProfilerNode::GetEditableFirstChild() const
{
	return m_firstChild;
}

ProfilerNode* ProfilerNode::GetEditableLastChild() const
{
	return m_lastChild;
}

ProfilerNode* ProfilerNode::GetEditableChildByIdx(int idx) const
{
	ProfilerNode* node = m_firstChild;
	for (int i = 0; i < idx; i++)
	{
		if (node == nullptr)
		{
			return nullptr;
		}
		ProfilerNode* temp = node->GetEditableNext();
		node = nullptr;
		node = temp;
	}
	return node;
}
ProfilerNode* ProfilerNode::GetEditableNext() const
{
	return m_next;
}

ProfilerNode* ProfilerNode::GetEditablePrev() const
{
	return m_prev;
}

//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//static methods
void ProfilerNode::UnhookProfilerNodeFromNeighborsAndParent(ProfilerNode* node)
{
	ProfilerNode* parent = node->GetEditableParent();
	ProfilerNode* next = node->GetEditableNext();
	ProfilerNode* prev = node->GetEditablePrev();

	if (parent != nullptr)
	{
		bool found = parent->RemoveChild(node);
		GUARANTEE_OR_DIE(found, "childNode not removed!");
	}
	if (next != nullptr)
	{
		next->SetPrev(prev);
	}
	if (prev != nullptr)
	{
		prev->SetNext(next);
	}
	node->SetNext(nullptr);
	node->SetPrev(nullptr);
	node->SetParent(nullptr);
}