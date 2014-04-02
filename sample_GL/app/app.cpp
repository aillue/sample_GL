#include "stdafx.h"
#include "app.h"

MyApp::MyApp() : angle_( 0.0f )
{
}

MyApp::~MyApp()
{
}

bool MyApp::Update()
{
	angle_ += 1.f;

	glClearColor( 0.0f, 0.5f, 1.0f, 1.0f );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	glEnable( GL_DEPTH_TEST );
	glDisable( GL_LIGHTING );

	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	glFrustum( 1 , -1 , -1 , 1 , 1, 10 );

	glMatrixMode( GL_MODELVIEW );

	float m[16] =
	{
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};
	glPushMatrix ();
	glLoadIdentity();
	glTranslatef( 0.0f, 0.0f, -3.f );
	glRotatef( angle_ , 0.0f, 1.0f, 0.0f );

	const GLfloat vertex[][3] = {
		{-1,-1,-1},
		{+1,-1,-1},
		{-1,+1,-1},
		{+1,+1,-1},
		{-1,-1,+1},
		{+1,-1,+1},
		{-1,+1,+1},
		{+1,+1,+1},
	};
	const GLfloat color[][3]={
		{ 0.f, 0.f, 0.f },
		{ 1.f, 0.f, 0.f },
		{ 0.f, 1.f, 0.f },
		{ 1.f, 1.f, 0.f },
		{ 0.f, 0.f, 1.f },
		{ 1.f, 0.f, 1.f },
		{ 0.f, 1.f, 1.f },
		{ 1.f, 1.f, 1.f },
	};

	const GLint index[][3] = {
		{0,1,2},
		{3,2,1},
		{1,5,3},
		{7,3,5},
		{5,4,7},
		{6,7,4},
		{4,6,0},
		{2,0,6},
		{0,1,4},
		{5,4,1},
		{3,2,7},
		{6,7,2},
	};

	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3 , GL_FLOAT , 0 , &vertex[0][0] );
	glBegin( GL_TRIANGLES );
	for(int i = 0; i < 12; i++){
		GLint i0 = index[i][0];
		GLint i1 = index[i][1];
		GLint i2 = index[i][2];
		glColor3f( color[i0][0], color[i0][1], color[i0][2] );
		glArrayElement(i0);
		glColor3f( color[i1][0], color[i1][1], color[i1][2] );
		glArrayElement(i1);
		glColor3f( color[i2][0], color[i2][1], color[i2][2] );
		glArrayElement(i2);

	}
	glDisableClientState(GL_VERTEX_ARRAY);
	glEnd();

	glPopMatrix ();

	return true;
}