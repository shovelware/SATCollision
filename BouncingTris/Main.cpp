////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include "stdafx.h"
#ifdef _DEBUG
#pragma comment(lib,"sfml-graphics-d.lib")
#pragma comment(lib,"sfml-audio-d.lib")
#pragma comment(lib,"sfml-system-d.lib")
#pragma comment(lib,"sfml-window-d.lib")
#pragma comment(lib,"sfml-network-d.lib")
#else
#pragma comment(lib,"sfml-graphics.lib")
#pragma comment(lib,"sfml-audio.lib")
#pragma comment(lib,"sfml-system.lib")
#pragma comment(lib,"sfml-window.lib")
#pragma comment(lib,"sfml-network.lib")
#endif
#pragma comment(lib,"opengl32.lib")
#pragma comment(lib,"glu32.lib")

#include <iostream>
#include "BouncingShapes.h"
#include "CollisionDetection.h"
 
 
////////////////////////////////////////////////////////////
///Entrypoint of application
////////////////////////////////////////////////////////////
 
bool coinFlip()
{
 if(rand() % 2 == 1)
	 return true;

 else return false;
}

float randomFloat(int max, int divide, bool neg)
{
	if (neg)
		if (coinFlip())
			return ((static_cast<float>(rand() % max) / divide) * -1);

	return (static_cast<float>(rand() % max) / divide);
}

Color randomColor()
{
	return Color(rand() % 255, rand() % 255, rand() % 255, 255);

}

int main()
{
//vars
	int x = 800;
	int y = 600;
	int framerate = 60;

	const enum modes{BALLS, TRIS, BOXES, ALL};
	int mode;

	const int balls = 0;
	const int triangles = 10;
	const int boxes = 10;
	
	BouncingObject* shapes[balls + triangles + boxes];
	CollisionDetection col;

	//////
	sf::ConvexShape box(4);
	box.setPointCount(4);
	box.setPosition(sf::Vector2f(x - 10, y - 10));
	box.setPoint(0, sf::Vector2f(-10, -10));
	box.setPoint(1, sf::Vector2f(10, -10));
	box.setPoint(2, sf::Vector2f(10, 10));
	box.setPoint(3, sf::Vector2f(-10, 10));
	box.setFillColor(sf::Color(160, 20, 40, 128));
	//////

// Create the main window
sf::RenderWindow window(sf::VideoMode(x, y, 32), "BOUNCING SHAPES");
window.setFramerateLimit(framerate);
srand(time(NULL));

mode = modes::ALL;

int total = 0;

if (mode == modes::BALLS || mode == modes::ALL)
{
	int j;
	for (j = total; j < total + balls; j++)
	{
		shapes[j] = new BouncingBall(16, Vector2f(randomFloat(x - 16, 1, false), randomFloat(y - 16, 1, false)), randomColor());
		shapes[j]->impulse(Vector2f(randomFloat(8, 1, true), randomFloat(8, 1, true)));
		shapes[j]->setRotate(false);
	}
	total += j;
}

if (mode == modes::TRIS || mode == modes::ALL)
{	
	int j = total;
	for (j; j < total + triangles; j++)
	{

		//shapes[j] = new BouncingTriangle(32, Vector2f(0,0), randomColor());
		//shapes[j] = new BouncingTriangle(32, Vector2f(x / 4 , y / 4), randomColor());

		shapes[j] = new BouncingTriangle(16, Vector2f(randomFloat(x - 16, 1, false), randomFloat(y - 16, 1, false)), randomColor());
		shapes[j]->impulse(Vector2f(randomFloat(80, 1, true), randomFloat(80, 1, true)));
		shapes[j]->spin(randomFloat(8, 1, true));
	}
	total += j - total;
}

if (mode == modes::BOXES || mode == modes::ALL)
{
	int j = total;
	for (j; j < total + boxes; j++)
	{
		shapes[j] = new BouncingRectangle(16, Vector2f(randomFloat(x - 16, 1, false), randomFloat(y - 16, 1, false)), randomColor());
		shapes[j]->impulse(Vector2f(randomFloat(8, 1, true), randomFloat(8, 1, true)));
		shapes[j]->spin(randomFloat(8, 1, true));
	}
	total += j - total;
}

// Start game loop
while (window.isOpen())
{
	// Process events
	sf::Event Event;
	while (window.pollEvent(Event))
	{
		// Close window : exit
		if (Event.type == sf::Event::Closed)
		window.close();
		// Escape key : exit
		if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::Escape))
			window.close();
		if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::Num1))
			shapes[0]->setPosition(sf::Vector2f(sf::Mouse().getPosition(window)));
		if ((Event.type == sf::Event::KeyPressed) && (Event.key.code == sf::Keyboard::Num2))
			shapes[1]->setPosition(sf::Vector2f(sf::Mouse().getPosition(window)));
		
	}

	//Declare looping vars
	int i, j;

	//update items
	for (i = 0; i < total; i++)
	{
		shapes[i]->update(0.05, x, y); 		
	}

	//check collisions
	for (i = 0; i < total - 1; i++)
	{
		bool collision = false;
		for (j = i + 1; j < total; j++)
		{
			if (col.CheckBoundingSphere(shapes[i]->getPosition(), shapes[j]->getPosition(), shapes[i]->getRadius(), shapes[j]->getRadius()))
			{
				if (col.CheckCollisionSAT(*shapes[i], *shapes[j]))
				{
					shapes[i]->bounceOther(shapes[j]->getPosition());
					shapes[j]->bounceOther(shapes[i]->getPosition());
				}
			}
		}
	}

	//prepare frame
	window.clear();

	//draw frame items	
	for (int i = 0; i < total; i++)
	{
		//shapes[i]->draw(window);
		window.draw(shapes[i]->getShape());
		//window.setTitle("" + sf::Mouse().getPosition(window).x + ", " + sf::Mouse().getPosition(window).x);
	}

	// Finally, display rendered frame on screen
	window.display();
	
	} //loop back for next frame
	return EXIT_SUCCESS;
}
