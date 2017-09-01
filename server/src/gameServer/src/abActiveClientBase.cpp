#include "abActiveClientBase.h"

ActiveClientBase::ActiveClientBase(ILogMessage& logger, eType type)
    : _logger(logger), _type(type)
{
    _logger.onModuleMessage(ILogMessage::eSeverity::eInfo, "ActiveClient created: " + typeStr());
}

std::string  ActiveClientBase::typeStr(void)
{
    switch(_type)
    {
        case eType::eUnknown:
            return "Unknown";
    }

    return "???";
}
