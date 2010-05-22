/* Version 2.3 */

#ifndef __AWSPRITEINCLUDED__

#define __AWSPRITEINCLUDED__  (1)

#include "SDL.h"

typedef struct {
	Uint32 currentFrame;
	Uint32 firstFrame;
	Uint32 lastFrame;
	bool   repeatAnimationForever;
	Uint32 animationRepeats;
	Uint32 animationInterval;
} animationstackentry;

#define ANIMATIONSTACKDEPTH 4


class AWSprite {
private:
	bool   visible;
	int height, width;
	Uint32 firstFrame;
	Uint32 lastFrame;
	bool   repeatAnimationForever;
	Uint32 animationRepeats;
	Uint32 lastMoveTicks;
	Uint32 lastAnimationTicks;
	Uint32 lastAccelerationTicks;
	Uint32 movementInterval;
	Uint32 animationInterval;
	Uint32 accelerationInterval;
	void push_animation_stack();
	void pop_animation_stack();
	void clear_animation_stack();
	int stackTop;
	animationstackentry stack[ANIMATIONSTACKDEPTH];
	Uint32 currentFrame;
	bool autoDestructActivated;

protected:
	Uint32 howManyFrames;

public:
	AWSprite(char *bmpName, Uint32 hmf);
	AWSprite::~AWSprite(void);
	void AWSprite::create_frames();
	bool dead;
	SDL_Surface *wholeSurface;
	SDL_Surface *surface;
	SDL_Surface **frames;
	Uint32 transparentColour;
	float worldX, worldY;
	float velX, velY;
	float accelX, accelY;
	float screenX, screenY;
	Uint32 percentHeight;
	Uint32 percentWidth;
	void set_world_position(int wX, int wY);
	void set_velocities(float vX, float vY);
	void set_accelerations(float aX, float aY);
	void make_visible(); // Note you still have to draw() it!!
	void hide(); // This just calls make_invisible()
	void make_invisible();
	void set_visibility(bool newVisibility);
	bool toggle_visibility(); // This returns the value it SETS visible to
	void move();
	void auto_move(void); // Move the sprite if it is time to do so
	void set_auto_move(Uint32 interval);
	void animate();
	void auto_animate(void); // Animate the sprite if it is time to do so
	void set_auto_animate(Uint32 interval);
	// This version allows you to repeat the animation a limited number of times
	void set_auto_animate(Uint32 interval, Uint32 repeats);
	// Set a subset of the available frames to animate
	void set_animation_subset(Uint32 first, Uint32 last);

	// Set up the destruction animation of the sprite
	// This cannot be countermanded!! Once the "countdown" starts, the sprite
	//  WILL die...
	void set_auto_destruct(Uint32 first, Uint32 last, Uint32 interval, Uint32 repeats);
	void set_auto_destruct(Uint32 first, Uint32 last, Uint32 interval);


	void accelerate();
	void auto_accelerate(); // Accelerate the sprite if it is time to do so
	void set_auto_accelerate(Uint32 interval);
	int get_x();
	int get_y();
	int get_height();
	int AWSprite::get_width();
	Uint32 AWSprite::get_currentFrame();
	void AWSprite::set_currentFrame(Uint32 frameNumber);
	float AWSprite::get_x_velocity();
	float AWSprite::get_y_velocity();
	float AWSprite::get_x_acceleration();
	float AWSprite::get_y_acceleration();
	void AWSprite::draw();
	void AWSprite::draw_onto(SDL_Surface *onto);
	void AWSprite::set_transparent_colour(int R, int G, int B);
	// Set screenX and screenY according to where in the world the top-left pixel of the screen is
	void AWSprite::map_world_to_screen(int topleftX, int topleftY);
	void AWSprite::map_world_to_screen(); // As above, but assume top-left is (0,0) (ie non-scrolling games)
	// Set screenX and screenY according to where in the world the top-left pixel of the screen is
	void AWSprite::update_everything(int topleftX, int topleftY);
	void AWSprite::update_everything();   // As above, but assume top-left is (0,0) (ie non-scrolling games)
	bool AWSprite::bb_collision(AWSprite *spriteB);
	bool AWSprite::bb_80_collision(AWSprite *spriteB);
	bool AWSprite::pp_slow_collision(AWSprite *spriteB);
	bool AWSprite::pp_collision(AWSprite *spriteB);
	Uint32 AWSprite::get_pixel_32(Uint32 x, Uint32 y);
	Uint32 AWSprite::get_pixel_24(Uint32 x, Uint32 y);
	// This simply returns the visible attribute. Notice that it doesn't say whether
	// the sprite is on the screen. All it says is that the sprite would be visible
	// if it was within the boundary of the screen.
	bool AWSprite::is_visible();
	// This simply returns true if the sprite is currently on screen. This function
	//  also takes into account visibility. If the sprite is not visible, it is
	//  considered to be OFF-screen.
	bool AWSprite::is_on_screen();
	bool AWSprite::is_on_screen(Uint32 topLeftX, Uint32 topLeftY);
};

#endif

