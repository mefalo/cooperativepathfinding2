/* Version 2.3 */


// Magazine! It's a magazine full of bullets!! Geddit???!?
// Don't worry too much about the vector stuff. This is an
//  advanced C++ technique. Vectors are generally considered
//  to be safer than arrays. The use of vectors also gives us
//  the chance to *extend* the magazine if we (eg) get a 
//  power-up. This isn't implemented here, not least because 
//  it would be significantly more difficult to *shrink* the
//  magazine once the power-up expired. Nevertheless, using
//  vectors is rather more flexible than using simple arrays.

#include <vector>
#include "AWBullet.h"
#include "AWMagazine.h"

// We construct a magazine with three parameters. The last 
//  two are standard for constructing AWBullets (and AWSprites 
//  of course). The first determines the how many of this type
//  bullet we want our program to handle
AWMagazine::AWMagazine(Uint32 hmb, char *bmpName, Uint32 hmf) {
	if(hmb < 1) hmb = 1;
	for(unsigned int i=0; i < hmb; i++) {
		magazine.push_back(new AWBullet(bmpName, hmf));
	}
	Uint32 ticks = SDL_GetTicks();
	lastShotTicks = ticks;
	shotInterval = 0;

}

// This searches through the bullets in the magazine, looking
// for one which is not currently "inUse". If it finds one, it
// returns a pointer to it, and you can treat that pointer like
// a pointer to any AWSprite in your game - for example, you can
// set its velocity or acceleration etc.
AWBullet* AWMagazine::allocate_a_bullet() {
	unsigned int i;
	if(time_to_auto_fire() == false) return NULL;

	for(i = 0; i < size(); i++) {
		if(magazine[i]->inUse == false) {
			magazine[i]->inUse = true;
			return magazine[i];
		}
	}
	return NULL;
}

// This determines whether it's time to fire yet
// This allows us to prevent repeat firing too fast
bool AWMagazine::time_to_auto_fire() {
	if(shotInterval == 0) return true;
	Uint32 ticks = SDL_GetTicks();
	if((ticks - lastShotTicks) > shotInterval) {
		lastShotTicks = ticks;
		return true;
	}
	return false;
}

void AWMagazine::set_auto_fire(Uint32 interval) {
	shotInterval = interval;
}

// This returns a reference to a specific bullet, based
// on the index, i. It is normally used in the main program
// when you want to loop through all the bullets. Don't 
// forget to check if the bullet is inUse or not.
AWBullet* AWMagazine::get(unsigned i) {
	if(i >= 0 && i < size())
		return (AWBullet *)magazine[i];
	else 
		return NULL;
}


/* This tells us how many bullets there are in the magazine.
 * Note that it's not concerned with whether the bullets are
 * inUse or not. It's just telling us how many there are in
 * total.
 */
Uint32 AWMagazine::size() {
	return magazine.size();
}

/*
 * These functions are provided for convenience. If you use these, 
 * you don't need a loop within your game loop to update all the
 * bullets individually.
 */
void AWMagazine::update_everything(Uint32 topLeftX, Uint32 topLeftY) {
	for(unsigned int i=0; i < size(); i++) {
		magazine[i]->update_everything(topLeftX, topLeftY);
	}
}

void AWMagazine::update_everything() {
	update_everything(0, 0);
}

/*
 * This function is provided because normally a particular type
 * of bullet/missile will always have the same transparent colour.
 */
void AWMagazine::set_transparent_colour(int R, int G, int B) {
	for(unsigned int i=0; i < size(); i++) {
		magazine[i]->set_transparent_colour(R, G, B);
	}
}


