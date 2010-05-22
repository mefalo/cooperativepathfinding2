/* Version 2.3 */

#ifndef __AWBULLETINCLUDED__

#define __AWBULLETINCLUDED__  (1)


// An AWBullet is exactly like an AWSprite except that it keeps track of
//  whether it is "inUse". If it is, it means this bullet (or missile
//  or bomb or arrow or whatever) has been fired and it is still going
//  ie it hasn't yet disappeared from the screen area.

#include "AWSprite.h"

class AWBullet : public AWSprite {
public:
	bool inUse; // The bullet has been fired but it hasn't disappeared yet

	AWBullet::AWBullet(char *bmpName, Uint32 hmf);

	void AWBullet::update_everything(Uint32 topLeftX, Uint32 topLeftY);
	
	void AWBullet::update_everything();

};


#endif

