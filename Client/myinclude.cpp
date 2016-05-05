#include"myinclude.h"

string getRoadFolder(string s)
{
    int  len = s.size() - 1;
    int  i = 0;
    for(i = len;i>=0;i--)
    {
        if(s[i] == '/')
            break;
    }
    return s.substr(i);
}



