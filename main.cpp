#include "openglwindow.h"

#include <QtGui/QGuiApplication>
#include <QtGui/QMatrix4x4>
#include <QtGui/QOpenGLShaderProgram>
#include <QtGui/QScreen>
#include <iostream>

#include <QVector3D>

#include <QtCore/qmath.h>

class ParamWindow : public OpenGLWindow
{
public:
    ParamWindow();
	~ParamWindow();

    void initialize() Q_DECL_OVERRIDE;
    void render() Q_DECL_OVERRIDE;

private:
	void generatePoints(QVector3D* points);
	void generateVertices(QVector3D* points);
	void calculateNormals();

    GLuint m_posAttr;
    GLuint m_normAttr;
    GLuint m_matrixUniform;

	QVector3D* vertices;
	QVector3D* normals;

	GLfloat xmin;
	GLfloat xmax;
	GLfloat ymin;
	GLfloat ymax;
	GLint xstep;
	GLint ystep;

    QOpenGLShaderProgram *m_program;
    int m_frame;
};

ParamWindow::~ParamWindow()
{
	delete[] normals;
	delete[] vertices;
}

ParamWindow::ParamWindow()
	: xmin(0)
	, xmax(6.28)
	, ymin(0)
	, ymax(3.14)
	, xstep(20)
	, ystep(10)
    , m_program(0)
    , m_frame(0)
{
	//Account for inclusive formulas used in maths
	xstep +=1; 
	ystep += 1;

	normals = new QVector3D[xstep * ystep * 6];
	vertices = new QVector3D[xstep * ystep * 6];
	
	QVector3D points[xstep * ystep];
	generatePoints(points);
	generateVertices(points);
	calculateNormals();
}

void ParamWindow::generateVertices(QVector3D* points)
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

void ParamWindow::generatePoints(QVector3D* points)
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

void ParamWindow::calculateNormals()
{
	for(int i = 0; i < xstep * ystep * 6; i += 3)
	{
		QVector3D u = vertices[i + 1] - vertices[i];
		QVector3D v = vertices[i + 2] - vertices[i];

		QVector3D normal = QVector3D(
			u.y() * v.z() - u.z() * v.y(),
			u.z() * v.x() - u.x() * v.z(),
			u.x() * v.y() - u.y() * v.x());

		normals[i] = normal;
		normals[i + 1] = normal;
		normals[i + 2] = normal;
	}	
}

int main(int argc, char **argv)
{
    QGuiApplication app(argc, argv);

    ParamWindow window;
    window.resize(640, 480);
    window.show();

    window.setAnimating(true);

    return app.exec();
}

void ParamWindow::initialize()
{
    m_program = new QOpenGLShaderProgram(this);
    m_program->addShaderFromSourceFile(QOpenGLShader::Vertex, "./shaders/vertex.glsl");
    m_program->addShaderFromSourceFile(QOpenGLShader::Fragment, "./shaders/fragment.glsl");
    m_program->link();

    m_posAttr = m_program->attributeLocation("posAttr");
	m_normAttr = m_program->attributeLocation("normAttr");
    m_matrixUniform = m_program->uniformLocation("matrix");
}

void ParamWindow::render()
{
	glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    const qreal retinaScale = devicePixelRatio();
    glViewport(0, 0, width() * retinaScale, height() * retinaScale);

    m_program->bind();

    QMatrix4x4 matrix;
    matrix.perspective(60.0f, 4.0f/3.0f, 5.0f, 100.0f);
    matrix.translate(0, 0, -10);
    matrix.rotate(50.0f * m_frame / screen()->refreshRate(), 0, 1, 0);

    m_program->setUniformValue(m_matrixUniform, matrix);

    glVertexAttribPointer(m_posAttr, 3, GL_FLOAT, GL_FALSE, 0, vertices);
    glVertexAttribPointer(m_normAttr, 3, GL_FLOAT, GL_FALSE, 0, normals);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glDrawArrays(GL_TRIANGLES, 0, xstep * ystep * 6);

    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(0);

    m_program->release();

    ++m_frame;
}
