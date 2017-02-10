#pragma once

#ifndef EVENTRECIPIENTTEMPLATE_HPP
#define EVENTRECIPIENTTEMPLATE_HPP

//The only thing this class does is have the GlobalUnregisterFromAllEvents inside of it's destructor.
//Thus this class is only intended as a convenience.
class EventRecipientTemplate
{
private:
protected:
public:
	EventRecipientTemplate();
	virtual ~EventRecipientTemplate();
};
#endif