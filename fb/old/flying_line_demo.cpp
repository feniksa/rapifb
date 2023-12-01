#include "flying_line_demo.h"

#include "FrameBuffer.h"

#include <random>
#include <chrono>
#include <thread>

void flying_line_demo()
{
	FrameBuffer frameBuffer("/dev/fb0");
	frameBuffer.clear();

	std::random_device dev;
    	std::mt19937 rng(dev());
    	std::uniform_int_distribution<std::mt19937::result_type> dist(0, 255); 
    	
	std::uniform_int_distribution<std::mt19937::result_type> distw(0, frameBuffer.width() - 1); 
	std::uniform_int_distribution<std::mt19937::result_type> disth(0, frameBuffer.height() - 1); 
	
	Color color(0,255,0,0);
	Color black(0,0,0,0);

	Point p1 { .x = distw(rng), .y = disth(rng) };
	Point p2 { .x = distw(rng), .y = disth(rng) };

	Point old_p1 (p1);
	Point old_p2 (p2);

	int dx1 = 1;
	int dy1 = 1;

	int dx2 = 1;
	int dy2 = 1;

	frameBuffer.enableDirectWrite(true);
	frameBuffer.clear();

	while(true) {
		/*Color color {
			.r = dist(rng),
			.g = dist(rng),
			.b = dist(rng),
			.a = 0,
		};*/
		
		if (p1.x == (frameBuffer.width() - 1) || p1.x == 0) {
			dx1 = -dx1;
		} 
		if (p1.y == (frameBuffer.height() - 1) || p1.y == 0) {
			dy1 = -dy1;
		} 
		p1.x += dx1;
		p1.y += dy1;

		if (p2.x == (frameBuffer.width() - 1) || p2.x == 0) {
			dx2 = -dx2;
		} 
		if (p2.y == (frameBuffer.height() - 1) || p2.y == 0) {
			dy2 = -dy2;
		}

		p2.x += dx2;
		p2.y += dy2;

		frameBuffer.line(old_p1, old_p2, black); // clear
		frameBuffer.line(p1, p2, color);         // draw

		old_p1 = p1;
		old_p2 = p2;

		/*frameBuffer.swap();
		frameBuffer.clear();*/
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	}

}
