#pragma once 

class User
{
public:
    User(QWORD accid) : m_accid(accid) {}

private:
    QWORD m_accid {0};
};
