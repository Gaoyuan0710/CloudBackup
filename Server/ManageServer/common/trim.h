/*
 * =====================================================================================
 *
 *       Filename:  trim.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2016年01月21日 19时13分17秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Gaoyuan, sishuiliunian0710@gmail.com
 *        Company:  Class 1204 of Computer Science and Technology
 *
 * =====================================================================================
 */

#include <cctype>
#include <iostream>
#include <algorithm>

using namespace std;

inline string& ltrim(string &str) {
    string::iterator p = find_if(str.begin(), str.end(), not1(ptr_fun<int, int>(isspace)));
    str.erase(str.begin(), p);
    return str;
}

inline string& rtrim(string &str) {
    string::reverse_iterator p = find_if(str.rbegin(), str.rend(), not1(ptr_fun<int , int>(isspace)));
    str.erase(p.base(), str.end());
    return str;
}

inline string& trim(string &str) {
    ltrim(rtrim(str));
    return str;
}

