#include "Dice.h"
#include <gl/glut.h>
#include "squadric.h"
#include "tga.h"

GLuint texture[1];

Dice::Dice(cyclone::real halfsize, cyclone::real x, cyclone::real y, cyclone::real z)
{
	box.halfSize = cyclone::Vector3(halfsize, halfsize, halfsize);
	box.body = new cyclone::RigidBody();

	cyclone::real mass = box.halfSize.x * box.halfSize.y * box.halfSize.z * 2.0f;
	box.body->setMass(mass);

	cyclone::Matrix3 tensor;
	tensor.setBlockInertiaTensor( box.halfSize, mass );
    box.body->setInertiaTensor( tensor );

	box.body->setDamping(1.0, 1.0);
	box.body->clearAccumulators();
	box.body->setAcceleration(cyclone::Vector3::GRAVITY);

	box.body->setCanSleep(true);
	box.body->setAwake(true);
	
		
	sphere.body = new cyclone::RigidBody();
	sphere.radius = halfsize * 0.75;
	
	box.body->setPosition(x, y, z);
	sphere.body->setPosition(x, y, z);

	box.body->calculateDerivedData();

	// Load Texture
	loadTGA ("D:\\dices.bmp",this->tgaFile);
	glGenTextures(1, &texture[0]);
	glBindTexture (GL_TEXTURE_2D, texture[0]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, tgaFile.width, tgaFile.height, 0, GL_RGB, GL_UNSIGNED_BYTE, tgaFile.data);
}

Dice::~Dice(void)
{
}

void Dice::recalculate() {
	// Calculate the Transform Matrix
	box.body->calculateDerivedData();

	// Calculate internal contact data
	box.calculateInternals();
}

void Dice::render()
{
	// Get the OpenGL transformation
    GLfloat mat[16];
    box.body->getGLTransform( mat );

    glPushMatrix();
        glMultMatrixf( mat );

        glPushMatrix();
            glScalef( box.halfSize.x, box.halfSize.y, box.halfSize.z );
			glColor3f(1,0,0);
			sqSolidRoundCube( box.halfSize.x*2, 30, 30 );
			//glutWireSphere(sphere.radius, 30, 20);

			glutSolidCube( box.halfSize.x );

        glPopMatrix();
    glPopMatrix();
}

static void
drawBox(GLfloat size, GLenum type)
{
  static GLfloat n[6][3] =
  {
    {-1.0, 0.0, 0.0},
    {0.0, 1.0, 0.0},
    {1.0, 0.0, 0.0},
    {0.0, -1.0, 0.0},
    {0.0, 0.0, 1.0},
    {0.0, 0.0, -1.0}
  };
  static GLint faces[6][4] =
  {
    {0, 1, 2, 3},
    {3, 2, 6, 7},
    {7, 6, 5, 4},
    {4, 5, 1, 0},
    {5, 6, 2, 1},
    {7, 4, 0, 3}
  };
  GLfloat v[8][3];
  GLint i;

  v[0][0] = v[1][0] = v[2][0] = v[3][0] = -size / 2;
  v[4][0] = v[5][0] = v[6][0] = v[7][0] = size / 2;
  v[0][1] = v[1][1] = v[4][1] = v[5][1] = -size / 2;
  v[2][1] = v[3][1] = v[6][1] = v[7][1] = size / 2;
  v[0][2] = v[3][2] = v[4][2] = v[7][2] = -size / 2;
  v[1][2] = v[2][2] = v[5][2] = v[6][2] = size / 2;

  //glColor3f(1,0,0);
  glEnable (GL_TEXTURE_2D); /* enable texture mapping */
  glBindTexture (GL_TEXTURE_2D, texture[0]);

  /*for (i = 5; i >= 0; i--) {
    glBegin(type);
	//glColor3f(i/5.f, i/5.f, i/5.f);
	
    glNormal3fv(&n[i][0]);
    glVertex3fv(&v[faces[i][0]][0]);
    glVertex3fv(&v[faces[i][1]][0]);
    glVertex3fv(&v[faces[i][2]][0]);
    glVertex3fv(&v[faces[i][3]][0]);

    glEnd();
  }*/

  glBegin(GL_QUADS);
  // Front Face
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f,1.0f);// Bottom Left Of The Texture and Quad
	glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f,1.0f);// Bottom Right Of The Texture and Quad
	glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,1.0f,1.0f);// Top Right Of The Texture and Quad
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,1.0f,1.0f);// Top Left Of The Texture and Quad
	// Back Face
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);// Bottom Right Of The Texture and Quad
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,1.0f, -1.0f);// Top Right Of The Texture and Quad
	glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,1.0f, -1.0f);// Top Left Of The Texture and Quad
	glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f);// Bottom Left Of The Texture and Quad
	// Top Face
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,1.0f, -1.0f);// Top Left Of The Texture and Quad
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f,1.0f,1.0f);// Bottom Left Of The Texture and Quad
	glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f,1.0f,1.0f);// Bottom Right Of The Texture and Quad
	glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,1.0f, -1.0f);// Top Right Of The Texture and Quad
	// Bottom Face
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f);// Top Right Of The Texture and Quad
	glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f, -1.0f, -1.0f);// Top Left Of The Texture and Quad
	glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f,1.0f);// Bottom Left Of The Texture and Quad
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,1.0f);// Bottom Right Of The Texture and Quad
	// Right face
	glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f);// Bottom Right Of The Texture and Quad
	glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,1.0f, -1.0f);// Top Right Of The Texture and Quad
	glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,1.0f,1.0f);// Top Left Of The Texture and Quad
	glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f,1.0f);// Bottom Left Of The Texture and Quad
	// Left Face
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);// Bottom Left Of The Texture and Quad
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,1.0f);// Bottom Right Of The Texture and Quad
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,1.0f,1.0f);// Top Right Of The Texture and Quad
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,1.0f, -1.0f);// Top Left Of The Texture and Quad
	glEnd();

   glDisable (GL_TEXTURE_2D);
}

void APIENTRY
glutSolidCube(GLdouble size)
{
  drawBox(size, GL_QUADS);
}

void Dice::integrate(cyclone::real duration)
{
	box.body->integrate(duration);
	box.calculateInternals();

	// Update the sphere position
    sphere.body->setPosition( box.body->getPosition() );
	sphere.body->setOrientation(box.body->getOrientation() );
}

void Dice::createContactsDice(Dice *dice, cyclone::CollisionData *data)
{
	// If the boxes and spheres of both dices are colliding, the dices are colliding
	if( cyclone::IntersectionTests::boxAndBox( this->box, dice->box ) &&
		cyclone::IntersectionTests::sphereAndSphere( this->sphere, dice->sphere ) )
    {
		cyclone::CollisionDetector::boxAndBox( box, dice->box, data );
    }
}

void Dice::createContactsPlane(cyclone::CollisionPlane *plane, cyclone::CollisionData *data) 
{
	// If box and sphere collide with the plane, then the dice collides
	if( cyclone::IntersectionTests::boxAndHalfSpace( box, *plane ) &&
		cyclone::IntersectionTests::sphereAndHalfSpace( sphere, *plane ) )
    {
        cyclone::CollisionDetector::boxAndHalfSpace( box, *plane, data );
    }
}

void Dice::setOrientation(cyclone::real r,cyclone::real i, cyclone::real j, cyclone::real k)
{
	box.body->setOrientation(r, i, j, k);
}

void Dice::setOrientation(cyclone::Quaternion &orientation)
{
	box.body->setOrientation(orientation);
}

void Dice::setRotation(cyclone::real x, cyclone::real y, cyclone::real z)
{
	box.body->setRotation(x, y, z);
}

void Dice::setRotation(cyclone::Vector3 &rotation)
{
	box.body->setRotation(rotation);
}

void Dice::setAcceleration(cyclone::real x, cyclone::real y, cyclone::real z)
{
	box.body->setAcceleration(x, y, z);
}

void Dice::setAcceleration(cyclone::Vector3 &rotation)
{
	box.body->setAcceleration(rotation);
}