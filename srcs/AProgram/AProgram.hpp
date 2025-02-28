#pragma once

class AProgram
{
    public:
    AProgram();
    virtual ~AProgram();

    virtual void Run() = 0;
};