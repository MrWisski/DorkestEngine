/*
	Name : Dorkest Camera
	Created : 5/26/2021
	Author : MrWisski
	License : (WISSKI-1)

	Copyright 2012 - 2021 Alex Wisnieski AKA MrWisski

	Permission is hereby granted, free of charge, to any person (hereafter "You") obtaining a copy or portion of this software, its resulting
	compilation products, or associated documentation files (hereafter "Software"), to deal in the Software with the following
	restrictions, using the following terminology :

	TERMINOLOGY :
		1) COMMERCIAL : Any use of the Software in which You, or others recieve compensation for any reason, of any kind.

	RESTRICTIONS :
		1) You MAY use, copy, publish, distribute, and/or merge, this Software, with any project or library or software,
		   that is not COMMERCIAL in nature or intent.

		2) You MAY NOT use, copy, publish, distribute, redistribute, sublicense, merge and/or sell copies of any software including partially,
		   containing whole, or any portion of the Software, nor permit persons to whom the Software is furnished to do so for COMMERCIAL
		   uses.

		3) Derivations of the Software are permitted, provided the above copyright notice and this permission notice are NOT
		   modified, with the exception of character encoding changes that do not substantially change the intent of the notice or the
		   above copyright notice, or the readibility or useability of this permission notice or the above copyright notice, except for
		   COMMERCIAL uses.

		4) You MAY NOT imply endorsement of any product that includes or derives from the Software, by any any person or group of people
		   involved in the creation of the Software without direct written permission from said persons or group of people.

		5) You MAY NOT use the legal names, handles, aliases, or nicknames of any person involved in the creation of the Software,
		   to promote any product derived from, or using the Software in any way, without written permission from said persons.

	The above copyright notice and this permission notice shall be included in all copies or portions of the Software.

	Any of the above restrictions which are unlawful or not permitted, whole or in part, in the jurisdiction You live in will not
	affect the rest of the portion of the restriction it exists in, the other restrictions listed in this permission notice.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
	MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
	FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
	WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

	CERTAIN JURISDICTIONS DO NOT ALLOW LIMITATIONS ON IMPLIED WARRANTIES OR THE EXCLUSION OR LIMITATION OF CERTAIN DAMAGES.
	IF YOU RESIDE IN SUCH A JURISDICTION, SOME OR ALL OF THE ABOVE DISCLAIMERS, EXCLUSIONS, OR LIMITATIONS MAY NOT APPLY TO YOU,
	AND YOU MAY HAVE ADDITIONAL RIGHTS. THE LIMITATIONS OR EXCLUSIONS OF WARRANTIES, REMEDIES, OR LIABILITY CONTAINED IN THESE TERMS
	APPLY TO YOU TO THE FULLEST EXTENT SUCH LIMITATIONS OR EXCLUSIONS ARE PERMITTED UNDER THE LAWS OF THE JURISDICTION IN WHICH
	YOU ARE LOCATED.
*/
#pragma once
#include <Util/Math/Geometry/AABB.h>
/*
Camera with settings for 3D viewing
 - orthogonal mode with x, y and z borders
 - perspective mode with fov, aspect, far and near
 - orthogonal mode can also use fov and aspect if wished
*/

class dorkestCamera
{
private:
	// camera parameters

	//Where in the world we're looking at.
	Vector3f m_center;

	//Screen width/height.
	int m_sWidth;
	int m_sHeight;

	Vector3f m_worldMin;
	Vector3f m_worldMax;
	Vector3f m_worldSize;

	float m_scale;

public:

	
	
	dorkestCamera(int w, int h);
	dorkestCamera(const dorkestCamera& src);
	virtual ~dorkestCamera();

	void setZoom(float zFactor) { m_scale = zFactor; }
	float getZoom() { return m_scale; }
	void zoomIn() { m_scale += 0.2f; }
	void zoomOut() { m_scale -= 0.2f; if (m_scale < 0.01f) m_scale = 0.01f; }



	Vector2i iTOc(Vector2f vTileSize, float scaleFactor, Vector3f map, Vector2f offset);
	Vector2f iTOc_f(Vector2f vTileSize, float scaleFactor, Vector3f map, Vector2f offset);

	Vector3f cTOi(Vector2f vTileSize, float scaleFactor, Vector2f screen, Vector2f offset);

	Vector3f cTOi_f(Vector2f vTileSize, float scaleFactor, Vector2f screen, Vector2f offset);

	Vector2i MapToScreen(Vector3f map);
	Vector2d MapToScreen_f(Vector3f map);
	Vector3f ScreenToMap(Vector2i screen);

	bool canSee(AABB3f box);
	
	void setScreenSize(int width, int height);

	void setCenter(float x, float y, float z);
	
	int getScreenWidth();
	int getScreenHeight();

	Vector3f getCenter();

};