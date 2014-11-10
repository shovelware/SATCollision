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
	int y = 800;
	int framerate = 900;

	const enum modes{BALLS, TRIS, BOXES, ALL};
	int mode;
	const int balls = 10;
	const int triangles = 1;
	const int boxes = 10;
	
	Ball* pit[balls];
	Triangle* mesh[triangles];
	BouncingObject* shapes[balls + triangles + boxes];

// Create the main window
sf::RenderWindow window(sf::VideoMode(x, y, 32), "BOUNCING SHAPES");
window.setFramerateLimit(framerate);
srand(NULL);

mode = modes::BALLS;

if (mode == modes::BALLS || mode == modes::ALL)
{
	for (int i = 0; i < balls; i++)
	{
		pit[i] = new Ball(16.0f, Vector2f(400, 400), randomColor());
		//pit[i] = Ball(16.0f, Vector2f(randomFloat(x, 1, false), randomFloat(y, 1, false)), Color(200, 25, 25, 255));
		pit[i]->impulse(Vector2f(randomFloat(800, 1000, true), randomFloat(800, 1000, true)));
	}
}

else if (mode == modes::TRIS || mode == modes::ALL)
{	
	for (int i = 0; i < triangles; i++)
	{
		mesh[i] = new Triangle(64, Vector2f(400, 300), randomColor());
		//mesh[i] = Triangle(24, Vector2f(randomFloat(x, 1, false), randomFloat(y, 1, false)), Color(200, 25, 25, 255));
		//mesh[i].impulse(Vector2f(randomFloat(800, 1000, true), randomFloat(800, 1000, true)));
	}
}

else if (mode == modes::BOXES || mode == modes::ALL)
{
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
	}


	//update items
	
	if (mode == modes::BALLS || mode == modes::ALL)
	{
		for (int i = 0; i < balls; i++)
		{
			pit[i]->update(1, x, y);
		}
	}
	
	
	else if (mode == modes::TRIS || mode == modes::ALL)
	{
		for (int i = 0; i < triangles; i++)
		{
			mesh[i]->update(1, x, y);
		}

	}
	
	else if (mode == modes::BOXES || mode == modes::ALL)
	{
	}
	//prepare frame
	window.clear();
	 
	//draw frame items
	if (mode == modes::BALLS)
	{
		for (int i = 0; i < balls; i++)
		{
			//window.draw(pit[i].getShape());
			pit[i]->draw(window);
		}
	}
	
	else if (mode == modes::TRIS)
	{		
		for (int i = 0; i < triangles; i++)
		{
			mesh[i]->draw(window);
		}
	}
	
	else if (mode == modes::BOXES)
	{
	}
	// Finally, display rendered frame on screen
	window.display();
	
	} //loop back for next frame
	return EXIT_SUCCESS;
}