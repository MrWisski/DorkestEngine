#pragma once
#include <Util/Math/Vector3.h>
#include <Util/Math/Matrix3.h>
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
	Vector3f center_;
	Vector3f lookat_;
	Vector3f lookup_;

	// is camera in orthogonal mode and shall fov be used
	/// true  = orthogonal
	/// false = perspective
	bool orthogonal_;
	bool useFov_;

	// screen size, also used for aspect calculation
	int sWidth_;
	int sHeight_;

	// viewport width and height for orthogonal if fov_ not used
	float vWidth_;
	float vHeight_;

	// far and near positions
	float near_;
	float far_;

	// field of vision in degree for perspective and maybe for orthogonal
	float fov_;

public:
	dorkestCamera();
	dorkestCamera(const dorkestCamera& src);
	virtual ~dorkestCamera();

	void setScreenSize(int width, int height);
	void setCameraParameters(float xc, float yc, float zc, float xl, float yl, float zl, float xu = 0.0, float yu = 1.0, float zu = 1.0);
	void setCenter(float x, float y, float z);
	void setLookat(float x, float y, float z);
	void setLookUp(float x, float y, float z);

	// sets parameters for orthogonal
	void setOrthogonal(float width, float height, float near, float far);
	void setOrthoFov(float fov);

	// sets parameters for perspective
	void setPerspective(float fov, float near, float far);

	void setPerspectiveMode();
	void setOrthogonalMode(bool useFov = false);

	// copy all settings from existing camera
	void copyCameraSettings(const dorkestCamera& camera);

	int getScreenWidth();
	int getScreenHeight();

	Vector3f getCenter();

	float getMaxDepth();
	float getNearPlane();

	float getFov();


	Matrix3f getLookatMatrix();
	Matrix3f getProjectionMatrix();
	Matrix3f getScreenMatrix();
};