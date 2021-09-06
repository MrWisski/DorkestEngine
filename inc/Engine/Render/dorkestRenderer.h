/*
	Name : Dorkest Renderer
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
#include<vector>

#include "Util/Math/Vector2.h"
#include "Util/Math/Vector3.h"
#include "dorkestCamera.h"
#include "dorkestSprite.h"
#include "Util/Math/Geometry/LineSeg.h"
#include "Util/Math/Geometry/AABB.h"
#include <Util/Color.h>
#include <Util/Math/Geometry/nGons.h>
#include <Util/Math/Geometry/Ray.h>

#include <SFML/Graphics.hpp>

class dorkestSprite;

#define dorkestColor Colorf


/// <summary>
/// An interface class between the DorkestEngine and whatever front end.
/// </summary>
class dorkestRenderer {
public:
	enum textAlign { NONE, LEFT, CENTER, RIGHT };
	enum sType { DIFFUSE = 64, NORMAL = 32, WIRE = 0 };

	/// <summary>
	/// Initializes the Renderer.
	/// </summary>
	/// <param name="useCamera">a sharedptr to the camera to use for all calcs - use setCam to change!</param>
	dorkestRenderer(sf::RenderWindow* win, dorkestCamera* useCamera) : m_camera(useCamera), m_window(win) {};
	void setCam(dorkestCamera* useCamera) { this->m_camera = useCamera; }

	~dorkestRenderer() {};

	void clearWindow();
	void swapBuffer();

	/// <summary>
	/// Draws a sprite
	/// </summary>
	/// <param name="mapPos">Map position of the sprite</param>
	/// <param name="name">Spritename of the sprite to draw. See dorkestSpriteManager</param>
	/// <param name="spriteType"> DIFFUSE (the colored sprite), NORMAL (the normal map of the sprite), WIRE (The wireframe of the sprite.)</param>
	/// <param name="forceColor">If you want to override the base color defined when the sprite was created. This color, if not (0,0,0,0), will be used instead.</param>
	/// <param name="forceScale">If you want to override the base scaling defined when the sprite was created. This scale if not -1, will be used instead.</param>
	/// <param name="forceSize">If you want to override the base size defined when the sprite was created. This size, if not (-1,-1), will be used instead.</param>
	void drawSprite(Vector2i mapPos, std::string name, float forceScale = -1.0f, Vector2f forceSize = Vector2f(-1, -1), Colorf forceColor = Colorf(0, 0, 0, 0) );

	/// <summary>
	/// Draws a line of text to the SCREEN via Row/Text Align
	/// </summary>
	/// <param name="row">The screen is divided into ROWS of Texthight * scale. (0 = top, 1 = </param>
	/// <param name="Str">The string to render</param>
	/// <param name="color">The color to render the string in</param>
	/// <param name="TA">Text alignment of the string : RIGHT, CENTER, LEFT</param>
	/// <param name="scale">Scale to use for the text</param>
	void drawTextRow(sf::Font* font, int row, std::string Str, float scale = 1.0f, Colorf color = Colorf(255, 255, 255, 255), textAlign TA = textAlign::NONE);

	/// <summary>
	/// Draws a line of text on the screen
	/// </summary>
	/// <param name="screenPos">Position to draw string, in pixels</param>
	/// <param name="Str">String to render.</param>
	/// <param name="color">Color to render the string in.</param>
	/// <param name="TA">Text alignment of the string : RIGHT, CENTER, LEFT</param>
	void drawTextPos(sf::Font* font, Vector2f screenPos, std::string Str, float scale = 1.0f, Colorf color = Colorf(255, 255, 255, 255), textAlign TA = textAlign::NONE);

	/// <summary>
	/// Draws a ray
	/// </summary>
	/// <param name="ray"> Ray data to draw</param>
	/// <param name="sCol"> What color to use for the sphere. </param>
	/// <param name="rCol"> What color to use for the line segment.</param>
	void drawRay(Ray<float> ray, Colorf sCol = Colorf(255, 255, 255, 255), Colorf rCol = Colorf(255, 0, 0, 255));

	/// <summary>
	/// Draws a 2d line segment
	/// </summary>
	/// <param name="line"> linesegment to draw.</param>
	/// <param name="col"> What color to use for the line.</param>
	void drawLine2D(LineSeg2Di line, Colorf col = Colorf(255, 255, 255, 255));

	/// <summary>
	/// Draws a 3d line segment
	/// </summary>
	/// <param name="line"> linesegment to draw.</param>
	/// <param name="col"> What color to use for the line.</param>
	void drawLine3D(LineSeg3Df line, Colorf col = Colorf(255, 255, 255, 255));

	/// <summary>
	/// Draws a vertex.
	/// </summary>
	/// <param name="v"> Vertex to draw </param>
	void drawVertex(Vert v);

	void drawTriangle(Triangle t, Colorf col = Colorf(255, 255, 255, 255));

	
	/* Draws a Bounding Box */
	void drawAABB3(AABB3f box, Colorf col = Colorf(255,255,255,255));
	void drawAABB2(AABB2i box, Colorf col = Colorf(255, 255, 255, 255));


	friend class dorkestScene;
protected:
	bool drawToScreen(Vector2i screenPos, std::string name, sType spriteType, Colorf color = Colorf(0,0,0,0), float scale = -1.0f, Vector2f size = Vector2i(-1,-1));
	
private:
	dorkestCamera* m_camera = nullptr;
	sf::RenderWindow* m_window = nullptr;
	
};