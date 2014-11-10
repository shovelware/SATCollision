#include "stdafx.h"
#include "BouncingShapes.h"

using sf::Transform;

#define _USE_MATH_DEFINES
#include "math.h"

#define RD 180/M_PI
#define DR M_PI/180

#pragma region Shape

BouncingObject::BouncingObject(int vertices, float radius, Vector2f position, Color color)
	{
		m_shape.setPosition(position.x, position.y);
		m_shape.setFillColor(color);
		m_shape.setOutlineThickness(- (radius / 8));
		m_shape.setOutlineColor(Color(64, 64, 64, 255));
		m_shape.setPointCount(vertices);
		m_bounceTimeMax = 16;
				
		m_pointnum = vertices;
		m_size = radius;

		for (int i = 0; i < m_pointnum; i++)
		{
			m_points.push_back(radius * Vector2f(cos(DR * 360 / vertices * i), sin(DR * 360 / vertices * i)));
		}

		m_move = true;
		m_rotate = true;
		m_draw = true;

		m_trans.Identity;

		setPoints();
	}

BouncingObject::~BouncingObject(){}

void BouncingObject::impulse(sf::Vector2f impulse)
{
	m_velocity += impulse;
}

void BouncingObject::spin(float spin)
{
	m_rotationvelo = spin;
	m_trans.rotate(m_rotationvelo);
}

void BouncingObject::stop()
{
	m_velocity = Vector2f(0, 0);
	m_rotationvelo = 0;
}

void BouncingObject::checkWallsRough(int width, int height)
{
	float x = m_velocity.x;
	float y = m_velocity.y;

	//Left || Right
	if ((m_shape.getPosition().x - m_size <= 0) || (m_shape.getPosition().x + m_size >= width / 2))
		bounce(Vector2f(-x, y));
	
	//Top || Bottom
	if ((m_shape.getPosition().y + m_size <= 0) || (m_shape.getPosition().y + m_size >= height / 2))
		bounce(Vector2f(x, -y));
}

float BouncingObject::dotProd(Vector2f a, Vector2f b)
{
	return a.x * b.x + a.y * b.y;
}

void BouncingObject::checkWalls(int width, int height)
{
	if (m_bounceTime <= 0)
	{
		float x = m_velocity.x;
		float y = m_velocity.y;

		for (int i = 0; i < m_pointnum; i++)
		{
			float j;
			//a · b = ax × bx + ay × by
			//velox * normx + veloy * normy

			//Left
			if ((m_shape.getPosition().x - m_points[i].x) <= 0)
				//if (! dotProd(m_velocity, Vector2f(1, 0)) > 20)
			{
			
				j = dotProd(m_velocity, Vector2f(1, 0));
				bounce(Vector2f(-x, y));
			}

			//Right
			if ((m_shape.getPosition().x + m_points[i].x) >= width)
				//if (! (m_velocity.x * -1 + m_velocity.y * 0) > 0 )
					bounce(Vector2f(-x, y));
			//Top
			if ((m_shape.getPosition().y - m_points[i].y) <= 0)
				//if (! (m_velocity.x * 0 + m_velocity.y * 1) > 0)
					bounce(Vector2f(x, -y));
			
			//Bottom
			if ((m_shape.getPosition().y + m_points[i].y) >= height)
				//if(! (m_velocity.x * 0 + m_velocity.y * -1) > 0)
					bounce(Vector2f(x, -y));

		}
	}
}

void BouncingObject::bounceN(Vector2f N)
{
	m_velocity = m_velocity - 2 * (m_velocity.x * N.x + m_velocity.y * N.y) * N;
	bounceCols();
}

void BouncingObject::bounceOther(Vector2f otherPos)
{
	Vector2f N = getPosition() - otherPos;
	N /= sqrt(N.x * N.x + N.y * N.y);
	m_velocity = m_velocity - 2 * (m_velocity.x * N.x + m_velocity.y * N.y) * N;
	bounceCols();
}

void BouncingObject::bounce(Vector2f newVelo)
{
	m_velocity = newVelo;
	bounceCols();
}

void BouncingObject::bounceCols()
{
	m_shape.setOutlineColor(Color(255, 255, 255, 255));
	m_collided = true;
	m_bounceTime = m_bounceTimeMax;
}

void BouncingObject::update(float const timescale, int x, int y)
{
	if (m_collided)
	{
		if (m_bounceTime > 0)
			m_bounceTime--;
		else
		{
			//reset edge col
			m_shape.setOutlineColor(Color(64, 64, 64, 255));
			m_collided = false;
		}
	}

	//checkWallsRough(x, y);
	checkWalls(x, y);

	if (m_move)
		/*Maybe use .move?*/m_shape.setPosition(m_shape.getPosition() + ((m_velocity ) * timescale));

	if (m_rotate)
		for (int i = 0; i < m_pointnum; i++)
		{
			m_points[i] = m_trans.transformPoint(m_points[i]);
			setPoints();
		}
}

void BouncingObject::draw(sf::RenderWindow& w)
{
	if (m_draw)
	{
		w.draw(m_shape);
	
	}
}

//Accessors
void BouncingObject::setMove(bool const move)
{
	m_move = move;
}

void BouncingObject::setRotate(bool const rotate)
{
	m_rotate = rotate;
}

void BouncingObject::setDraw(bool const draw)
{
	m_draw = draw;
}

void BouncingObject::setColor(Color const color)
	{
		m_shape.setFillColor(color);
	}

void BouncingObject::setPoints()
{
	//Puts the points from m_points vector into m_shape
	for (int i = 0; i < m_points.size(); i++)
	{
		m_shape.setPoint(i, m_points.at(i));
	}
}

vector<sf::Vector2f> BouncingObject::getPoints()
{
	vector<sf::Vector2f> points;
	for (int i = 0; i < m_pointnum; i++)
	{
		points.push_back(m_shape.getPoint(i));
	}

	return points;
}

vector<sf::Vector2f> BouncingObject::getPointsRelative()
{
	vector<sf::Vector2f> points;
	for (int i = 0; i < m_pointnum; i++)
	{
		points.push_back(m_shape.getPoint(i) + m_shape.getPosition());
	}

	return points;
}

#pragma endregion

//override void BouncingBall::bounce()
//{
//}

#pragma region Ball

Ball::Ball(float radius, Vector2f position, Color color) : m_circle(radius)
{
	m_circle.setPosition(position.x - m_circle.getRadius(), position.y - m_circle.getRadius());
	m_circle.setFillColor(color);
	m_circle.setOutlineThickness(- (radius / 8));
	m_circle.setOutlineColor(Color(64, 64, 64, 255));
}

Ball::~Ball()
{
}

void Ball::impulse(sf::Vector2f impulse)
{
	m_velocity += impulse;
}

void Ball::stop()
{
	m_velocity = Vector2f(0, 0);
}

void Ball::update(float const timescale, int x, int y)
{
	if (m_collided)
	{
		if (m_bounceTime > 0)
			m_bounceTime--;
		else
		{
			//reset edge col
			m_circle.setOutlineColor(Color(64, 64, 64, 255));
			m_collided = false;
		}
	}

	m_centre = m_circle.getPosition() + Vector2f(m_circle.getRadius(), m_circle.getRadius());

	checkWalls(x, y);

	if (m_move)
		m_circle.setPosition(m_circle.getPosition() + ((m_velocity ) * timescale));

}

void Ball::checkWalls(int width, int height)
{
	float m_radius = m_circle.getRadius();
	//Left
	if (m_centre.x - m_radius <= 0)
		bounce(-1, 1);
	
	//Right
	if (m_centre.x + m_radius >= width)
		bounce(-1, 1);
	
	//Top
	if (m_centre.y + m_radius <= 0)
		bounce(1, -1);

	//Bottom
	if (m_centre.y + m_radius >= height)
		bounce(1, -1);
}

void Ball::bounce(float x, float y)
{
	m_velocity.x *= x;
	m_velocity.y *= y;
	m_circle.setOutlineColor(Color(255, 255, 255, 255));
	m_collided = true;
	m_bounceTime = 32;
}

//Accessors
void Ball::setMove(bool const move)
{
	m_move = move;
}

void Ball::setDraw(bool const draw)
{
	m_draw = draw;
}

void Ball::setColor(sf::Color const color)
{
	m_circle.setFillColor(color);
}

bool Ball::getMove() const
{
	return m_move;
}

bool Ball::getDraw() const
{
	return m_draw;
}

Color Ball::getColor() const
{
	return m_circle.getFillColor();
}

Vector2f Ball::getPosition() const
{
	//correct for top left
	return m_circle.getPosition() + Vector2f(m_circle.getRadius(), m_circle.getRadius());
}

sf::CircleShape Ball::getShape() const
{
	return m_circle;
}

void Ball::draw(sf::RenderWindow& w)
{
	w.draw(m_circle);
}

#pragma endregion