#include <Render/dorkestCamera.h>
#include <Util/Math/Matrix3.h>



dorkestCamera::dorkestCamera() : sWidth_(640), sHeight_(480), vWidth_(4.0), vHeight_(3.0),
fov_(60), far_(INFINITY), near_(1.0), orthogonal_(true), useFov_(false)
{
	lookat_.setVector(0.0, 0.0, 1.0);
	lookup_.setVector(0.0, 1.0, 0.0);
}


dorkestCamera::dorkestCamera(const dorkestCamera& src)
{
	copyCameraSettings(src);
}


dorkestCamera::~dorkestCamera()
{
}


void dorkestCamera::setScreenSize(int width, int height)
{
	sWidth_ = width;
	sHeight_ = height;
}

void dorkestCamera::setCameraParameters(float xc, float yc, float zc, float xl, float yl, float zl, float xu, float yu, float zu)
{
	center_.setVector(xc, yc, zc);
	lookat_.setVector(xl, yl, zl);
	lookup_.setVector(xu, yu, zu);
}

void dorkestCamera::setCenter(float x, float y, float z)
{
	center_.setVector(x, y, z);
}

void dorkestCamera::setLookat(float x, float y, float z)
{
	lookat_.setVector(x, y, z);
}

void dorkestCamera::setLookUp(float x, float y, float z)
{
	lookup_.setVector(x, y, z);
}

// sets parameters for orthogonal
void dorkestCamera::setOrthogonal(float width, float height, float nearset, float farset)
{
	orthogonal_ = true;
	useFov_ = false;

	vWidth_ = width;
	vHeight_ = height;

	far_ = farset;
	
	near_ = nearset;
}

void dorkestCamera::setOrthoFov(float fov)
{
	orthogonal_ = true;
	useFov_ = true;

	fov_ = fov;
}

// sets parameters for perspective
void dorkestCamera::setPerspective(float fov, float nearset, float farset)
{
	orthogonal_ = false;

	fov_ = fov;
	far_ = farset;
	near_ = nearset;
}

void dorkestCamera::setPerspectiveMode()
{
	orthogonal_ = false;
}

void dorkestCamera::setOrthogonalMode(bool useFov)
{
	orthogonal_ = true;
	useFov_ = useFov;
}

int dorkestCamera::getScreenWidth()
{
	return sWidth_;
}

int dorkestCamera::getScreenHeight()
{
	return sHeight_;
}

float dorkestCamera::getMaxDepth()
{
	return far_;
}

float dorkestCamera::getNearPlane()
{
	return near_;
}

float dorkestCamera::getFov()
{
	return fov_;
}

Vector3f dorkestCamera::getCenter()
{
	return center_;
}

// copy all settings from existing camera
void dorkestCamera::copyCameraSettings(const dorkestCamera& camera)
{
	center_.setVector(camera.center_);
	lookat_.setVector(camera.lookat_);
	lookup_.setVector(camera.lookup_);

	orthogonal_ = camera.orthogonal_;
	useFov_ = camera.useFov_;

	sWidth_ = camera.sWidth_;
	sHeight_ = camera.sHeight_;

	vWidth_ = camera.vWidth_;
	vHeight_ = camera.vHeight_;

	fov_ = camera.fov_;

	near_ = camera.near_;
	far_ = camera.far_;
}

Matrix3f dorkestCamera::getLookatMatrix()
{
	Matrix3f view;

	// camera transformation from world space to view space
	Vector3f forward = lookat_ - center_;
	forward.norm();

	//Cross product.
	Vector3f right = Vector3f(lookup_.cross( forward));
	right.norm();

	Vector3f up = Vector3f(forward.cross(right));

	// build matrix

	//Dot product
	view.buildMatrixFromVectors(right, up, forward);
	view.at(0, 3) = -(right.dot( center_));
	view.at(1, 3) = -(up.dot(center_));
	view.at(2, 3) = -(forward.dot(center_));

	return view;
}




Matrix3f dorkestCamera::getProjectionMatrix()
{
	Matrix3f norm;

	float aspect = static_cast<float>(sWidth_) / static_cast<float>(sHeight_);

	if (orthogonal_)
	{
		float xmax = vWidth_ / 2.0;
		float xmin = -xmax;

		float ymax = vHeight_ / 2.0;
		float ymin = -ymax;

		if (useFov_)
		{
			float t = tan(fov_ / 2.0);

			xmax = far_ * aspect * t;
			xmin = -xmax;

			ymax = far_ * t;
			ymin = -ymax;
		}

		norm.orthogonal(xmin, xmax, ymin, ymax, near_, far_);
	}
	else
	{
		norm.perspective(fov_, aspect, near_, far_);
	}

	return norm;
}


Matrix3f dorkestCamera::getScreenMatrix()
{
	Matrix3f screen;

	screen.screen(sWidth_, sHeight_);

	return screen;
}