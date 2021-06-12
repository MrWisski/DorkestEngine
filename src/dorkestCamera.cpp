#include <Engine/Render/dorkestCamera.h>
#include <Util/Math/Matrix3.h>
#include <Util/Math/Geometry/nGons.h>
#include <instPGE.h>





dorkestCamera::dorkestCamera(int w, int h) : m_sWidth(w), m_sHeight(h), m_scale(1.0f)
{
	
}


dorkestCamera::~dorkestCamera()
{
}



Vector2i dorkestCamera::iTOc(Vector2f vTileSize, float scaleFactor, Vector3f map, Vector2f offset)
{
	//The grid is universal. OBEY THE GRID.
	float sizex = vTileSize.x;
	float sizey = vTileSize.y;
	float hsizex = sizex / 2;
	float hsizey = sizey / 2;
	float qsizey = hsizey / 2;
	offset.x = 0; offset.y = 0;


	int ltx = getCenter().x + (offset.x + ((map.x - map.y) * hsizex)) - hsizex;
	int lty = getCenter().y + (offset.y + ((map.x + map.y) * qsizey) - (map.z * hsizey)) - hsizey;


	return Vector2f(ltx, lty);
}

Vector2f dorkestCamera::iTOc_f(Vector2f vTileSize, float scaleFactor, Vector3f map, Vector2f offset)
{
	//The grid is universal. OBEY THE GRID.
	float sizex = vTileSize.x;
	float sizey = vTileSize.y;
	float hsizex = sizex / 2;
	float hsizey = sizey / 2;
	float qsizey = hsizey / 2;
	offset.x = 0; offset.y = 0;

	float scaleCorrectx = 0;// (hsizex * scaleFactor);
	float scaleCorrecty = 0;// (hsizey * scaleFactor) + 6;

	float ltx = getCenter().x + (offset.x + ((map.x - map.y) * hsizex)) - scaleCorrectx;
	float lty = getCenter().y + (offset.y + ((map.x + map.y) * qsizey) - (map.z * hsizey)) - scaleCorrecty;


	return Vector2f(ltx, lty);
}

Vector3f dorkestCamera::cTOi(Vector2f vTileSize, float scaleFactor, Vector2f screen, Vector2f offset)
{
	//Correct for the camera position
	screen.x -= getCenter().x;
	screen.y -= getCenter().y;

	//The grid is universal. OBEY THE GRID.
	//Convert SCREEN coordinates to WORLD coordinates
	float ltx = (screen.x / vTileSize.x) + (screen.y / vTileSize.y);
	float lty = (screen.y / vTileSize.y) - (screen.x / vTileSize.x);

	//Correct for crossing 0
	if ((ltx < 0)) ltx -= 1;
	if ((lty < 0)) lty -= 1;

	//Correct for the centering of the sprite *BLACK MAGIC* I really have no clue why this works. Maybe rounding? *shrug*
	ltx += 1.5f;
	lty += 0.5f;

	//Return our screen Coordinates.
	return Vector3f((int)ltx, (int)lty, 0);
}

Vector3f dorkestCamera::cTOi_f(Vector2f vTileSize, float scaleFactor, Vector2f screen, Vector2f offset)
{
	//Correct for the camera position
	screen.x -= getCenter().x;
	screen.y -= getCenter().y;

	//The grid is universal. OBEY THE GRID.
	//Convert SCREEN coordinates to WORLD coordinates
	float ltx = (screen.x / vTileSize.x) + (screen.y / vTileSize.y);
	float lty = (screen.y / vTileSize.y) - (screen.x / vTileSize.x);

	//Correct for crossing 0
	if ((ltx < 0)) ltx -= 1;
	if ((lty < 0)) lty -= 1;

	//Correct for the centering of the sprite *BLACK MAGIC* I really have no clue why this works. Maybe rounding? *shrug*
	ltx += 1.5f;
	lty += 0.5f;

	//Return our screen Coordinates.
	return Vector3f(ltx, lty, 0);
}

Vector2i dorkestCamera::MapToScreen(Vector3f map) {
	return iTOc({ 32.0f,16.0f }, this->m_scale, map, { 0.0f,0.0f });
}

Vector2d dorkestCamera::MapToScreen_f(Vector3f map) {
	return iTOc_f({ 32.0f,16.0f }, this->m_scale, map, { 0.0f,0.0f });
}

Vector3f dorkestCamera::ScreenToMap(Vector2i screen) {
	return cTOi({ 32.0f,16.0f }, this->m_scale, screen, { 0,0 });
}

bool dorkestCamera::canSee(AABB3f box)
{

	Vector3f righte = Vector3f(box.getMax().x, box.getMin().y,0);
	Vector3f lefte = Vector3f(box.getMin().x, box.getMax().y, 0);
	Vert vl,vr,vt,vb;
	vl.pos = lefte;
	vr.pos = righte;
	vt.pos = box.getMin();
	vb.pos = Vector3f(box.getMax().x, box.getMax().y, 0); vb.col = olc::BLACK;;

	


	Vector2f l = iTOc_f({ 32.0f,16.0f }, this->m_scale, lefte, { 0,0 });
	Vector2f r = iTOc_f({ 32.0f,16.0f }, this->m_scale, righte, { 0,0 });
	Vector2f t = iTOc_f({ 32.0f,16.0f }, this->m_scale, Vector3f(box.getMin().x, box.getMin().y, box.getMax().z), { 0,0 });
	//FLATTEN THE MAX VALUE TO 2D, otherwise iTOc returns the wrong value for our use case.
	Vector2f b = iTOc_f({ 32.0f,16.0f }, this->m_scale, Vector3f(box.getMax().x, box.getMax().y, 0), { 0,0 });

	
	//Test LEFT of our screen against the RIGHT of the box
	if (r.x+32 < 0) return false;
	//Test RIGHT of our screen against the LEFT of the box
	if (l.x-32 > this->m_sWidth) return false;

	//Test TOP of our screen against the BOTTOM of the box.
	if (b.y+64 < 0) return false;
	//Test BOTTOM of our screen against the TOP of the box.
	if ((t.y-64)-16 > this->m_sHeight) return false;


	//if ( b.y < 0) return false;
	//Test BOTTOM of our screen against the TOP of the box
	//if (t.x > this->m_sWidth || t.y > this->m_sHeight) return false;


	return true;
}


void dorkestCamera::setScreenSize(int width, int height)
{
	m_sWidth = width;
	m_sHeight = height;

}

void dorkestCamera::setCenter(float x, float y, float z)
{
	this->m_center.setVector(x, y, z);

}



int dorkestCamera::getScreenWidth()
{
	return m_sWidth;
}

int dorkestCamera::getScreenHeight()
{
	return m_sHeight;
}

Vector3f dorkestCamera::getCenter()
{
	return m_center;
}

