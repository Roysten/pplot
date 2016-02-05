#ifndef MODEL_H
#define MODEL_H

#include <QVector>
#include <QVector3D>
#include <qopengl.h>

typedef QVector<QVector3D> Verts;

class Model
{
	public:
		Model();

		Verts* getVertices();
		Verts* getNormals();

	private:
		Verts vertices;
		Verts normals;

		GLfloat xmin;
		GLfloat xmax;
		GLfloat ymin;
		GLfloat ymax;

		GLint xstep;
		GLint ystep;

		void generatePoints(Verts &points);
		void generateVertices(Verts const& points);
		void calculateNormals();
};

#endif
