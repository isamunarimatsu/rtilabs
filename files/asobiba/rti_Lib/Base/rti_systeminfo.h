#ifndef ___SYSTEMINFO_H
#define ___SYSTEMINFO_H

class TSystemInfo
{
    bool MMX;   //MMX を使えるかどうか.
public:
    TSystemInfo();
    bool GetMMMX(){return MMX;};
};


extern TSystemInfo SystemInfo;

#endif
