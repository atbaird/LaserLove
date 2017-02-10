#include "Engine/Locks/ScopeLock.hpp"

//template<LockType>
//ScopeLock::ScopeLock(LockType& t)
//{
//	lock_ptr = &t;
//	lock_ptr->lock();
//}
//template<LockType>
//ScopeLock::~ScopeLock()
//{
//	lock_ptr->unlock();
//}