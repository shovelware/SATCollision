#ifndef BOUNCINGSHAPES_H
#define BOUNCINGSHAPES_H

using std::vector;
using sf::Vector2f;
using sf::Transform;
using sf::Color;
using sf::CircleShape;
using sf::ConvexShape;

class BouncingObject {

public:
	BouncingObject(int vertices = 8, float radius = 16, Vector2f position = Vector2f(400, 300), Color color = Color(rand() % 255, rand() % 255, rand() % 255, 255));
	~BouncingObject();
	
	void impulse(Vector2f impulse);
	void spin(float spin);
	void stop();
	
	void checkWallsRough(int width, int height);
	void checkWalls(int width, int height);
	void bounce(Vector2f newVelo);
	void bounceN(Vector2f N);
	void bounceOther(Vector2f otherPos);
	void bounceCols();

	void randColor();

	void update(float const timescale, int width, int height);
	void draw(sf::RenderWindow& w);

	//Accessors
	void setMove(bool const move);
	void setRotate(bool const rotate);
	void setDraw(bool const draw);
	void setColor(Color const color);
	vector<sf::Vector2f> getPoints();
	vector<sf::Vector2f> getPointsRelative();
	void setPosition(sf::Vector2f newPos) { m_shape.setPosition(newPos); }

	const bool getMove() { return m_move; }
	const bool getRotate() { return m_rotate; }
	const bool getDraw() { return m_draw; }
	const Color getColor() { return m_shape.getFillColor(); }
	const Vector2f getVelo() { return m_velocity; }
	const Vector2f getPosition() {return m_shape.getPosition(); }
	const ConvexShape getShape() { return m_shape; }
	const float getRadius() { return m_size; }
	
private:
	int m_pointnum;
	vector<Vector2f> m_points;
	void setPoints();

	Transform m_trans;
	
	bool m_move;
	bool m_rotate;
	bool m_draw;
	bool m_collided;
	float m_size;
	int m_bounceTime;
	int m_bounceTimeMax;

	float dotProd(Vector2f a, Vector2f b);

	sf::ConvexShape m_shape;
	Vector2f m_velocity;
	float m_rotationvelo;

};

class BouncingBall : public BouncingObject{

public:
	BouncingBall(float radius = 16, Vector2f position = Vector2f(400, 300), Color color = Color(rand() % 255, rand() % 255, rand() % 255, 255)) 
		: BouncingObject(360, radius, position, color){ }

};

class BouncingTriangle : public BouncingObject{

public:
	BouncingTriangle(float radius = 16, Vector2f position = Vector2f(0, 0), Color color = Color(rand() % 255, rand() % 255, rand() % 255, 255)) 
		: BouncingObject(3, radius, position, color){}
};

class BouncingRectangle : public BouncingObject{

public:
	BouncingRectangle(float radius = 16, Vector2f position = Vector2f(400, 300), Color color = Color(rand() % 255, rand() % 255, rand() % 255, 255)) 
		: BouncingObject(4, radius, position, color){}
};

class BouncingPentagon : public BouncingObject{

public:
	BouncingPentagon(float radius = 16, Vector2f position = Vector2f(400, 300), Color color = Color(rand() % 255, rand() % 255, rand() % 255, 255)) 
		: BouncingObject(5, radius, position, color){}
};

class BouncingHexagon : public BouncingObject{

public:
	BouncingHexagon(float radius = 16, Vector2f position = Vector2f(400, 300), Color color = Color(rand() % 255, rand() % 255, rand() % 255, 255)) 
		: BouncingObject(6, radius, position, color){}
};

class BouncingSeptagon : public BouncingObject{

public:
	BouncingSeptagon(float radius = 16, Vector2f position = Vector2f(400, 300), Color color = Color(rand() % 255, rand() % 255, rand() % 255, 255)) 
		: BouncingObject(7, radius, position, color){}
};

class BouncingOctagon : public BouncingObject{

public:
	BouncingOctagon(float radius = 16, Vector2f position = Vector2f(400, 300), Color color = Color(rand() % 255, rand() % 255, rand() % 255, 255)) 
		: BouncingObject(8, radius, position, color){}
};

class Ball {

public:
	Ball(float radius = 16, Vector2f position = Vector2f(0, 0), Color color = Color(rand() % 255, rand() % 255, rand() % 255, 255));
	~Ball();
	
	void impulse(sf::Vector2f impulse);
	void stop();
	
	void update(float const timescale, int x, int y);
	void checkWalls(int width, int height);
	void bounce(float x, float y);

	//Accessors
	void setMove(bool const move);
	void setDraw(bool const draw);
	void setColor(sf::Color const color);

	bool getMove() const;
	bool getDraw() const;
	Color getColor() const;
	Vector2f getPosition() const;
	CircleShape getShape() const;
	void draw(sf::RenderWindow& w);
	
private:
	bool m_move;
	bool m_draw;
	bool m_collided;
	int m_bounceTime;

	sf::CircleShape m_circle;

	Vector2f m_centre;
	Vector2f m_velocity;
	Color m_color;

};

#endif