#pragma once

class Manager
{
protected:
    
public:
    virtual void Initialize() {};
    virtual void Execute() {};
    virtual void Render() {};
    virtual void Finalize() {};

};