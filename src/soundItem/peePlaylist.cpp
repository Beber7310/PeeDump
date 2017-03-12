/*
 * peePlaylist.cpp
 *
 *  Created on: 11 mars 2017
 *      Author: Bertrand
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <iostream>

#include "peePlaylist.h"

using namespace std;

peePlaylist::peePlaylist() {
	// TODO Auto-generated constructor stub
	_name=NULL;
}

peePlaylist::~peePlaylist() {
	// TODO Auto-generated destructor stub
}

peePlaylist::peePlaylist(uint32_t id,const char* name) {
	// TODO Auto-generated constructor stub
	_id=id;
	_name=(char*)malloc(strlen(name)+1);
	strcpy(_name,name);

}

void peePlaylist::print() {

	cout << _name  <<  endl;

}
