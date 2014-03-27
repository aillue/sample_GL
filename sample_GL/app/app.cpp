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
	glTranslatef( 0.0f, 0.0f, -2.f );
	glRotatef( angle_ , 0.0f, 1.0f, 0.0f );

	glBegin( GL_TRIANGLE_STRIP );
	glColor3f( 1.f, 1.f, 1.f );
	glVertex3f( -1.0f, -1.0f, 0.0f );
	glColor3f( 1.f, .4f, .4f );
	glVertex3f( -1.0f, 1.0f, 0.0f );
	glColor3f( .4f, 1.f, .4f );
	glVertex3f( 1.0f, -1.0f, 0.0f );
	glColor3f( .4f, .4f, 1.f );
	glVertex3f( 1.0f, 1.0f, 0.0f );
	glEnd();

	glPopMatrix ();

	return true;
}