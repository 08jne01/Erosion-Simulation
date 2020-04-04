#pragma once
struct Config

{
	Config(): width(300), height(300), isFullscreen(0), renderDistance(16), fieldOfView(90)

	{

	}

	Config(int width_, int height_, int isFullscreen_, int renderDistance_, int fieldOfView_):
		width(width_), height(height_), isFullscreen(isFullscreen_), renderDistance(renderDistance_), fieldOfView(fieldOfView_)

	{

	}

	int width, height, isFullscreen, renderDistance, fieldOfView;
};