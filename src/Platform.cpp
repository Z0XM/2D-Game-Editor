#include "Platform.hpp"
#include <SFML/Graphics.hpp>

template <typename U, typename V>
sf::Vector2f operator*(const sf::Vector2<U>& a, const sf::Vector2<V>& b)
{
	return sf::Vector2f(a.x * b.x, a.y * b.y);
}

template <typename U, typename V>
sf::Vector2f operator/(const sf::Vector2<U>& a, const sf::Vector2<V>& b)
{
	return sf::Vector2f(a.x / b.x, a.y / b.y);
}

template <typename U>
sf::Vector2f unitVec(const sf::Vector2<U>& vec)
{
	sf::Vector2f unit(0, 0);
	if (vec.x != 0) unit.x = vec.x / std::abs(vec.x);
	if (vec.y != 0) unit.y = vec.y / std::abs(vec.y);
	return unit;
}


Platform::Platform()
{
	
}

Platform::~Platform()
{

}

void Platform::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (isActive()) {
		states.transform *= getTransform();
		for (int i = 0; i < m_imgs.size(); i++) {
			target.draw(m_imgs[i], states);
		}
	}
}

void Platform::load(const sf::Vector2f& startPos, const sf::Vector2f& size, std::array<sf::Texture, 12>& tileTextures)
{
	m_imgs.clear();

	setPointCount(4);
	setPoint(0, sf::Vector2f(0,0));
	setPoint(1, sf::Vector2f(size.x, 0));
	setPoint(2, sf::Vector2f(size.x, size.y));
	setPoint(3, sf::Vector2f(0, size.y));

	setOrigin(size * 0.5f);

	setPosition(startPos + size * 0.5f);

	sf::Vector2f tileSize(32, 32);
	sf::Vector2i tileCount(size.x / 32, size.y / 32);

	// emplace back a single tile into m_imgs
	// platTilePos -> position of tile wrt platform tiles 
	auto loadTileSingle = [this, tileSize](const sf::Vector2i& platTilePos, const sf::Texture& texture) {
		m_imgs.emplace_back(texture);
		// size of whole image -> textureSize
		// size of image on screen -> tileSize
		// mapping image size into tileSize
		// image size(pixels) -> tileSize
		// 1(pixels) -> tileSize / image size									<<< scaleFactor
		// final size of image(pixels) -> current size of image * scaleFactor	<<< result
		m_imgs.back().setScale(tileSize / texture.getSize());// scaleFactor

		// final image position in pixels
		m_imgs.back().setPosition(platTilePos * tileSize);
	};

	// emplace back multiple tiles using a single texture and single sprite into m_imgs
	// platTilePos -> position of first tile wrt platform tiles
	// tileCount -> total number of tiles across and below in region of collective tiles
	auto loadTileMultiple = [this, &tileSize](const sf::Vector2i& platTilePos, const sf::Vector2i& tileCount, const sf::Texture& texture) {
		m_imgs.emplace_back(texture);

		// when rect size is greater than texture size, texture repeats if setRepeat is enabled
		// size of texture used in sprite -> tileCount * texture size
		m_imgs.back().setTextureRect({ 0, 0, tileCount.x * (int)texture.getSize().x, tileCount.y * (int)texture.getSize().y });

		// size of whole image -> tilecount * textureSize
		// size of image on screen -> tileCount * tileSize
		// for mapping see above in loadTileSingle
		m_imgs.back().setScale(tileSize / texture.getSize());

		// final image position in pixels
		m_imgs.back().setPosition(platTilePos * tileSize);
	};

	// load a two single tiles using loadTileSingle with first & last texture
	// load a multiple tiles using loadTileMultiple with middle texture
	auto loadTileLine = [&loadTileSingle, &loadTileMultiple, this, &tileSize](
		const sf::Vector3<sf::Vector2i> platTilePositions,		// positions of each tile(first, mid, last) wrt platform tiles
		const sf::Vector2i& midTileCount,						// number of tiles across and below in middle tile region
		const sf::Texture& first,								// texture used for first tile
		const sf::Texture& mid,									// texture used for middle tiles
		const sf::Texture& last									// texture used for last tile
		) {
			loadTileSingle(platTilePositions.x, first);
			if (midTileCount.x != 0 && midTileCount.y != 0) { // when midTileCount is non zero, middle tiles and last tile use thier respective positions
				loadTileMultiple(platTilePositions.y, midTileCount, mid);
				loadTileSingle(platTilePositions.z, last);
			}
			else { // when midTileCount is zero, last tile uses positions of middle tile
				loadTileSingle(platTilePositions.y, last);
			}
	};

	if (tileCount.x == 1 && tileCount.y == 1)
		loadTileSingle({ 0, 0 }, tileTextures[10]); // mid top

	else if (tileCount.x == 1)
		loadTileLine({ {0,0} ,{0,1} , {0 , tileCount.y - 1} }, { 1, tileCount.y - 2 }, tileTextures[1], tileTextures[4], tileTextures[7]); // mid line

	else if (tileCount.y == 1)
		loadTileLine({ {0,0}, {1,0}, {tileCount.x - 1 , 0} }, { tileCount.x - 2, 1 }, tileTextures[9], tileTextures[10], tileTextures[11]); // top edge

	else {
		loadTileLine({ {0, 0}, {1, 0}, {tileCount.x - 1, 0} }, { tileCount.x - 2, 1 }, tileTextures[0], tileTextures[1], tileTextures[2]); // top edge

		loadTileMultiple({ 0, 1 }, { 1, tileCount.y - 2 }, tileTextures[3]); // left edge

		loadTileMultiple({ 1, 1 }, { tileCount.x - 2, tileCount.y - 2 }, tileTextures[4]);// center

		loadTileMultiple({ tileCount.x - 1, 1 }, { 1, tileCount.y - 2 }, tileTextures[5]); // right edge

		loadTileLine({
				{0, tileCount.y - 1},
				{1, tileCount.y - 1},
				{tileCount.x - 1, tileCount.y - 1} },
			{ tileCount.x - 2, 1 }, tileTextures[6], tileTextures[7], tileTextures[8]
			); // bottom edge
	}
}