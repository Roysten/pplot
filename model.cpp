#include <QtCore/qmath.h>
#include <cmath>
#include "model.h"

Model::Model()
: xmin(0)
, xmax(M_PI * 2)
, ymin(0)
, ymax(M_PI)
, xstep(100)
, ystep(100)
{
	//Account for inclusive formulas used in maths
	xstep += 1; 
	ystep += 1;

	QVector<QVector3D> points(xstep * ystep);

	normals = Verts(xstep * ystep * 6);
	vertices = Verts(xstep * ystep * 6);

	generatePoints(points);
	generateVertices(points);
	calculateNormals();
}	

QVector<QVector3D>* Model::getVertices()
{
	return &vertices;
}

QVector<QVector3D>* Model::getNormals()
{
	return &normals;
}

void Model::generateVertices(Verts const& points)
{
	for(int i = 0; i < xstep - 1; ++i)
	{
		for(int j = 0; j < ystep - 1; ++j)
		{
			GLint pstart = i * ystep + j;
			GLint pstart2 = (i + 1) * ystep + j;
			GLint vstart = i * ystep * 6 + (j * 6);

			//Bottom triangle
			vertices[vstart] = points[pstart];
			vertices[vstart + 1] = points[pstart + 1];
			vertices[vstart + 2] = points[pstart2];

			//Top triangle
			vertices[vstart + 5] = points[pstart2];
			vertices[vstart + 4] = points[pstart2 + 1];
			vertices[vstart + 3] = points[pstart + 1];
		}
	}
}

void Model::generatePoints(Verts &points)
{
	for(GLint i = 0; i < xstep; ++i)
	{
		GLfloat x = ((xmax - xmin) / (xstep - 1)) * i;
		for(GLint j = 0; j < ystep; ++j)
		{
			GLfloat y = ((ymax - ymin) / (ystep - 1)) * j;
			GLint index = i * ystep + j;

			//Plane
			//points[index] = QVector3D(x, y, 1 - x - y);
			
			//Cone
			//points[index] = QVector3D(y * cos(x), y * sin(x), y);
			
			//Cylinder
			points[index] = QVector3D(2 * cos(x), 2 * sin(x), 2 * y);

			//Spheres
			//points[index] = QVector3D(2 * sin(x) * cos(y), 2 * sin(x) * sin(y), 2 * cos(x));
			//points[index] = QVector3D(2 * sin(y) * cos(x), 2 * sin(y) * sin(x), 2 * cos(y));
		}
	}
}

void Model::calculateNormals()
{
	for(int i = 0; i < xstep * ystep * 6; i += 3)
	{
		QVector3D u = vertices[i + 1] - vertices[i];
		QVector3D v = vertices[i + 2] - vertices[i];

		QVector3D normal = QVector3D(
			u.y() * v.z() - u.z() * v.y(),
			u.z() * v.x() - u.x() * v.z(),
			u.x() * v.y() - u.y() * v.x());
		normal.normalize();

		normals[i] = normal;
		normals[i + 1] = normal;
		normals[i + 2] = normal;
	}	
}
