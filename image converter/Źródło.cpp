#include <iostream>
#include <iomanip>
#include <SFML\Graphics.hpp>
#include "AI.h"
#include "ScreenCapturer.h"

int main()
{
	const int squareSize = 8, squareCount = 8;
	const int imageSize = squareSize * squareCount;
	sf::RenderWindow window(sf::VideoMode(1024, 512), "image converter");
	sf::Texture displayTexture, generatedTexture;
	displayTexture.create(imageSize, imageSize);
	generatedTexture.create(imageSize, imageSize);
	displayTexture.setSmooth(true);
	generatedTexture.setSmooth(true);
	sf::Sprite displaySprite(displayTexture), generatedSprite(generatedTexture);
	displaySprite.setScale(512 / imageSize, 512 / imageSize);
	generatedSprite.setScale(512 / imageSize, 512 / imageSize);
	generatedSprite.setPosition(512, 0);
	sf::Image displayImage, generatedImage;
	displayImage.create(imageSize, imageSize);
	generatedImage.create(imageSize, imageSize);
	ScreenCapturer capturer(imageSize, imageSize);
	NeuralNetwork network({ 3 * squareSize * squareSize, 8, 3 * squareSize * squareSize });
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			switch (event.type)
			{
			case sf::Event::Closed:
				window.close();
				break;
			}
		}
		capturer.capture();
		RGBQUAD * displayData = capturer.getData();
		float input[squareSize][squareSize][3];
		for (int yy = 0; yy < squareCount; yy++)
		{
			for (int xx = 0; xx < squareCount; xx++)
			{
				for (int y = 0; y < squareSize; y++)
				{
					for (int x = 0; x < squareSize; x++)
					{
						float * inputColor = &input[y][x][0];
						int absx = x + xx * squareSize, absy = y + yy * squareSize;
						RGBQUAD& pixel = displayData[absx + absy * imageSize];
						displayImage.setPixel(absx, absy, sf::Color(pixel.rgbRed, pixel.rgbGreen, pixel.rgbBlue));
						inputColor[0] = pixel.rgbRed / 255.0f;
						inputColor[1] = pixel.rgbGreen / 255.0f;
						inputColor[2] = pixel.rgbBlue / 255.0f;
					}
				}
				network.copyInput(&input[0][0][0]);
				network.calculateGradient(&input[0][0][0]);
				network.adjustNetwork(0.01f);
				network.getOutput(&input[0][0][0]);
				for (int y = 0; y < squareSize; y++)
				{
					for (int x = 0; x < squareSize; x++)
					{
						float * inputColor = &input[y][x][0];
						generatedImage.setPixel(x + xx * squareSize, y + yy * squareSize, sf::Color(inputColor[0] * 255, inputColor[1] * 255, inputColor[2] * 255));
					}
				}
			}
		}
		displayTexture.update(displayImage);
		generatedTexture.update(generatedImage);
		window.draw(displaySprite);
		window.draw(generatedSprite);
		window.display();
	}
}