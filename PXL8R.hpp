//////////////////////////////////////////////////////////////////////////////
//
// PXL8R (https://github.com/Hapaxia/Apris)
// --
//
// Copyright(c) 2020 M.J.Silk
//
// This software is provided 'as-is', without any express or implied
// warranty. In no event will the authors be held liable for any damages
// arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions :
//
// 1. The origin of this software must not be misrepresented; you must not
// claim that you wrote the original software.If you use this software
// in a product, an acknowledgment in the product documentation would be
// appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such, and must not be
// misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.
//
// M.J.Silk
// MJSilk2@gmail.com
//
//////////////////////////////////////////////////////////////////////////////

/*

PXL8R

by Hapaxia
July 2020
based on PXL8 (July 2014)
> github.com/Hapaxia
> twitter.com/Hapaxiation

Creates a pixelated render target.

*/

#ifndef PXL8R_HPP
#define PXL8R_HPP

#include <vector>
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/Vertex.hpp>
#include <SFML/System/Vector2.hpp>

// PXL8R v1.0.0
class PXL8R : public sf::Drawable, public sf::Transformable
{
public:
	PXL8R(sf::Vector2f pixelSize = { 1.f, 1.f });
	PXL8R(sf::Vector2f pixelSize, sf::Vector2f size);

	void clear();
	void clear(sf::Color colour);
	void draw(const sf::Drawable& drawable, const sf::RenderStates& states = sf::RenderStates());
	void apply();

	void resetView();
	void setView(sf::View view);
	void setViewRect(sf::FloatRect viewRect);
	void setPixelLock(bool isPixelLock);
	void setColor(sf::Color color);
	void setClearColor(sf::Color color);
	void setSize(sf::Vector2f size);
	void setSize(sf::Vector2f size, bool lockTextureSize, bool viewReset = true);
	void setPixelSize(sf::Vector2f size);
	void setPixelSize(float size);
	sf::Color getColor() const;
	sf::Color getClearColor() const;
	sf::Vector2f getSize() const;
	sf::Vector2f getPixelSize() const;










	////////////////////
	// IMPLEMENTATION //
	////////////////////

private:
	const sf::PrimitiveType m_primitiveType;
	bool m_isPixelLock;
	mutable bool m_needsApplying;
	mutable sf::RenderTexture m_texture;
	sf::View m_view;
	sf::Color m_color;
	sf::Vector2f m_size;
	sf::Vector2f m_pixelSize;
	sf::Color m_clearColor;
	std::vector<sf::Vertex> m_vertices;

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	void priv_reformPixelation();
};

inline sf::Color PXL8R::getColor() const
{
	return m_color;
}

inline sf::Color PXL8R::getClearColor() const
{
	return m_clearColor;
}

inline sf::Vector2f PXL8R::getSize() const
{
	return m_size;
}

inline sf::Vector2f PXL8R::getPixelSize() const
{
	return m_pixelSize;
}

inline PXL8R::PXL8R(sf::Vector2f pixelSize)
	: m_primitiveType{ sf::PrimitiveType::TriangleStrip }
	, m_isPixelLock{ true }
	, m_needsApplying{ true }
	, m_texture()
	, m_view()
	, m_size{ 800.f, 600.f }
	, m_pixelSize{ 1.f, 1.f }
	, m_clearColor{ sf::Color::Black }
	, m_vertices(4u)
{
	setPixelSize(pixelSize);
}

inline PXL8R::PXL8R(sf::Vector2f pixelSize, sf::Vector2f size)
	: PXL8R(pixelSize)
{
	setSize(size);
}

inline void PXL8R::setView(sf::View view)
{
	m_view.reset({ 0, 0, view.getSize().x, view.getSize().y });
	m_texture.setView(view);
}

inline void PXL8R::setViewRect(sf::FloatRect viewRect)
{
	m_view.reset(viewRect);
	m_texture.setView(m_view);
}

inline void PXL8R::clear(sf::Color color)
{
	m_needsApplying = true;
	m_texture.clear(color);
}

inline void PXL8R::clear()
{
	clear(m_clearColor);
}

inline void PXL8R::resetView()
{
	setViewRect({ 0.f, 0.f, static_cast<float>(m_size.x), static_cast<float>(m_size.y) });
}

inline void PXL8R::setPixelLock(const bool isPixelLock)
{
	m_isPixelLock = isPixelLock;
}

inline void PXL8R::draw(const sf::Drawable& drawable, const sf::RenderStates& states)
{
	m_needsApplying = true;
	m_texture.draw(drawable, states);
}

inline void PXL8R::apply()
{
	m_texture.display();
	m_needsApplying = false;
}

inline void PXL8R::setColor(const sf::Color color)
{
	m_color = color;
	for (auto& vertex : m_vertices)
		vertex.color = m_color;
}

inline void PXL8R::setSize(const sf::Vector2f size, const bool isPixelLock, const bool viewReset)
{
	setPixelLock(isPixelLock);
	setSize(size);
	if (viewReset)
		resetView();
}

inline void PXL8R::setSize(const sf::Vector2f size)
{
	m_size = size;
	m_vertices[0u].position = { 0.f, 0.f };
	m_vertices[1u].position = { m_size.x, 0.f };
	m_vertices[2u].position = { 0.f, m_size.y };
	m_vertices[3u].position = m_size;
	const sf::Vector2f textureSize{ m_isPixelLock ? sf::Vector2f(m_texture.getSize()) : sf::Vector2f{ m_size.x / m_pixelSize.x, m_size.y / m_pixelSize.y } };
	m_vertices[0u].texCoords = { 0.f, 0.f };
	m_vertices[1u].texCoords = { textureSize.x, 0.f };
	m_vertices[2u].texCoords = { 0.f, textureSize.y };
	m_vertices[3u].texCoords = textureSize;
}

inline void PXL8R::setClearColor(sf::Color colour)
{
	m_clearColor = colour;
}

inline void PXL8R::setPixelSize(sf::Vector2f ratio)
{
	constexpr float epsilon{ 0.000001f };
	if (ratio.x < epsilon)
		ratio.x = epsilon;
	if (ratio.y < epsilon)
		ratio.y = epsilon;
	m_pixelSize = ratio;
	m_texture.create(static_cast<unsigned int>((static_cast<float>(m_size.x) / m_pixelSize.x) + 1.f), static_cast<unsigned int>((static_cast<float>(m_size.y) / m_pixelSize.y) + 1.f));
	m_texture.setView(m_view);
	m_texture.clear(m_clearColor);
}

inline void PXL8R::setPixelSize(float ratio)
{
	setPixelSize({ ratio, ratio });
}

inline void PXL8R::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.texture = &m_texture.getTexture();
	states.transform = getTransform();
	if (m_needsApplying)
		m_texture.display();
	m_needsApplying = false;
	target.draw(m_vertices.data(), m_vertices.size(), m_primitiveType, states);
}

#endif // PXL8R_HPP
