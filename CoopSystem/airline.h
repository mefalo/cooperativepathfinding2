#include "math.h"

double AStarNode::Airline() {
	int i, correct;
	char c;
	char *s;
	double h=0.0;
	double x, y, corX, corY, dx, dy;

	s=TARGETSTATE;
	
	for(i=0; i < 9; i++) {
		c = tiles[i];
		x = (double)(i%3);
		y = (double)(i/3);
		// find the character in TARGETSTATE
		for(correct = 0; correct < 9; correct++) {
			if(s[correct] == c) {
				corX = (double)(correct%3);
				corY = (double)(correct/3);
				break;
			}
		}
		dx = (x-corX)*(x-corX);
		dy = (y-corY)*(y-corY);

		h = h + sqrt(dx+dy);
	}
	
	return h;
}
