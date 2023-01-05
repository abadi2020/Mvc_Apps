#pragma once

#include "Command.hpp"



class CommandTarget
{
public: virtual bool Proc(Command* theCommand) = 0;


};
