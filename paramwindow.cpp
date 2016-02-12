#include <QtGui/QMatrix4x4>
#include <QtGui/QScreen>
#include <QVector3D>

#include "paramwindow.h"

ParamWindow::ParamWindow()
	: m_program(0)
	  , m_frame(0)
{
	models.append(Model());

	/*for(QVector<Model>::iterator elem = models.begin(); elem != models.end(); ++elem)
	  {
	  QOpenGLVertexArrayObject vao = new QOpenGLVertexArrayObject(this);
	  }*/
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
	matrix.perspective(60.0f, width()/height(), 5.0f, 100.0f);
	matrix.translate(0, 0, -10);
	matrix.rotate(50.0f * m_frame / screen()->refreshRate(), 0, 1, 0);

	m_program->setUniformValue(m_matrixUniform, matrix);

	Model m = models.first();

	glVertexAttribPointer(m_posAttr, 3, GL_FLOAT, GL_FALSE, 0, m.getVertices()->data());
	glVertexAttribPointer(m_normAttr, 3, GL_FLOAT, GL_FALSE, 0, m.getNormals()->data());

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glDrawArrays(GL_TRIANGLES, 0, m.getVertices()->length());

	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);

	m_program->release();

	++m_frame;
}
