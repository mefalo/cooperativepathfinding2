/* Version 2.3 */


// An AWBullet is exactly like an AWSprite except that it keeps track of
//  whether it is "inUse". If it is, it means this bullet (or missile
//  or bomb or arrow or whatever) has been fired and it is still going
//  ie it hasn't yet disappeared from the screen area.

#include "AWSprite.h"
#include "AWBullet.h"


AWBullet::AWBullet(char *bmpName, Uint32 hmf) : AWSprite(bmpName, hmf) {
	inUse = false;
}


void AWBullet::update_everything(Uint32 topLeftX, Uint32 topLeftY) {
	if(!inUse) return;
	if(is_on_screen(topLeftX, topLeftY)) {
		auto_move();
		auto_animate();
		auto_accelerate();
		map_world_to_screen(topLeftX, topLeftY);
		draw();
	} else {
		inUse = false;
		set_auto_move(0);
		set_auto_animate(0);
		set_auto_accelerate(0);
	}
}

void AWBullet::update_everything() {
	update_everything(0, 0);
}

