/*
 * peeBase.h
 *
 *  Created on: 11 mars 2017
 *      Author: Bertrand
 */

#ifndef SOUNDITEM_PEEBASE_H_
#define SOUNDITEM_PEEBASE_H_

#include <stdint.h>

class peeBase {
public:
	peeBase();
	virtual ~peeBase();

	uint32_t _id;


	void print(void);
};

#endif /* SOUNDITEM_PEEBASE_H_ */
