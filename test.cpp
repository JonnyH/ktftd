/*
 * =====================================================================================
 *
 *       Filename:  test.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  25/09/12 09:50:24
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Jonathan Hamilton (jh), jtrhamilton@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */
#include <iostream>

#include "datafiles.h"

int main(int argc, char **argv)
{
	std::cout << sizeof(ktftd::datafiles::soldier_dat)<<std::endl;
	std::cout << "0x" << std::hex << offsetof(ktftd::datafiles::soldier_dat, soldier[0].MCSkill) <<std::endl;
}
