/*
 * =====================================================================================
 *
 *       file_name:  config.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2016年01月21日 18时55分48秒
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  Gaoyuan, sishuiliunian0710@gmail.com
 *        Company:  Class 1204 of Computer Science and Technology
 *
 * =====================================================================================
 */

//Config.h
//Config.h
#pragma once

#include <string>
#include <map>
#include <iostream>
#include <fstream>
#include <sstream>

#include "trim.h"
/*
* \brief Generic configuration Class
*
*/
class Config {
	// Data
protected:
	std::string delimiter_;  //!< separator between key and value
	std::string comment_;    //!< separator between value and comments
	std::map<std::string,std::string> contents_;  //!< extracted keys and values

	typedef std::map<std::string,std::string>::iterator mapi;
	typedef std::map<std::string,std::string>::const_iterator mapci;
	// Methods
public:

	Config( std::string file_name,std::string delimiter = "=",std::string comment = "#" );
	Config();
	template<class T> T ReadConf( const std::string& input_key ) const;  //!<Search for key and ReadConf value or optional default value, call as ReadConf<T>
	template<class T> T ReadConf( const std::string& input_key, const T& input_value ) const;
	template<class T> bool ReadConfInto( T& output_var, const std::string& input_key ) const;
	template<class T>
	bool ReadConfInto( T& output_var, const std::string& input_key, const T& input_value ) const;
	bool FileExist(std::string file_name);
	void ReadConfFile(std::string file_name,std::string delimiter = "=",std::string comment = "#" );

	// Check whether key exists in configuration
	bool KeyExist( const std::string& input_key ) const;

	// Modify keys and values
	template<class T> void AddMember( const std::string& input_key, const T& input_value );
	void RemoveMember( const std::string& input_key );

	// Check or change configuration syntax
	std::string get_delimiter() const { return delimiter_; }
	std::string get_comment() const { return comment_; }
	std::string set_delimiter( const std::string& input_str )
	{ std::string old = delimiter_;  delimiter_ = input_str;  return old; }  
	std::string set_comment( const std::string& input_str )
	{ std::string old = comment_;  comment_ =  input_str;  return old; }

	// Write or ReadConf configuration
	friend std::ostream& operator<<( std::ostream& os, const Config& cf );
	friend std::istream& operator>>( std::istream& is, Config& cf );

protected:
	template<class T> static std::string T_as_string( const T& t );
	template<class T> static T string_as_T( const std::string& s );
	static void Trim( std::string& inout_s );


	// Exception types
public:
	struct FileNotFound {
		std::string file_name;
		FileNotFound( const std::string& file_name_ = std::string() )
			: file_name(file_name_) {} };
		struct Key_not_found {  // thrown only by T ReadConf(key) variant of ReadConf()
			std::string key;
			Key_not_found( const std::string& key_ = std::string() )
				: key(key_) {} };
};


/* static */
template<class T>
std::string Config::T_as_string( const T& t )
{
	// Convert from a T to a string
	// Type T must support << operator
	std::ostringstream ost;
	ost << t;
	return ost.str();
}


/* static */
template<class T>
T Config::string_as_T( const std::string& s )
{
	// Convert from a string to a T
	// Type T must support >> operator
	T t;
	std::istringstream ist(s);
	ist >> t;
	return t;
}


/* static */
template<>
inline std::string Config::string_as_T<std::string>( const std::string& s )
{
	// Convert from a string to a string
	// In other words, do nothing
	return s;
}


/* static */
template<>
inline bool Config::string_as_T<bool>( const std::string& s )
{
	// Convert from a string to a bool
	// Interpret "false", "F", "no", "n", "0" as false
	// Interpret "true", "T", "yes", "y", "1", "-1", or anything else as true
	bool b = true;
	std::string sup = s;
	for( std::string::iterator p = sup.begin(); p != sup.end(); ++p )
		*p = toupper(*p);  // make string all caps
	if( sup==std::string("FALSE") || sup==std::string("F") ||
		sup==std::string("NO") || sup==std::string("N") ||
		sup==std::string("0") || sup==std::string("NONE") )
		b = false;
	return b;
}


template<class T>
T Config::ReadConf( const std::string& key ) const
{
	// ReadConf the value corresponding to key
	mapci p = contents_.find(key);
	if( p == contents_.end() ) throw Key_not_found(key);
	return string_as_T<T>( p->second );
}


template<class T>
T Config::ReadConf( const std::string& key, const T& value ) const
{
	// Return the value corresponding to key or given default value
	// if key is not found
	mapci p = contents_.find(key);
	if( p == contents_.end() ) return value;
	return string_as_T<T>( p->second );
}


template<class T>
bool Config::ReadConfInto( T& var, const std::string& key ) const
{
	// Get the value corresponding to key and store in var
	// Return true if key is found
	// Otherwise leave var untouched
	mapci p = contents_.find(key);
	bool found = ( p != contents_.end() );
	if( found ) var = string_as_T<T>( p->second );
	return found;
}


template<class T>
bool Config::ReadConfInto( T& var, const std::string& key, const T& value ) const
{
	// Get the value corresponding to key and store in var
	// Return true if key is found
	// Otherwise set var to given default
	mapci p = contents_.find(key);
	bool found = ( p != contents_.end() );
	if( found )
		var = string_as_T<T>( p->second );
	else
		var = value;
	return found;
}


template<class T>
void Config::AddMember( const std::string& input_key, const T& value )
{
	// AddMember a key with given value
	std::string v = T_as_string( value );
	std::string key=input_key;
	trim(key);
	trim(v);
	contents_[key] = v;
	return;
}


