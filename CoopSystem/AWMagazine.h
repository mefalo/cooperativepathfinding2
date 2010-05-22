/* Version 2.3 */

#ifndef __AWMAGAZINEINCLUDED__
#define __AWMAGAZINEINCLUDED__		(1)

#include <vector>
#include "AWBullet.h"


class AWMagazine {

private:
	Uint32 lastShotTicks;
	Uint32 shotInterval;

public:
	std::vector<AWBullet*> magazine;

	// We construct a magazine with three parameters. The last 
	//  two are standard for constructing AWBullets (and AWSprites 
	//  of course). The first determines the how many of this type
	//  bullet we want our program to handle
	AWMagazine(Uint32 hmb, char *bmpName, Uint32 hmf);

	// This searches through the bullets in the magazine, looking
	// for one which is not currently "inUse". If it finds one, it
	// returns a pointer to it, and you can treat that pointer like
	// a pointer to any AWSprite in your game - for example, you can
	// set its velocity or acceleration etc.
	AWBullet* AWMagazine::allocate_a_bullet();

	// This determines whether it's time to fire yet
	// This allows us to prevent repeat firing too fast
	bool AWMagazine::time_to_auto_fire();
	void AWMagazine::set_auto_fire(Uint32 interval);


	// This returns a reference to a specific bullet, based
	// on the index, i. It is normally used in the main program
	// when you want to loop through all the bullets. Don't 
	// forget to check if the bullet is inUse or not.
	AWBullet* AWMagazine::get(unsigned i);


	/* This tells us how many bullets there are in the magazine.
	 * Note that it's not concerned with whether the bullets are
	 * inUse or not. It's just telling us how many there are in
	 * total.
	 */
	Uint32 AWMagazine::size();

	/*
	 * These functions are provided for convenience. If you use these, 
	 * you don't need a loop within your game loop to update all the
	 * bullets individually.
	 */
	void AWMagazine::update_everything(Uint32 topLeftX, Uint32 topLeftY);

	void AWMagazine::update_everything();

	/*
	 * This function is provided because normally a particular type
	 * of bullet/missile will always have the same transparent colour.
	 */
	void AWMagazine::set_transparent_colour(int R, int G, int B);

};

#endif
