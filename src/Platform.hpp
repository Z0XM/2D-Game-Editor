#pragma once
#include <SFML/Graphics.hpp>
#include <ZUI/Button.hpp>
#include <array>

class Platform : public zui::Button
{
public:

	Platform();

	~Platform();

	void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default) const;

	void load(const sf::Vector2f& startPos, const sf::Vector2f& size, std::array<sf::Texture, 12>& tileTextures);

private:

	std::vector<sf::Sprite> m_imgs;                 /// < set of sprites of tiles of the object 
};
