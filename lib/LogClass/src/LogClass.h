/*********************************************************************************************************
**********************************************************************************************************			
								Library: LogClass												**
								Organizacao: Engetech													**
								Autor: Eng. Mateus Domingues Dias										**
								Email: mateusxdias@gmail.com											**
								Release: 10																**
								Data do release 14/02/2019												**
**********************************************************************************************************/
/*********************************************************************************************************/

#ifndef _LogClass_h
#define _LogClass_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

class LogClass
{
protected:
	

public:
	void begin();

	void list_dir(const char *dir);

	void write_file(const char *file, const char *text);

	void delete_file(const char *file);

	void append_file(const char *file, const char *text);

	long size_file(const char *path);

	String read_file(const char *file);

	void delete_buffer();

};
#endif
