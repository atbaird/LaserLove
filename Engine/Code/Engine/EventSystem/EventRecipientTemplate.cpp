#include "Engine/EventSystem/EventRecipientTemplate.hpp"
#include "Engine/EventSystem/EventSystem.hpp"

EventRecipientTemplate::EventRecipientTemplate()
{

}

EventRecipientTemplate::~EventRecipientTemplate()
{
	EventSystem::GlobalUnregisterFromAllEvents(this);
}