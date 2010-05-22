/* Version 2.3 */

#include "AWSprite.h"

// This is used in bb_80_collision(). In fact, bb_80_collision only
//  does 80% C.D. if we set COLLISIONDIVISOR to 10. If we set this
//  constant to 20, we would have 90% collision detection. If we set 
//  it to 5, we would have 60% collision detection
#define COLLISIONDIVISOR (10)

// Construct a sprite from a bitmap file and a number of Frames
AWSprite::AWSprite(char *bmpName, Uint32 hmf) {
	/* load bitmap to temp surface */
	SDL_Surface* temp = SDL_LoadBMP(bmpName);
	/* convert bitmap to display format */
	wholeSurface = SDL_DisplayFormat(temp);
	SDL_FreeSurface(temp);
	dead = false;
	visible = true;
	howManyFrames = hmf;
	firstFrame = 0;
	lastFrame=howManyFrames-1;
	worldX = 0.0;
	worldY = 0.0;
	screenX = 0.0;
	screenY = 0.0;
	height = wholeSurface->h;
	width = wholeSurface->w / hmf;
	create_frames();
	set_currentFrame(0);

	percentHeight = height / COLLISIONDIVISOR;
	percentWidth = width / COLLISIONDIVISOR;
	velX = 0.0;
	velY = 0.0;
	accelX = 0.0;
	accelY = 0.0;
	Uint32 ticks = SDL_GetTicks();
	lastMoveTicks = ticks;
	lastAnimationTicks = ticks;
	lastAccelerationTicks = ticks;
	movementInterval = 0;
	animationInterval = 0;
	accelerationInterval = 0;
	repeatAnimationForever = false;
	animationRepeats = 0;
	autoDestructActivated = false;
	clear_animation_stack();
	push_animation_stack();
}

AWSprite::~AWSprite(void) {
	SDL_FreeSurface(wholeSurface);
	for(Uint32 f=0; f < howManyFrames; f++) {
		SDL_FreeSurface(frames[f]);
	}
}		

void AWSprite::create_frames() {
	frames = new SDL_Surface*[howManyFrames];
    Uint32 rmask, gmask, bmask, amask;
	SDL_Rect src, dest;

	dest.x = 0;
	dest.y = 0;
	dest.w = width;
	dest.h = height;
	src = dest;      // src.x will change, depending on which frame

    /* SDL interprets each pixel as a 32-bit number, so our masks must depend
       on the endianness (byte order) of the machine */
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    rmask = 0xff000000;
    gmask = 0x00ff0000;
    bmask = 0x0000ff00;
    amask = 0x000000ff;
#else
    rmask = 0x000000ff;
    gmask = 0x0000ff00;
    bmask = 0x00ff0000;
    amask = 0xff000000;
#endif
	SDL_Surface *screen = SDL_GetVideoSurface();
	SDL_Surface * temp;
	for(Uint32 f=0; f < howManyFrames; f++) {
		temp = SDL_CreateRGBSurface(screen->flags, width, height, 
										 screen->format->BitsPerPixel, 
                                         rmask, gmask, bmask, amask);
		frames[f] = SDL_DisplayFormat(temp);
		SDL_FreeSurface(temp);
		src.x = f*width;
		SDL_BlitSurface(wholeSurface, &src, frames[f], &dest);
	}
}



void AWSprite::set_world_position(int wX, int wY) {
	worldX = (float)wX;
	worldY = (float)wY;
}

int AWSprite::get_x() {
	return (int)worldX;
}

int AWSprite::get_y() {
	return (int)worldY;
}

int AWSprite::get_height() {
	return height;
}

int AWSprite::get_width() {
	return width;
}


Uint32 AWSprite::get_currentFrame() {
	return currentFrame;
}

void AWSprite::set_currentFrame(Uint32 frameNumber) {
	currentFrame = frameNumber;
	if(currentFrame >= howManyFrames) {
		currentFrame = howManyFrames-1;
	}
	surface = frames[currentFrame];
}

void AWSprite::set_velocities(float vX, float vY) {
	velX = vX;
	velY = vY;
}

float AWSprite::get_x_velocity() {
	return velX;
}

float AWSprite::get_y_velocity() {
	return velY;
}

void AWSprite::set_accelerations(float aX, float aY) {
	accelX = aX;
	accelY = aY;
}

float AWSprite::get_x_acceleration() {
	return accelX;
}

float AWSprite::get_y_acceleration() {
	return accelY;
}

void AWSprite::accelerate() {
	velX += accelX;
	velY += accelY;
}

void AWSprite::auto_accelerate() {
	if(accelerationInterval == 0) return;
	Uint32 ticks = SDL_GetTicks();
	if((ticks - lastAccelerationTicks) > accelerationInterval) {
		lastAccelerationTicks = ticks;
		accelerate();
	}
}

void AWSprite::set_auto_accelerate(Uint32 interval) {
	accelerationInterval = interval;
}


void AWSprite::make_visible() {
	visible = true;
}

void AWSprite::make_invisible() {
	visible = false;
}

void AWSprite::hide() {
	make_invisible();
}

void AWSprite::set_visibility(bool newVisibility) {
	visible = newVisibility;
}

bool AWSprite::toggle_visibility() {
	if(visible) visible = false;
	else visible = true;

	return visible;
}


void AWSprite::draw_onto(SDL_Surface *onto) {
	SDL_Rect src, dest;

	if(!visible) return;
	src.x = 0;
	src.y = 0;
	src.w = width;
	src.h = height;
	dest.x = (Uint32)screenX;
	dest.y = (Uint32)screenY;
	SDL_BlitSurface(frames[currentFrame], &src, onto, &dest);
}

void AWSprite::draw() {
	SDL_Surface *onto = SDL_GetVideoSurface();
	this->draw_onto(onto);
}

void AWSprite::set_transparent_colour(int R, int G, int B) {
	SDL_Surface *onto = SDL_GetVideoSurface();
	transparentColour = SDL_MapRGB(onto->format, R, G, B);
	SDL_SetColorKey(wholeSurface, (SDL_SRCCOLORKEY | SDL_RLEACCEL), transparentColour);
	for(Uint32 f=0; f < howManyFrames; f++) {
		SDL_SetColorKey(frames[f], (SDL_SRCCOLORKEY | SDL_RLEACCEL), transparentColour);
	}
}

// map (worldX, worldY) to (screenX, screenY)
//  The function needs to know where in the world the top-left 
//  of the screen currently is. Once we have mapped, we can just 
//  use draw().
void AWSprite::map_world_to_screen(int topleftX, int topleftY) {
	screenX = worldX - topleftX;
	screenY = worldY - topleftY;
}

// This version assumes that the world is the same size as the screen
void AWSprite::map_world_to_screen() {
	screenX = worldX;
	screenY = worldY;
}


void AWSprite::move() {
	worldX += velX;
	worldY += velY;
	map_world_to_screen();
}

void AWSprite::auto_move() {
	if(movementInterval == 0) return;

	Uint32 ticks = SDL_GetTicks();

	if((ticks - lastMoveTicks) > movementInterval) {
		lastMoveTicks = ticks;
		move();
	}			
}

void AWSprite::set_auto_move(Uint32 interval) {
	movementInterval = interval;
}


void AWSprite::animate() {
	//set_currentFrame(get_currentFrame()+1);
	currentFrame++;
	if(repeatAnimationForever) {
		// In ths case, we are just animating over and over
		// and we don't care about how many times we repeat
		if(get_currentFrame() > lastFrame) {
			set_currentFrame(firstFrame);
		}
	} else {
		if(get_currentFrame() > lastFrame) {
			animationRepeats = animationRepeats - 1;
			if(animationRepeats == 0) {
				if(!autoDestructActivated) {
					pop_animation_stack();
				} else {
					this->make_invisible();
					this->set_auto_accelerate(0);
					this->set_auto_move(0);
					autoDestructActivated = false;
					clear_animation_stack();
					this->set_auto_animate(0);
					push_animation_stack();
				}
			} else {
				set_currentFrame(firstFrame);
			}
		}
	}
	set_currentFrame(currentFrame);
}


/* Callback section for the auto animation feature.
   It is very complex. You don't need to understand it.
   It is this complex because I wanted to keep currentFrame,
   lastFrame, firstFrame and howmanyFrames private
*/
void AWSprite::auto_animate() {
	if(animationInterval == 0) return;

	Uint32 ticks = SDL_GetTicks();
	if((ticks - lastAnimationTicks) > animationInterval) {
		lastAnimationTicks = ticks;
		animate();
	}
}


// This allows us to set up an animation to repeat a limited
// number of times. If you set the second parameter to zero,
// the animation will repeat FOREVER. If you want to *stop*
// animation, set the FIRST parameter to zero
void AWSprite::set_auto_animate(Uint32 interval, Uint32 repeats) {
	if(repeats == 0) repeatAnimationForever = true;
	else repeatAnimationForever = false;
	animationInterval = interval;
	animationRepeats = repeats;
	// Remember what we were doing before
	push_animation_stack();
}

// Calling this function means we want to repeat forever...
void AWSprite::set_auto_animate(Uint32 interval) {
	set_auto_animate(interval, 0);
}


// This allows you to use a subset of the frames available for an animation
void AWSprite::set_animation_subset(Uint32 first, Uint32 last) {
	if(first < 0) first = 0;
	if(first >= howManyFrames) first = howManyFrames-1;
	if(last < first) last = first;
	if(last >= howManyFrames) last = howManyFrames-1;

	firstFrame = first;
	lastFrame = last;
	set_currentFrame(firstFrame);
}


// Set up the destruction animation of the sprite
// ******** This cannot be countermanded!! ***********
// Once the "countdown" starts, the sprite WILL die...
void AWSprite::set_auto_destruct(Uint32 first, Uint32 last, Uint32 interval, Uint32 repeats) {
	// No need to do it twice!!
	if(autoDestructActivated) return;
	this->make_visible();
	// It makes no sense to have an infinite destruction animation!
	if(repeats < 1) repeats = 1;
	// Once the auto-destruct starts, the stack is irrelevant
	clear_animation_stack();
	set_animation_subset(first, last);
	set_auto_animate(interval, repeats);
	autoDestructActivated = true;
}


void AWSprite::set_auto_destruct(Uint32 first, Uint32 last, Uint32 interval) {
	set_auto_destruct(first, last, interval, 1);
}




// This function moves, animates, maps and draws!!
void AWSprite::update_everything(int topLeftX, int topLeftY) {
	auto_move();
	auto_animate();
	auto_accelerate();
	map_world_to_screen(topLeftX, topLeftY);
	draw();
}

// This function moves, animates, maps and draws!!
void AWSprite::update_everything() {
	update_everything(0, 0);
}


bool AWSprite::bb_collision(AWSprite *spriteB) {
	Uint32 topA, botA, leftA, rightA;
	Uint32 topB, botB, leftB, rightB;

	if(false == is_visible()) return false;
	if(false == spriteB->is_visible()) return false;

	topA = (Uint32)worldY;
	botA = (Uint32)worldY + height;
	leftA = (Uint32)worldX;
	rightA = (Uint32)worldX + width;
	topB   = spriteB->get_y();
	leftB  = spriteB->get_x();
	rightB = spriteB->get_x() + spriteB->get_width();
	botB   = spriteB->get_y() + spriteB->get_height();

	if(botA < topB) return false;
	if(botB < topA) return false;
	if(rightA < leftB) return false;
	if(rightB < leftA) return false;
	
	return true;
}

bool AWSprite::bb_80_collision(AWSprite *spriteB) {
	Uint32 topA, botA, leftA, rightA;
	Uint32 topB, botB, leftB, rightB;

	if(false == is_visible()) return false;
	if(false == spriteB->is_visible()) return false;

	topA = (Uint32)worldY + percentHeight;
	botA = (Uint32)worldY + height - percentHeight;
	leftA = (Uint32)worldX + percentWidth;
	rightA = (Uint32)worldX + width - percentWidth;
	topB   = spriteB->get_y() + spriteB->percentHeight;
	leftB  = spriteB->get_x() + spriteB->percentWidth;
	rightB = spriteB->get_x() + spriteB->get_width() - spriteB->percentWidth;
	botB   = spriteB->get_y() + spriteB->get_height() - spriteB->percentHeight;

	if(botA < topB) return false;
	if(botB < topA) return false;
	if(rightA < leftB) return false;
	if(rightB < leftA) return false;
	
	return true;
}

bool AWSprite::pp_slow_collision(AWSprite *spriteB) {
	Uint32 topA, botA, leftA, rightA;
	Uint32 topB, botB, leftB, rightB;
	Uint32 topO, botO, leftO, rightO;
	Uint32 ax, ay, bx, by;
	Uint32 APx, APy, ASx, ASy;
	Uint32 BPx, BPy, BSx, BSy;
	Uint32 aColour, bColour;
	Uint32 bTC;

	if(false == is_visible()) return false;
	if(false == spriteB->is_visible()) return false;

	topA = (Uint32)worldY;
	botA = (Uint32)worldY + height;
	leftA = (Uint32)worldX;
	rightA = (Uint32)worldX + width;
	topB   = spriteB->get_y();
	leftB  = spriteB->get_x();
	rightB = spriteB->get_x() + spriteB->get_width();
	botB   = spriteB->get_y() + spriteB->get_height();

	if(botA < topB) return false;
	if(botB < topA) return false;
	if(rightA < leftB) return false;
	if(rightB < leftA) return false;
	
	// If we get here, we know that there is an overlap
	// So we work out where the sides of the ovelap are
	if(leftA < leftB) leftO = leftB;
	else leftO = leftA;
	if(rightA > rightB) rightO = rightB;
	else rightO = rightA;
	if(botA > botB) botO = botB;
	else botO = botA;
	if(topA < topB) topO = topB;
	else topO = topA;

	// P is the top-left, S is the bottom-right of the overlap
	APx = leftO-leftA;   APy = topO-topA;
	ASx = rightO-leftA; ASy = botO-topA;
	BPx = leftO-leftB;   BPy = topO-topB;
	BSx = rightO-leftB; BSy = botO-topB;
	bTC = spriteB->transparentColour;
	by = BPy;
	for(ay=APy; ay < ASy; ay++) {
		bx = BPx;
		for(ax=APx; ax < ASx; ax++) {
			aColour = get_pixel_32(ax, ay);
			bColour = spriteB->get_pixel_32(bx, by);
			if(aColour != transparentColour && bColour != bTC)
				return true;
			bx++;
		}
		by++;
	};

	return false;
}


/*
 * Return the pixel value at (x, y)
 * Notes:
 *  1. This has been translated into AWSprite terms.
 *  2. This function only handles 32-bit colour.
 *  3. The surface must be locked before the pixels are read.
 */
Uint32 AWSprite::get_pixel_32(Uint32 x, Uint32 y) {
    /* bpp will be 1 if it's 8-bit colour, 2 if it's 16-bit colour, 
     *  3 if it's 24-bit colour and 4 if it's 32-bit colour. In 
     *  this case, we have said that we are only dealing with 32-bit 
     *  colour, so if it's not 4 we return the transparent colour,
     *  (because we're saying we can't interpret the colour).
     */
    int bpp = surface->format->BytesPerPixel;
    if(bpp != 4) return transparentColour;
	SDL_LockSurface(surface);

    /* pixels is the data structure where the pixels are stored
     *  it is effectively an array of numbers, where each number
     *  represents a pixel. The size of the number (8-bit, 16-bit
     *  24-bit, 32-bit) determines the colour depth of the pixel.
     * pitch is the number of bytes per scanline of the screen; if
     *  our screen is 640 pixels wide, and we have 32-bit colour,
     *  pitch will be 640*4 = 2560.
     * p is a pointer which will point at the pixel at position
     *  (x,y) on the surface
     */
    Uint8 *p;
	p = (Uint8 *)surface->pixels + (y * surface->pitch) + (x * bpp);

    /* This says: Despite the fact that we declared it as a pointer 
     *  to an 8-bit number, p is actually pointing at a 32-bit number 
     *  and we want to return THE NUMBER THAT p IS POINTING AT
     */
	Uint32 colour = *(Uint32 *)p;
	SDL_UnlockSurface(surface);
    return colour;
}




bool AWSprite::pp_collision(AWSprite *spriteB) {
	Uint32 topA, botA, leftA, rightA;
	Uint32 topB, botB, leftB, rightB;
	Uint32 topO, botO, leftO, rightO;
	Uint32 ax, ay;
	Uint32 APx, APy, ASx, ASy;
	Uint32 BPx, BPy, BSx, BSy;
	Uint32 aColour, bColour;
	Uint32 bTC;

	if(false == is_visible()) return false;
	if(false == spriteB->is_visible()) return false;

	topA = (Uint32)worldY;
	botA = (Uint32)worldY + height;
	leftA = (Uint32)worldX;
	rightA = (Uint32)worldX + width;
	topB   = spriteB->get_y();
	leftB  = spriteB->get_x();
	rightB = spriteB->get_x() + spriteB->get_width();
	botB   = spriteB->get_y() + spriteB->get_height();

	if(botA < topB) return false;
	if(botB < topA) return false;
	if(rightA < leftB) return false;
	if(rightB < leftA) return false;
	
	// If we get here, we know that there is an overlap
	// So we work out where the sides of the ovelap are
	if(leftA < leftB) leftO = leftB;
	else leftO = leftA;
	if(rightA > rightB) rightO = rightB;
	else rightO = rightA;
	if(botA > botB) botO = botB;
	else botO = botA;
	if(topA < topB) topO = topB;
	else topO = topA;
	Uint32 widthO = rightO - leftO;

	// P is the top-left, S is the bottom-right of the overlap
	APx = leftO-leftA;   APy = topO-topA;
	ASx = rightO-leftA; ASy = botO-topA;
	BPx = leftO-leftB;   BPy = topO-topB;
	BSx = rightO-leftB; BSy = botO-topB;
	bTC = spriteB->transparentColour;

	bool foundCollision = false;

	SDL_LockSurface(surface);
	SDL_LockSurface(spriteB->surface);
	
	// These two are pointers into the pixel areas for 
	//  spriteA and spriteB, respectively. We set them
	//  up at first then move them onwards by incrementing
	//  this version of the function only works with 32-bit
	//  colour depth, although adapting it would not be hard
	Uint8 *pA, *pB;
    // We only need to do this once, as loading a sprite in AWSprite
	//  forces it to have the same colour depth as the screen surface.
	//  Whatever this colour depth is, it will be the same for both
	//  spriteA and spriteB
	int bpp = surface->format->BytesPerPixel;
	Uint32 widthObytes = widthO*bpp;

	// These are widths in BYTES. They are used inside the loop 
	//  to avoid the need to do the slow multiplications
	Uint32 surfaceWidthA = surface->pitch;
	Uint32 surfaceWidthB = spriteB->surface->pitch;


	pA = (Uint8 *)surface->pixels + (APy * surfaceWidthA) + (APx * bpp);
	pB = (Uint8 *)spriteB->surface->pixels + (BPy * surfaceWidthB) + (BPx * bpp);

	for(ay=APy; ay < ASy; ay++) {
		for(ax=APx; ax < ASx; ax++) {
			aColour = *(Uint32 *)pA;
			bColour = *(Uint32 *)pB;;
			if(aColour != transparentColour && bColour != bTC) {
				foundCollision = true;
				break;
			}
			pA += bpp;
			pB += bpp;
		}
		if(foundCollision) break;
		pA = pA + (surfaceWidthA - widthObytes);
		pB = pB + (surfaceWidthB - widthObytes);
	};
	SDL_UnlockSurface(surface);
	SDL_UnlockSurface(spriteB->surface);

	return foundCollision;
}	

void AWSprite::pop_animation_stack() {
	// We CANNOT countermand an auto-destruct!
	if(autoDestructActivated) return;

	// Trying to pop from an empty stack is a serious error, 
	//  so we do nothing and just return
	stackTop--;
	if(stackTop < 0) return;
	set_currentFrame(stack[stackTop].currentFrame);
	firstFrame = stack[stackTop].firstFrame;
	lastFrame = stack[stackTop].lastFrame;
	repeatAnimationForever = stack[stackTop].repeatAnimationForever;
	animationRepeats = stack[stackTop].animationRepeats;
	animationInterval = stack[stackTop].animationInterval;
}

void AWSprite::push_animation_stack() {
	// We CANNOT countermand an auto-destruct!
	if(autoDestructActivated) return;

	stackTop++;
	if(stackTop == ANIMATIONSTACKDEPTH) stackTop--;
	if(repeatAnimationForever) stackTop = 0;
	stack[stackTop].currentFrame = get_currentFrame();
	stack[stackTop].firstFrame = firstFrame;
	stack[stackTop].lastFrame = lastFrame;
	stack[stackTop].repeatAnimationForever = repeatAnimationForever;
	stack[stackTop].animationRepeats = animationRepeats;
	stack[stackTop].animationInterval = animationInterval;
}

void AWSprite::clear_animation_stack() {
	stackTop = -1;
}


bool AWSprite::is_visible() {
	return visible;
}


bool AWSprite::is_on_screen(Uint32 topLeftX, Uint32 topLeftY) {
	SDL_Surface *videoSurface;

	videoSurface = SDL_GetVideoSurface();
	map_world_to_screen(topLeftX, topLeftY);
	if( visible &&
		screenX >= 0 && screenX < videoSurface->w && 
		screenY >= 0 && screenY < videoSurface->h)
	{
		return true;
	} else {
		return false;
	}
}


bool AWSprite::is_on_screen() {
	return is_on_screen(0, 0);
}

