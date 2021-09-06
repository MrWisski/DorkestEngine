#include <Engine/Render/dorkestCamera.h>
#include <Util/Math/Matrix3.h>
#include <Util/Math/Geometry/nGons.h>


dorkestCamera::dorkestCamera(int w, int h, int x, int y) : m_scale(1.0f)
{

	m_view.setSize(w, h);
	m_viewSize.width = w;
	m_viewSize.height = h;
	m_viewSize.top = y;
	m_viewSize.left = x;
	setCenter(x + (w / 2), y + (h / 2));
	m_view.setCenter(x + (w / 2), y + (h / 2));
	m_view.zoom(m_scale);

}


dorkestCamera::~dorkestCamera()
{
}


/*
Vector2i dorkestCamera::iTOc(Vector2f vTileSize, float scaleFactor, Vector3f map, Vector2f offset)
{
	//The grid is universal. OBEY THE GRID.
	float sizex = vTileSize.x * (scaleFactor * this->m_scale);
	float sizey = vTileSize.y * (scaleFactor * this->m_scale);
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
	float sizex = vTileSize.x * (scaleFactor * this->m_scale);
	float sizey = vTileSize.y * (scaleFactor * this->m_scale);
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

	//correct for scaling
	float sizex = vTileSize.x * (scaleFactor * this->m_scale);
	float sizey = vTileSize.y * (scaleFactor * this->m_scale);

	//The grid is universal. OBEY THE GRID.
	//Convert SCREEN coordinates to WORLD coordinates
	float ltx = (screen.x / sizex) + (screen.y / sizey);
	float lty = (screen.y / sizey) - (screen.x / sizex);

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
	//correct for scaling
	float sizex = vTileSize.x * (scaleFactor * this->m_scale);
	float sizey = vTileSize.y * (scaleFactor * this->m_scale);

	//The grid is universal. OBEY THE GRID.
	//Convert SCREEN coordinates to WORLD coordinates
	float ltx = (screen.x / sizex) + (screen.y / sizey);
	float lty = (screen.y / sizey) - (screen.x / sizex);

	//Correct for crossing 0
	if ((ltx < 0)) ltx -= 1;
	if ((lty < 0)) lty -= 1;

	//Correct for the centering of the sprite *BLACK MAGIC* I really have no clue why this works. Maybe rounding? *shrug*
	ltx += 1.5f;
	lty += 0.5f;

	//Return our screen Coordinates.
	return Vector3f(ltx, lty, 0);
}
*/

Vector2i dorkestCamera::WorldToScreen(Vector3f map) {

	Vector2f offset = m_center - (Vector2f(m_viewSize.width, m_viewSize.height) / 2);
	return Vector2i(map.x, map.y) - offset;

}

Vector2f dorkestCamera::ScreenToWorld(Vector2i screen) {
	return Vector2f(0, 0);
}

bool dorkestCamera::canSee(AABB3f box)
{
	return true;
}


void dorkestCamera::setScreenSize(int width, int height)
{
	this->m_viewSize.height = height;
	m_viewSize.width = width;
	m_view.setSize(width, height);

}

Vector2i dorkestCamera::getScreenSize() {
	return Vector2i(m_viewSize.width, m_viewSize.height);

}

void dorkestCamera::setCenter(float x, float y)
{

	this->m_center = Vector2f(x, y);
	this->m_view.setCenter(x, y);
	this->m_viewSize.left = x - (m_viewSize.width / 2);
	this->m_viewSize.top = y - (m_viewSize.height / 2);


}

Vector2f dorkestCamera::getCenter()
{
	return m_center;
}

void dorkestCamera::render(sf::RenderWindow* window)
{
	this->m_window = window;

	window->setView(m_view);
}

bool dorkestCamera::needsUpdate(Vector2i subject) {
	Vector2i offset = (this->m_center - subject);
	if (offset.x > 0.01f || offset.y > 0.01f) {
		return true;
	}

	return false;
}

void dorkestCamera::update(Vector2i subject, float dTime)
{
	
	//Convert the world coords of the subject to screen pixel coords.
	//sf::Vector2i wc = m_window->mapCoordsToPixel(sf::Vector2f(subjectW.x, subjectW.y));
	//Vector2i subject = Vector2i(wc.x,wc.y);
	
	int leftv = subject.x - (this->m_view.getSize().x /2);
	int topv = subject.y - (this->m_view.getSize().y / 2);
	int rightv = subject.x + (this->m_view.getSize().x /2);
	int botv = subject.y + (this->m_view.getSize().y / 2);

	int rightm = this->m_mapSize.left + this->m_mapSize.width;
	int botm = this->m_mapSize.top + this->m_mapSize.height;

	//debug("topv = " + std::to_string(topv) + " vleft = " + std::to_string(leftv) + " vbottom = " + std::to_string(botv) + " vright = " + std::to_string(rightv));
	//debug("mtop = " + std::to_string(m_mapSize.top) + " mleft = " + std::to_string(m_mapSize.left) + " mbottom = " + std::to_string(botm) + " mright = " + std::to_string(rightm));
	
	
	int nx = subject.x, ny = subject.y;

	//First - Do we even need to update the camera?
	//if (!needsUpdate(subject)) return;

	//Second - Focus on the subject.
	//this->setCenter(subject.x, subject.y);

	//debug("vtop = " + std::to_string(m_viewSize.top) + " vleft = " + std::to_string(m_viewSize.left) + " vbottom = " + std::to_string(botv) + " vright = " + std::to_string(rightv));

	

	if ( leftv <= this->m_mapSize.left) {//Third - Have we hit the WORLD MINIMUM LEFT BORDER?
		//We have - set the view center to the minimum map value + view halfsize.
		nx = this->m_mapSize.left + (this->m_viewSize.width / 2);
		//debug("left");
	} 
	if (rightv >= rightm) { //Fourth - Have we hit the WORLD MAXIMUM RIGHT BORDER?
			//We have - set the view center to the maximum map value - view halfsize.
		nx = rightm - (this->m_viewSize.width / 2);
		//debug("right");
	}


	if (topv <= this->m_mapSize.top) {//Fifth - Have we hit the WORLD MINIMUM TOP BORDER?
		//We have - set the view center to the minimum map value + view halfsize.
		ny = this->m_mapSize.top + (this->m_viewSize.height / 2);

		//debug("top");

	} 
	if (botv >= botm) {//Sixth - Have we hit the WORLD MAXIMUM RIGHT BORDER?
		 //We have - set the view center to the maximum map value - view halfsize.
		ny = botm - (this->m_viewSize.height / 2);
		//debug("bottom");
	}

	//if (nx != subject.x) debug("NX = " + std::to_string(nx));
	//if (ny != subject.y) debug("NY = " + std::to_string(ny));

	this->setCenter(nx, ny);

	

	/*Vector2f offset = this->m_center - subject;
	Vector2i vp = Vector2i(m_view.getCenter().x, m_view.getCenter().y) - Vector2i(m_view.getSize().x / 2, m_view.getSize().y / 2);
	debug("View pos = " + vp.toStr());
	debug("subject = " + subject.toStr());
	debug("Center = " + m_center.toStr());
	debug("Offset = " + offset.toStr());
	Vector2f move;
	move = offset / (dTime);
	debug("Move = " + move.toStr());

	m_view.move(move.x, move.y);
	sf::Vector2f nc = m_view.getCenter();*/

}

void dorkestCamera::setIsoView() {
	this->isIso = true;
}
void dorkestCamera::setTopDownView() {
	this->isIso = false;
}

void dorkestCamera::setMapSize(int w, int h) {
	this->m_mapSize.height = h;
	this->m_mapSize.width = w;
}
Vector2i dorkestCamera::getMapSize() {
	return Vector2i(m_mapSize.width, m_mapSize.height);
}

void dorkestCamera::setMapCorner(int top, int left) {
	m_mapSize.top = top;
	m_mapSize.left = left;
}
Vector2i dorkestCamera::getMapCorner() {
	return Vector2i(m_mapSize.left, m_mapSize.top);
}

Vector2i dorkestCamera::getScreenTL() {
	return Vector2i(m_viewSize.left, m_viewSize.top);
}

