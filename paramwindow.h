#ifndef PARAMWINDOW_H
#define PARAMWINDOW_H

#include <QVector>
#include <QtGui/QOpenGLVertexArrayObject>
#include <QtGui/QOpenGLShaderProgram>

#include "model.h"
#include "openglwindow.h"

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
		QVector<QOpenGLVertexArrayObject> vaos;

		QOpenGLShaderProgram *m_program;
		int m_frame;
};

#endif
