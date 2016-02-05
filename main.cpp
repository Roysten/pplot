//TODO Use VAO's to support multiple models at once

#include <iostream>

#include "openglwindow.h"
#include "model.h"

#include <QtGui/QGuiApplication>
#include <QtGui/QMatrix4x4>
#include <QtGui/QOpenGLShaderProgram>
#include <QtGui/QOpenGLVertexArrayObject>
#include <QtGui/QScreen>

#include <QVector>
#include <QVector3D>

#include <QtCore/qmath.h>

class ParamWindow : public OpenGLWindow
{
public:
    ParamWindow();

    void initialize() Q_DECL_OVERRIDE;
    void render() Q_DECL_OVERRIDE;

private:
    GLuint m_posAttr;
    GLuint m_normAttr;
    GLuint m_matrixUniform;

	QVector<Model> models;
	//QVector<QOpenGLVertexArrayObject> vaos;

    QOpenGLShaderProgram *m_program;
    int m_frame;
};

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
