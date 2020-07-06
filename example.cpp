#include <SFML/Graphics.hpp>

#include <vector> // for circle shapes
#include <string> // for displaying values as text

#include <iostream>

#include "PXL8R.hpp"

void setCircles(std::vector<sf::CircleShape>& circles, std::vector<sf::CircleShape>& smallCircles, sf::Vector2f size, std::size_t rows, std::size_t columns, bool includeOutlines)
{
	const sf::Vector2f sixteenth{ size / 4.f }; // size of a sixteeth of the object (4x4)
	for (std::size_t cy{ 0u }; cy < rows; ++cy)
	{
		for (std::size_t cx{ 0u }; cx < columns; ++cx)
		{
			// main circles
			const float radius{ (sixteenth.x < sixteenth.y) ? sixteenth.x / 2.f : sixteenth.y / 2.f }; // make sure circle's size stays within "sixteenth" regardless of aspect ratio
			sf::CircleShape& circle{ circles[cy * columns + cx] };
			circle.setRadius(radius);
			circle.setOrigin({ radius, radius });
			circle.setPosition({ (cx * sixteenth.x) + sixteenth.x / 2.f, (cy * sixteenth.y) + sixteenth.y / 2.f });
			if (includeOutlines)
				circle.setOutlineThickness(-4.f);
			else
				circle.setOutlineThickness(0.f);

			// small circles
			if ((cy > 0u) && (cx > 0u))
			{
				const float smallRadius{ radius * 0.7f };
				sf::CircleShape& smallCircle{ smallCircles[(cy - 1u) * (columns - 1u) + (cx - 1u)] };
				smallCircle.setRadius(smallRadius);
				smallCircle.setOrigin({ smallRadius, smallRadius });
				smallCircle.setPosition({ cx * sixteenth.x, cy * sixteenth.y });
				if (includeOutlines)
					smallCircle.setOutlineThickness(2.f);
				else
					smallCircle.setOutlineThickness(0.f);
			}
		}
	}
}

void randomizeCircleColors(std::vector<sf::CircleShape>& circles, std::vector<sf::CircleShape>& smallCircles)
{
	for (auto& circle : circles)
	{
		circle.setFillColor(sf::Color(rand() % 256u, rand() % 256u, rand() % 256u));
		circle.setOutlineColor(sf::Color(rand() % 256u, rand() % 256u, rand() % 256u));
	}
	for (auto& smallCircle : smallCircles)
	{
		smallCircle.setFillColor(sf::Color(rand() % 256u, rand() % 256u, rand() % 256u));
		smallCircle.setOutlineColor(sf::Color(rand() % 256u, rand() % 256u, rand() % 256u));
	}
}

int main()
{
	// randomness in this example is extremely trivial so rand is used
	srand(static_cast<unsigned int>(time(0)));

	// initial size
	const sf::Vector2u initWindowSize({ 800u, 600u });

	// show text switch
	bool showText{ true };

	// circles outlines switch
	bool includeCircleOutlines{ true };

	// current pixelation
	sf::Vector2f currentPixelSize({ 1.f, 1.f });
	sf::Vector2f previousPixelSize{ currentPixelSize };

	// current size
	sf::Vector2f currentSize(initWindowSize);

	// display switch
	bool displayImageInsteadOfCircles{ false };

	// pixel lock switch
	bool isPixelLock{ true };

	// rotation
	constexpr int maximumRotationControl{ 1000 };
	constexpr float baseRotationSpeed{ 1.f };
	int rotationControl{ 0 };

	// feedback text
	sf::Font font;
	if (!font.loadFromFile("arial.ttf"))
		return EXIT_FAILURE;
	sf::Text text;
	text.setFont(font);
	text.setCharacterSize(24u);
	text.setPosition({ 20.f, 20.f });
	text.setFillColor(sf::Color::White);
	text.setOutlineColor(sf::Color::Black);
	text.setOutlineThickness(1.f);

	// image sprite
	sf::Texture texture;
	if (!texture.loadFromFile("image.jpg"))
		return EXIT_FAILURE;
	sf::Sprite sprite(texture);
	
	// circles
	constexpr std::size_t rowsOfCircles{ 4u };
	constexpr std::size_t columnsOfCircles{ 4u };
	std::vector<sf::CircleShape> circles(rowsOfCircles * columnsOfCircles), smallCircles((rowsOfCircles - 1u) * (columnsOfCircles - 1u));
	setCircles(circles, smallCircles, currentSize, rowsOfCircles, columnsOfCircles, includeCircleOutlines);
	randomizeCircleColors(circles, smallCircles);

	// window
	sf::ContextSettings contextSettings;
	contextSettings.antialiasingLevel = 16u;
	sf::RenderWindow window(sf::VideoMode(initWindowSize.x, initWindowSize.y), "PXL8R Example", sf::Style::Default, contextSettings);

	// PXL8R
	PXL8R pxl8r(currentPixelSize);

	// animation clock
	sf::Clock clock;

	// main loop
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if ((event.type == sf::Event::Closed) || ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Escape)))
				window.close();
			else if (event.type == sf::Event::Resized)
				window.setView(sf::View(sf::FloatRect(0.f, 0.f, static_cast<float>(event.size.width), static_cast<float>(event.size.height)))); // resize window view to match window size
			else if (event.type == sf::Event::KeyPressed)
			{
				switch (event.key.code)
				{
					// F1 = toggle text
				case sf::Keyboard::F1:
					showText = !showText;
					break;
					/*
					// BACKSPACE = reset pixelation (reset pixel size to 1x1)
				case sf::Keyboard::BackSpace:
					currentPixelSize = { 1.f, 1.f };
					break;
					*/
					// 1/2 = adjust pixelation (pixel size)
				case sf::Keyboard::Num1:
					if (event.key.alt && event.key.shift)
						currentPixelSize = { 1.f, 1.f };
					else if (event.key.alt)
						currentPixelSize -= { 1.f, 1.f };
					else if (event.key.shift)
						currentPixelSize /= 2.f;
					else
						currentPixelSize /= 1.1f;
					break;
				case sf::Keyboard::Num2:
					if (event.key.alt && event.key.shift)
						currentPixelSize = { 1.f, 1.f };
					else if (event.key.alt)
						currentPixelSize += { 1.f, 1.f };
					else if (event.key.shift)
						currentPixelSize *= 2.f;
					else
						currentPixelSize *= 1.1f;
					break;
					// 3/4 = adjust pixelation (pixel size) (width only)
				case sf::Keyboard::Num3:
					if (event.key.alt && event.key.shift)
						currentPixelSize.x = 1.f;
					else if (event.key.alt)
						currentPixelSize.x -= 1.f;
					else if (event.key.shift)
						currentPixelSize.x /= 2.f;
					else
						currentPixelSize.x /= 1.1f;
					break;
				case sf::Keyboard::Num4:
					if (event.key.alt && event.key.shift)
						currentPixelSize.x = 1.f;
					else if (event.key.alt)
						currentPixelSize.x += 1.f;
					else if (event.key.shift)
						currentPixelSize.x *= 2.f;
					else
						currentPixelSize.x *= 1.1f;
					break;
					// 5/6 = adjust pixelation (pixel size) (height only)
				case sf::Keyboard::Num5:
					if (event.key.alt && event.key.shift)
						currentPixelSize.y = 1.f;
					else if (event.key.alt)
						currentPixelSize.y -= 1.f;
					else if (event.key.shift)
						currentPixelSize.y /= 2.f;
					else
						currentPixelSize.y /= 1.1f;
					break;
				case sf::Keyboard::Num6:
					if (event.key.alt && event.key.shift)
						currentPixelSize.y = 1.f;
					else if (event.key.alt)
						currentPixelSize.y += 1.f;
					else if (event.key.shift)
						currentPixelSize.y *= 2.f;
					else
						currentPixelSize.y *= 1.1f;
					break;
					// 7 = set colour to red (alt to set to inverse: cyan)
				case sf::Keyboard::Num7:
					pxl8r.setColor(event.key.alt ? sf::Color::Cyan : sf::Color::Red);
					break;
					// 8 = set colour to green (alt to set to inverse: magenta)
				case sf::Keyboard::Num8:
					pxl8r.setColor(event.key.alt ? sf::Color::Magenta : sf::Color::Green);
					break;
					// 9 = set colour to blue (alt to set to inverse: yellow)
				case sf::Keyboard::Num9:
					pxl8r.setColor(event.key.alt ? sf::Color::Yellow : sf::Color::Blue);
					break;
					// 0 = set colour to white (alt to set to half-transparent white)
				case sf::Keyboard::Num0:
					pxl8r.setColor(event.key.alt ? sf::Color(255u, 255u, 255u, 128u) : sf::Color::White);
					break;
					// Q = quantize pixel size (round to nearest integer)
				case sf::Keyboard::Q:
					currentPixelSize.x = std::round(currentPixelSize.x);
					currentPixelSize.y = std::round(currentPixelSize.y);
					break;
					// -/= = adjust size
				case sf::Keyboard::Dash:
					if (event.key.alt && event.key.shift)
						currentSize = window.getView().getSize();
					else if (event.key.alt)
						currentSize -= { 1.f, 1.f };
					else if (event.key.shift)
						currentSize /= 2.f;
					else
						currentSize /= 1.1f;
					if (!event.key.control)
						setCircles(circles, smallCircles, currentSize, rowsOfCircles, columnsOfCircles, includeCircleOutlines);
					break;
				case sf::Keyboard::Equal:
					if (event.key.alt && event.key.shift)
						currentSize = window.getView().getSize();
					else if (event.key.alt)
						currentSize += { 1.f, 1.f };
					else if (event.key.shift)
						currentSize *= 2.f;
					else
						currentSize *= 1.1f;
					if (!event.key.control)
						setCircles(circles, smallCircles, currentSize, rowsOfCircles, columnsOfCircles, includeCircleOutlines);
					break;
					// ,/. = set direction of rotation
				case sf::Keyboard::Comma:
					if (event.key.alt)
					{
						if (rotationControl > 0)
							rotationControl = -rotationControl; // flip direction if new direction is opposite to current direction
						else
							rotationControl = 0; // stop rotation if same as current direction
					}
					else
					{
						if (event.key.shift)
							rotationControl -= 10; // SHIFT decreases rotation control by 10 instead of 1
						else
							--rotationControl;
						if (rotationControl < -maximumRotationControl)
							rotationControl = -maximumRotationControl;
					}
					break;
				case sf::Keyboard::Period:
					if (event.key.alt)
					{
						if (rotationControl < 0)
							rotationControl = -rotationControl; // flip direction if new direction is opposite to current direction
						else
							rotationControl = 0; // stop rotation if same as current direction
					}
					else
					{
						if (event.key.shift)
							rotationControl += 10; // SHIFT increases rotation control by 10 instead of 1
						else
							++rotationControl;
						if (rotationControl > maximumRotationControl)
							rotationControl = maximumRotationControl;
					}
					break;
					// / = reset rotation
				case sf::Keyboard::Slash:
					pxl8r.setRotation(0.f);
					if (event.key.alt)
						rotationControl = 0;
					break;
					// SPACE = switch between image and circles displayed
				case sf::Keyboard::Space:
					displayImageInsteadOfCircles = !displayImageInsteadOfCircles;
					break;
					// TAB = toggle pixel lock
				case sf::Keyboard::Tab:
					isPixelLock = !isPixelLock;
					break;
					// ENTER = new random circle colours
				case sf::Keyboard::Enter:
					if (event.key.alt)
					{
						includeCircleOutlines = !includeCircleOutlines;
						setCircles(circles, smallCircles, currentSize, rowsOfCircles, columnsOfCircles, includeCircleOutlines);
					}
					else if (event.key.control)
						setCircles(circles, smallCircles, currentSize, rowsOfCircles, columnsOfCircles, includeCircleOutlines);
					else
						randomizeCircleColors(circles, smallCircles);
					break;
				}
			}
		}

		if (currentPixelSize.x < 1.f)
			currentPixelSize.x = 1.f;
		if (currentPixelSize.y < 1.f)
			currentPixelSize.y = 1.f;

		const bool pixelSizeHasChanged{ previousPixelSize != currentPixelSize };
		previousPixelSize = currentPixelSize;

		sprite.setScale({ currentSize.x / texture.getSize().x, currentSize.y / texture.getSize().y }); // scale image to match view

		// PXL8R setup
		if (pixelSizeHasChanged)
			pxl8r.setPixelSize(currentPixelSize); // setting the pixel size is the heaviest operation and can be avoided if it does not change
		pxl8r.setSize(currentSize, isPixelLock); // the actual size of the object. automatically resets the view (unless stopped using the third parameter).
		pxl8r.setOrigin(currentSize / 2.f);
		pxl8r.setPosition(window.getView().getCenter());

		// PXL8R rotation
		pxl8r.rotate(clock.restart().asSeconds() * baseRotationSpeed * rotationControl);

		// PXL8R clear
		pxl8r.clear(sf::Color::Blue);

		// PXL8R draw
		if (displayImageInsteadOfCircles)
			pxl8r.draw(sprite);
		else
		{
			for (auto& smallCircle : smallCircles)
				pxl8r.draw(smallCircle);
			for (auto& circle : circles)
				pxl8r.draw(circle);
		}

		// update text
		std::string textString{ "Pixel width: " + std::to_string(pxl8r.getPixelSize().x) + "\nPixel height: " + std::to_string(pxl8r.getPixelSize().y) };
		textString += "\nPixel Lock: ";
		textString += (isPixelLock ? "enabled" : "disabled");
		textString += "\nSize: " + std::to_string(pxl8r.getSize().x) + "x" + std::to_string(pxl8r.getSize().y);
		textString += "\nColor: (" + std::to_string(pxl8r.getColor().r) + "," + std::to_string(pxl8r.getColor().g) + "," + std::to_string(pxl8r.getColor().b);
		if (pxl8r.getColor().a < 255u)
			textString += "," + std::to_string(pxl8r.getColor().a);
		textString += ")\nRotation Control: " + std::to_string(rotationControl);
		text.setString(textString);

		// window update
		window.clear();
		window.draw(pxl8r); // draw PXL8R's render
		if (showText)
			window.draw(text); // draw text over PXL8R's render using window's own view
		window.display();
	}
}
