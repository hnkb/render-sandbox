#pragma once

#include "../utils/Math.h"


class Camera
{
public:
	// [world: xyz] <--- camera position and properties --> [view space: xyz]
	//   (in view space: object coordinates are relative to camera not origin)
	// [view space: xyz ] <--- projection ---> [clip space: xy -1...+1]
	//   (clip space is what gets sent to the fragment /pixel shader)
	// [clip space: xy -1...+1] <--- linear ---> [screen space: xy 0..dims]
	//   (for rendering, no need to track this, but to convert screen positions, like when
	//    user clicks, it's useful to be able to convert to/from.)

	void setViewport(int2 dims)
	{
		this->pixelSize = float2(2, -2) / (float2)dims;
		aspectRatio = dims.x / (float)dims.y;
	}

	float2 pixelToClip(float2 pixel) const { return pixel * pixelSize + float2(-1, 1); }
	float2 clipToPixel(float2 clip) const { return (clip - float2(-1, 1)) / pixelSize; }

	// TODO: these will be replaced with transformation matrix for projection + view
	// This is basically the same transformation that happens in shader code (and its
	// inverse)
	float2 worldToClip(float2 world) const { return world * scaleWithAR() + view.offset; }
	float2 clipToWorld(float2 clip) const { return (clip - view.offset) / scaleWithAR(); }

	float2 pixelToWorld(float2 pixel) const { return clipToWorld(pixelToClip(pixel)); }
	float2 worldToPixel(float2 world) const { return clipToPixel(worldToClip(world)); }


	// this can be thought of as our projection for now
	float2 scaleWithAR() const { return { view.scale / aspectRatio, view.scale }; }


	// this is the view transformation
	// should be replaced with a transformation matrix
	struct
	{
		float2 offset = { 0, 0 };  // or translation
		float scale = 1.f;
		// no rotation for now
	} view;

	// private:  (made public for unit tests)
	float2 pixelSize;
	float aspectRatio = 1.f;
};
