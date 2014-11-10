#ifndef COLLISIONDETECTION_H
#define COLLISIONDETECTION_H

#include "stdafx.h"

#include <vector>
#include "BouncingShapes.h"

using std::vector;
using sf::Vector2f;
using std::cout;
//using std::iostream;
using std::endl;

class CollisionDetection {
private:
	vector<Vector2f> zeroPoint, shouPoint;
	vector<Vector2f> zeroEdges, shouEdges;
	vector<Vector2f> zeroSAxes, shouSAxes;
public:
	CollisionDetection() {}

	bool CheckCollisionSAT(BouncingObject zero, BouncingObject shou)
	{
		//Clear everything before we start
		ClearVecs();
		bool collision = false;

		//Get points
		zeroPoint = zero.getPointsRelative();
		shouPoint = shou.getPointsRelative();
		//Get Edges
		zeroEdges = GetEdges(zeroPoint);
		shouEdges = GetEdges(shouPoint);
		//Get Axes
		zeroSAxes = GetSAxes(zeroEdges);
		shouSAxes = GetSAxes(shouEdges);
		//??Check for dupes
		//Check SAT
		collision = CheckSAT(zeroSAxes, shouSAxes, zeroPoint, shouPoint);
		//CheckSAT(GetSAxes(GetEdges(zeroPoint)), GetSAxes(GetEdges(shouPoint)), zeroPoint, shouPoint);
		
		//Clear everything and return the result
		ClearVecs();

		return collision;

	}
	//Get all edges, check for dupes
	//Get perpendicular normal to each edge
	//Cross each point with perp
	//if zmax > smin || smax > zmin then collide

	//Get edges from points
	vector<Vector2f> GetEdges(const vector<Vector2f> points)
	{
		vector<Vector2f> edges;
		int i, j;

		//Take each point from the previous, including first from last
		for (i = 0; i < points.size() - 1; i++)
		{
			j = i + 1;
			edges.push_back(points.at(i) - points.at(j));
		}

		edges.push_back(points.at(i) - points.at(0));

		return edges;
	}

	//Get Separating Axes from edges
	vector<Vector2f> GetSAxes(const vector<Vector2f> edges)
	{
		//Take the perpendicular of each edge and normalise
		vector<Vector2f> SAxes;
		sf::Vector2f vec;
		float div;
		int i;

		for (i = 0; i < edges.size(); i++)
		{
			vec = sf::Vector2f(-edges.at(i).y, edges.at(i).x);
			div = sqrt(vec.x * vec.x + vec.y * vec.y);
			SAxes.push_back(vec / div);
		}

		return SAxes;
	}

	bool CheckBoundingSphere(Vector2f alphaPos, Vector2f omegaPos, float alphaRad, float omegaRad)
	{
		//If distance between positions is greater than radii added no collision
		//sqrt ((x2-x1)^2 + (y2-y1)^2)
		float dist = sqrt(((omegaPos.x - alphaPos.x) * (omegaPos.x - alphaPos.x)) + ((omegaPos.y - alphaPos.y) * (omegaPos.y - alphaPos.y)));
		
		return (dist < alphaRad + omegaRad);
	}

	//Check two lists of points against two sets of Separating Axes
	bool CheckSAT(const vector<Vector2f> alphaSAxes, const vector<Vector2f> omegaSAxes, const vector<Vector2f> alphaPoint, const vector<Vector2f> omegaPoint)
	{
		//Test all points on each SA (Dot product!), if there's a gap no collision and stop, if you run out of edges there is a collision!
		float alphaMin, omegaMin;
		float alphaMax, omegaMax;
		float k;
		int i, j;

		//Combine all SAxes into one vector
		//!! RESERVE SPACE !!
		vector<Vector2f> gammaSAxes;
		gammaSAxes.reserve(alphaSAxes.size() + omegaSAxes.size());
		gammaSAxes.insert(gammaSAxes.end(), alphaSAxes.begin(), alphaSAxes.end());
		gammaSAxes.insert(gammaSAxes.end(), omegaSAxes.begin(), omegaSAxes.end());

		//Test points on each SA
		for (i = 0; i < gammaSAxes.size(); i++)
		{
			//Reset min and max
			alphaMin = omegaMin = FLT_MAX;
			alphaMax = omegaMax = -FLT_MAX;

			//Check alpha against SA
			for (j = 0; j < alphaPoint.size(); j++)
			{
				k = alphaPoint.at(j).x * gammaSAxes.at(i).x + alphaPoint.at(j).y * gammaSAxes.at(i).y;
				if (alphaMax < k)
					alphaMax = k;
				if (alphaMin > k)
					alphaMin = k;
			}

			//Check omega against SA
			for (j = 0; j < omegaPoint.size(); j++)
			{
				k = omegaPoint.at(j).x * gammaSAxes.at(i).x + omegaPoint.at(j).y * gammaSAxes.at(i).y;
				if (omegaMax < k)
					omegaMax = k;
				if (omegaMin > k)
					omegaMin = k;
			}

			//Check for a gap , if there is, there's no collision and return false
			if (alphaMax < omegaMin || omegaMax < alphaMin)
			{
				return false;
			}
		}
		//For loop exits, we've run out of axes, so there is a collision
		return true;
	}

	void ClearVecs()
	{
		zeroPoint.clear();
		shouPoint.clear();
		zeroEdges.clear();
		shouEdges.clear();
		zeroSAxes.clear();
		shouSAxes.clear();
	}

	void DupeCheck(vector<Vector2f> vec)
	{
		for (int i = 0; i < vec.size() - 1; i++)
		{
			for (int j = i + 1; j < vec.size(); j++)
			{
				if (vec.at(i) == vec.at(j))
				{
					
				}
			}
		}
	}
};

#endif