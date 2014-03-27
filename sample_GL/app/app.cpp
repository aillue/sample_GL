#include "stdafx.h"
#include "app.h"


bool MyApp::Update()
{
	glClearColor( 0.0f, 0.5f, 1.0f, 1.0f );
	glClear( GL_COLOR_BUFFER_BIT );

	glRectf( -0.5f, -0.5f, 0.5f, 0.5f );


	return true;
}