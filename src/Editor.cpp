#include "Editor.hpp"
#include <iostream>
#include <fstream>

void Editor::initWindow()
{
	sf::ContextSettings settings;
	settings.antialiasingLevel = 4;

	m_window.create(sf::VideoMode(1400, 700), "Level Editor", sf::Style::Default, settings);
	m_window.setFramerateLimit(60);
}

void Editor::initzui()
{
	m_lvlSize = { 32.f, 16.f }; m_tileType = GRASS; m_penSize = 1;  m_penMode = SELECT;

	for (int i = 0; i < 12; i++) {
		m_texture_tiles[0][i].loadFromFile("data/textures/tiles/grass/Tile (" + std::to_string(i) + ").png");
		m_texture_tiles[0][i].setRepeated(true);
	}

	m_font.loadFromFile("data/fonts/BIG JOHN.otf");
	m_texture_update.loadFromFile("data/textures/update.png");
	m_texture_pencil.loadFromFile("data/textures/pencil.png");
	m_texture_eraser.loadFromFile("data/textures/eraser.png");
	m_texture_select.loadFromFile("data/textures/select.png");
	m_texture_lvlTile.loadFromFile("data/textures/lvlTile.png"); m_texture_lvlTile.setRepeated(true);

	sf::Color black_shade(20, 16, 32), yellow_shade(255, 178, 0), white_shade(220, 220, 220), grey_shade(231, 231, 231);

	m_frame.setWindow(m_window);

	m_page_left = zui::create<zui::Page>();
	m_page_left->setMaxSize({ getWinSize().x * 0.25f, getWinSize().y });
	m_page_left->setActiveRegion({ 0, 0, m_page_left->getMaxSize().x, m_page_left->getMaxSize().y });
	m_page_left->setFillColor(black_shade);
	m_page_left->setPosition(0, 0);

	m_textbutton_merge = zui::create<zui::TextButton>();
	m_textbutton_merge->setSize(sf::Vector2f(125, 50));
	m_textbutton_merge->setOutlineColor(yellow_shade);
	m_textbutton_merge->setFillColor(black_shade);
	m_textbutton_merge->setOutlineThickness(2);
	m_textbutton_merge->setSelectionFillColor(yellow_shade);
	m_textbutton_merge->setSelectionOutlineColor(yellow_shade);
	m_textbutton_merge->setSelectionOutlineThickness(2);
	m_textbutton_merge->setPosition(30, 30);
	m_textbutton_merge->setFont(m_font);
	m_textbutton_merge->setString("Merge");
	m_textbutton_merge->setCharacterSize(20);
	m_textbutton_merge->setTextFillColor(white_shade);
	m_textbutton_merge->setTextOutlineColor(black_shade);
	m_textbutton_merge->setTextOutlineThickness(1);
	m_textbutton_merge->actionEvent = zui::Entity::ActionEvent::PRESS;

	m_textbutton_delete = zui::copy(m_textbutton_merge);
	m_textbutton_delete->setString("Delete");
	m_textbutton_delete->setPosition(185, 30);

	m_drop_tiles = zui::create<zui::Dropdown>();
	m_drop_tiles->setFillColor(grey_shade);
	m_drop_tiles->setHeaderSize(sf::Vector2f(100, 50));
	m_drop_tiles->setPosition(50, 330);
	m_drop_tiles->setItemHeight(m_drop_tiles->getHeaderSize().x - 30);
	m_drop_tiles->setItemLimit(5);
	m_drop_tiles->setHeaderColor(yellow_shade);
	m_drop_tiles->setHeaderOutlineThickness(2);
	m_drop_tiles->setHeaderOutlineColor(white_shade);
	m_drop_tiles->setMinimiseButtonColor(white_shade);
	m_drop_tiles->setMinimiseButtonOutlineColor(black_shade);
	m_drop_tiles->setTitle(" Tile", m_font, 20, black_shade);
	m_drop_tiles->setMovementEnabled(false);
	m_drop_tiles->collapse();
	
	m_drop_objects = zui::create<zui::Dropdown>();
	m_drop_objects->setFillColor(grey_shade);
	m_drop_objects->setHeaderSize(sf::Vector2f(100, 50));
	m_drop_objects->setPosition(190, 330);
	m_drop_objects->setItemHeight(m_drop_objects->getHeaderSize().x - 30);
	m_drop_objects->setItemLimit(5);
	m_drop_objects->setHeaderColor(yellow_shade);
	m_drop_objects->setHeaderOutlineThickness(2);
	m_drop_objects->setHeaderOutlineColor(white_shade);
	m_drop_objects->setMinimiseButtonColor(white_shade);
	m_drop_objects->setMinimiseButtonOutlineColor(black_shade);
	m_drop_objects->deactivateSelection();
	m_drop_objects->setTitle(" Item", m_font, 20, black_shade);
	m_drop_objects->setMovementEnabled(false);
	m_drop_objects->collapse();

	m_button_eraser = zui::create<zui::Button>();
	m_button_eraser->setPointCount(4);
	m_button_eraser->setPoint(0, sf::Vector2f(0, 0));
	m_button_eraser->setPoint(1, sf::Vector2f(75.f, 0));
	m_button_eraser->setPoint(2, sf::Vector2f(75.f, 75.f));
	m_button_eraser->setPoint(3, sf::Vector2f(0, 75.f));
	m_button_eraser->setOrigin(37.5f, 37.5f);
	m_button_eraser->setSelectionScale(sf::Vector2f(1.1f, 1.1f));
	m_button_eraser->setPosition(105.f, 167.5f);
	m_button_eraser->setTexture(&m_texture_eraser);
	m_button_eraser->setFillColor(sf::Color(126, 126, 126));
	m_button_eraser->setSelectionFillColor(grey_shade);

	m_button_pencil = zui::copy(m_button_eraser);
	m_button_pencil->setTexture(&m_texture_pencil);
	m_button_pencil->setPosition(225.f, 167.5f);
	m_button_pencil->setFillColor(sf::Color(126, 126, 126));
	m_button_pencil->setSelectionFillColor(grey_shade);

	m_button_select = zui::copy(m_button_eraser);
	m_button_select->setTexture(&m_texture_select);
	m_button_select->setPosition(105.f, 262.5f);
	m_button_select->setFillColor(sf::Color(126, 126, 126));
	m_button_select->setSelectionFillColor(grey_shade);

	m_page_left->addEntity(m_textbutton_merge.get());
	m_page_left->addEntity(m_textbutton_delete.get());
	m_page_left->addEntity(m_drop_tiles.get());
	m_page_left->addEntity(m_drop_objects.get());
	m_page_left->addEntity(m_button_eraser.get());
	m_page_left->addEntity(m_button_pencil.get());
	m_page_left->addEntity(m_button_select.get());

	m_page_top = zui::create<zui::Page>();
	m_page_top->setMaxSize({ 1024, 75 });
	m_page_top->setActiveRegion({ 0, 0, m_page_top->getMaxSize().x, m_page_top->getMaxSize().y });
	m_page_top->setFillColor(yellow_shade);
	m_page_top->setPosition(getWinSize().x * 0.25f + 15, 20);

	m_input_lvlW = zui::create<zui::Inputbox>();
	m_input_lvlW->setSize(sf::Vector2f(100, 50));
	m_input_lvlW->setFillColor(black_shade);
	m_input_lvlW->setOutlineThickness(2);
	m_input_lvlW->setOutlineColor(grey_shade);
	m_input_lvlW->setFont(m_font);
	m_input_lvlW->setCharacterSize(22);
	m_input_lvlW->setTextFillColor(white_shade);
	m_input_lvlW->setPosition(50.0f, 12.5f);
	m_input_lvlW->setTextOutlineColor(yellow_shade);
	m_input_lvlW->setSelectionFillColor(black_shade);
	m_input_lvlW->setSelectionOutlineColor(sf::Color::Cyan);
	m_input_lvlW->setSelectionOutlineThickness(3);
	m_input_lvlW->setString("32");

	m_input_lvlH = zui::copy(m_input_lvlW);
	m_input_lvlH->setPosition(160.0f, 12.5f);
	m_input_lvlH->setString("16");

	m_button_updatelvlWH = zui::create<zui::Button>();
	m_button_updatelvlWH->setPointCount(4);
	m_button_updatelvlWH->setPoint(0, sf::Vector2f(0, 0));
	m_button_updatelvlWH->setPoint(1, sf::Vector2f(50, 0));
	m_button_updatelvlWH->setPoint(2, sf::Vector2f(50, 50));
	m_button_updatelvlWH->setPoint(3, sf::Vector2f(0, 50));
	m_button_updatelvlWH->setOrigin(25.f, 25.f);
	m_button_updatelvlWH->setPosition(305.f, 37.5f);
	m_button_updatelvlWH->setSelectionScale(sf::Vector2f(1.1f, 1.1f));
	m_button_updatelvlWH->setTexture(&m_texture_update);

	m_textbutton_clear = zui::create<zui::TextButton>();
	m_textbutton_clear->setSize(sf::Vector2f(105, 50));
	m_textbutton_clear->setFillColor(yellow_shade);
	m_textbutton_clear->setSelectionFillColor(yellow_shade);
	m_textbutton_clear->setSelectionScale(sf::Vector2f(1.1f, 1.1f));
	m_textbutton_clear->setOrigin(m_textbutton_clear->getSize() * 0.5f);
	m_textbutton_clear->setPosition(402.5f, 37.5f);
	m_textbutton_clear->setFont(m_font);
	m_textbutton_clear->setString("Clear");
	m_textbutton_clear->setCharacterSize(22);
	m_textbutton_clear->setTextFillColor(black_shade);

	m_input_fileName = zui::copy(m_input_lvlW);
	m_input_fileName->setSize(sf::Vector2f(300, 50));
	m_input_fileName->setPosition(475.f, 12.5f);
	m_input_fileName->setCharacterSize(20);
	m_input_fileName->setString("Level");

	m_textbutton_saveLvl = zui::copy(m_textbutton_clear);
	m_textbutton_saveLvl->setPosition(840.f, 37.5f);
	m_textbutton_saveLvl->setString("Save");

	m_textbutton_loadLvl = zui::copy(m_textbutton_clear);
	m_textbutton_loadLvl->setPosition(940.f, 37.5f);
	m_textbutton_loadLvl->setString("Load");

	m_page_top->addEntity(m_input_lvlW.get());
	m_page_top->addEntity(m_input_lvlH.get());
	m_page_top->addEntity(m_button_updatelvlWH.get());
	m_page_top->addEntity(m_textbutton_clear.get());
	m_page_top->addEntity(m_input_fileName.get());
	m_page_top->addEntity(m_textbutton_saveLvl.get());
	m_page_top->addEntity(m_textbutton_loadLvl.get());

	m_page_lvl = zui::create<zui::Page>();
	m_page_lvl->setMaxSize({ 1024, 512 });
	m_page_lvl->setActiveRegion({ 0, 0, m_page_lvl->getMaxSize().x, m_page_lvl->getMaxSize().y });
	m_page_lvl->setFillColor(sf::Color::Transparent);
	m_page_lvl->setPosition(getWinSize().x * 0.25f + 15, 150);
	m_page_lvl->setScroll(zui::Page::BOTTOM);
	m_page_lvl->setScroll(zui::Page::RIGHT);
	m_page_lvl->setTexture(&m_texture_lvlTile);
	m_page_lvl->setTextureRect(sf::IntRect(0,0, m_page_lvl->getSize().x, m_page_lvl->getSize().y));
	m_page_lvl->setFillColor(white_shade);

	m_frame.addEntity(m_page_left.get());
	m_frame.addEntity(m_page_top.get());
	m_frame.addEntity(m_page_lvl.get());

	m_button_tileChoices[0] = zui::create<zui::Button>();
	m_button_tileChoices[0]->setSelectionScale(sf::Vector2f(1.1f, 1.1f));
	m_button_tileChoices[0]->setTexture(&m_texture_tiles[0][1]);
	m_drop_tiles->addItem(m_button_tileChoices[0].get());


	m_button_curr_plat.setInactive();

	m_page_lvl->addEntity(m_button_curr_plat);

	m_frame.push_in_navigationOrder(*m_input_lvlW);
	m_frame.push_in_navigationOrder(*m_input_lvlH);
	m_frame.push_in_navigationOrder(*m_button_updatelvlWH);
	m_frame.push_in_navigationOrder(*m_textbutton_clear);
}

void Editor::initzuiFunctionality()
{
	
	m_textbutton_delete->setAction([this]
		{
			for (auto it = m_button_selected_platforms.begin(); it != m_button_selected_platforms.end();){
				
				for (int i = 0; i < m_button_platforms.size(); i++) {
					if (m_button_platforms[i]->getID() == it->get()->getID())
						m_button_platforms.erase(m_button_platforms.begin() + i);
				}

				m_page_lvl->removeEntity(it->get()->getID());

				it = m_button_selected_platforms.erase(it);
			}
		}
	);

	m_button_updatelvlWH->setAction([this]
		{
			int newW = std::stoi(m_input_lvlW->getString());
			int newH = std::stoi(m_input_lvlH->getString());

			newW = std::max(newW, 32); newW = std::min(newW, 512);
			newH = std::max(newH, 16); newH = std::min(newH, 32);


			m_lvlSize.x = newW;
			m_lvlSize.y = newH;

			m_page_lvl->setMaxSize(sf::Vector2f(m_lvlSize.x * 32, m_lvlSize.y * 32));
			m_page_lvl->setActiveRegion({ 0, 0, 1024, 522 });
			m_page_lvl->setTextureRect(sf::IntRect(0, 0, m_page_lvl->getMaxSize().x / 2, m_page_lvl->getMaxSize().y / 2));

			m_input_lvlW->setString(std::to_string(newW));
			m_input_lvlH->setString(std::to_string(newH));
		}
	);

	m_button_pencil->setAction([this]
		{
			m_button_pencil->setFillColor(sf::Color::Cyan);
			m_button_eraser->setFillColor(sf::Color(126, 126, 126));
			m_button_select->setFillColor(sf::Color(126, 126, 126));
			m_penMode = DRAW;
		}
	);

	m_button_eraser->setAction([this]
		{
			m_button_eraser->setFillColor(sf::Color::Cyan);
			m_button_pencil->setFillColor(sf::Color(126, 126, 126));
			m_button_select->setFillColor(sf::Color(126, 126, 126));
			m_penMode = ERASE;

			m_button_curr_plat.setInactive();
		}
	);

	m_button_select->setAction([this]
		{
			m_button_select->setFillColor(sf::Color::Cyan);
			m_button_pencil->setFillColor(sf::Color(126, 126, 126));
			m_button_eraser->setFillColor(sf::Color(126, 126, 126));
			m_penMode = SELECT;

			m_button_curr_plat.setInactive();
		}
	);

	for (int i = 0; i < m_button_tileChoices.size(); i++) {
		m_button_tileChoices[i]->setAction([this, i]
			{
				m_tileType = (TileType)i;
			}
		);
	}

	m_textbutton_clear->setAction([this]
		{
			for (int i = 0; i < m_button_platforms.size(); i++) {
				m_page_lvl->removeEntity(m_button_platforms[i]->getID());
			}

			m_button_platforms.clear();
			m_button_curr_plat.setInactive();
			while(!m_button_inactive_platforms.empty())
				m_button_inactive_platforms.pop();
			m_button_selected_platforms.clear();
		}
	);


	m_textbutton_saveLvl->setAction([this]
		{
			std::ofstream lvl;
			
			std::string fileName = m_input_fileName->getString();
			if (fileName.empty())fileName = "Level";
			lvl.open("data/levels/" + fileName + ".txt");

			if (!lvl.is_open()) {
				std::cout << "Error while loading file : data / levels / " + fileName + ".txt\n";
			}
			else {

				lvl << "l " << m_input_lvlW->getString() << " " << m_input_lvlH->getString() << '\n';

				for (auto it = m_button_platforms.begin(); it != m_button_platforms.end(); it++) {

					sf::Vector2f sizef(it->get()->getPoint(2) - it->get()->getPoint(0));
					sf::Vector2i startPos((it->get()->getPosition() - sizef / 2.f) / 32.f);
					sf::Vector2i size(sizef / 32.f);

					lvl << "# " << std::to_string(startPos.x) << ' ' << std::to_string(startPos.y) << ' ' << std::to_string(size.x) << ' ' << std::to_string(size.y) << '\n';
				}
			}
			
			lvl.close();
		}
	);

	m_textbutton_loadLvl->setAction([this]
		{
			m_textbutton_clear->callAction();

			std::ifstream lvl;

			std::string fileName = m_input_fileName->getString();
			if (fileName.empty())fileName = "Level";
			lvl.open("data/levels/" + fileName + ".txt");

			if (!lvl.is_open()) {
				std::cout << "Error while loading file : data / levels / " + fileName + ".txt\n";
			}
			else {
				std::string info;
				while (lvl >> info)
				{
					if (info == "l") {
						lvl >> info; m_input_lvlW->setString(info);
						lvl >> info; m_input_lvlH->setString(info);

						m_button_updatelvlWH->callAction();
					}

					else if (info == "#") {
						sf::Vector2f size, pos;

						lvl >> pos.x >> pos.y >> size.x >> size.y;

						m_button_platforms.push_back(std::make_shared<Platform>());
						m_button_platforms.back()->load(pos * 32.f, size * 32.f, m_texture_tiles[m_tileType]);
						m_button_platforms.back()->setSelectionScale(sf::Vector2f(1.05, 1.05));

						m_page_lvl->addEntity(*m_button_platforms.back());
					}

				}
			}

			lvl.close();
		}
	);

}

void Editor::addButtonPlatform()
{
	m_button_platforms.push_back(std::make_shared<Platform>());

	sf::Vector2f size = m_button_curr_plat.getPoint(2) - m_button_curr_plat.getPoint(0);
	m_button_platforms.back()->load(m_button_curr_plat.getPosition() - size * 0.5f, size, m_texture_tiles[m_tileType]);
	m_button_platforms.back()->setSelectionScale(sf::Vector2f(1.05, 1.05));

	m_page_lvl->addEntity(*m_button_platforms.back());
}

Editor::Editor()
{
	initWindow();
	initzui();
	initzuiFunctionality();

	m_button_pencil->callAction();

	m_penMode = DRAW;
	m_penSize = 1;

	m_running = true;
}

bool Editor::isRunning() const
{
	return m_running;
}

void Editor::update()
{
	if (m_button_inactive_platforms.size() >= 2) {
		m_button_inactive_platforms.pop();
	}


	if (m_penMode == DRAW && m_button_curr_plat.isActive()) {
		sf::Vector2f endPos = m_page_lvl->getMousePosition();
		endPos.x = ((int)(endPos.x / 32)) * 32;
		endPos.y = ((int)(endPos.y / 32)) * 32;

		sf::Vector2f startPos = m_plat_quad[0];

		if (endPos.x >= startPos.x)endPos.x += 32;
		if (endPos.y >= startPos.y)endPos.y += 32;

		m_plat_quad[0].x = startPos.x;			m_plat_quad[0].y = startPos.y;
		m_plat_quad[1].x = endPos.x;			m_plat_quad[1].y = startPos.y;
		m_plat_quad[2].x = endPos.x;			m_plat_quad[2].y = endPos.y;
		m_plat_quad[3].x = startPos.x;			m_plat_quad[3].y = endPos.y;

		sf::Vector2f size = m_plat_quad[2] - m_plat_quad[0];
		size.x = abs(size.x); size.y = abs(size.y);

		startPos = m_plat_quad[0];
		startPos.x = std::min(startPos.x, m_plat_quad[1].x);
		startPos.x = std::min(startPos.x, m_plat_quad[2].x);
		startPos.x = std::min(startPos.x, m_plat_quad[3].x);
										  
		startPos.y = std::min(startPos.y, m_plat_quad[1].y);
		startPos.y = std::min(startPos.y, m_plat_quad[2].y);
		startPos.y = std::min(startPos.y, m_plat_quad[3].y);

		m_button_curr_plat.load(startPos, size, m_texture_tiles[m_tileType]);
	}

	m_frame.update();

	pollEvents();
}

void Editor::pollEvents()
{
	sf::Event event;
	while (m_window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed || event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
			m_running = false;

		if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
			if (m_page_lvl->contains(m_frame.getMousePosition())) {
				if (m_penMode == DRAW) {
					if (m_button_curr_plat.isActive()) {
						addButtonPlatform();
						m_button_curr_plat.setInactive();
					}

					else {
						sf::Vector2f pos = m_page_lvl->getMousePosition();
						pos.x = ((int)(pos.x / 32)) * 32;
						pos.y = ((int)(pos.y / 32)) * 32;

						m_plat_quad[0].x = pos.x;			m_plat_quad[0].y = pos.y;
						m_plat_quad[1].x = pos.x + 32;		m_plat_quad[1].y = pos.y;
						m_plat_quad[2].x = pos.x + 32;		m_plat_quad[2].y = pos.y + 32;
						m_plat_quad[3].x = pos.x;			m_plat_quad[3].y = pos.y + 32;

						m_button_curr_plat.setActive();
						m_button_curr_plat.load(m_plat_quad[0], m_plat_quad[2] - m_plat_quad[0], m_texture_tiles[m_tileType]);
					}
				}
			}
		}

		else if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
			if (m_page_lvl->contains(m_frame.getMousePosition()) && m_frame.getClickedEntity() == m_frame.getMouseHoveringOnWhichEntity()) {
				if (m_penMode == ERASE) {
					zui::Entity* entity = m_frame.getClickedEntity();
					if (zui::Entity::getClassID(*entity) == zui::ZUI_ID_BUTTON) {

						for (auto it = m_button_platforms.begin(); it != m_button_platforms.end(); it++) {
							if ((*it)->getID() == entity->getID()) {
								m_button_inactive_platforms.push(*it);
								m_button_platforms.erase(it);
								break;
							}
						}
						m_page_lvl->removeEntity(entity->getID());
					}
				}
			}
		}

		m_frame.pollEvents(event);
		

		if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
			if (m_penMode == SELECT && m_frame.getClickedEntity() == m_frame.getMouseHoveringOnWhichEntity()) {

				if (!sf::Keyboard::isKeyPressed(sf::Keyboard::RControl) && !sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)) {
					m_button_selected_platforms.clear();
					m_selected_platforms_quad.clear();
				}

				zui::Entity* entity = m_frame.getClickedEntity();
				if (zui::Entity::getClassID(*entity) == zui::ZUI_ID_BUTTON) {
					for (auto it = m_button_platforms.begin(); it != m_button_platforms.end(); it++) {
						if ((*it)->getID() == entity->getID()) {
							m_button_selected_platforms.push_back(*it);
							m_selected_platforms_quad.emplace_back();
							sf::Vector2f offset((*it)->getPosition() - ((*it)->getPoint(2) - (*it)->getPoint(0)) * 0.5f - m_page_lvl->getActiveRegionPosition() + m_page_lvl->getPosition());
							m_selected_platforms_quad.back().setPrimitiveType(sf::Quads);
							m_selected_platforms_quad.back().append(sf::Vertex((*it)->getPoint(0) + offset, sf::Color(0, 0, 180, 76)));
							m_selected_platforms_quad.back().append(sf::Vertex((*it)->getPoint(1) + offset, sf::Color(0, 0, 180, 76)));
							m_selected_platforms_quad.back().append(sf::Vertex((*it)->getPoint(2) + offset, sf::Color(0, 0, 180, 76)));
							m_selected_platforms_quad.back().append(sf::Vertex((*it)->getPoint(3) + offset, sf::Color(0, 0, 180, 76)));
							break;
						}
					}
				}
			}
		}
	}
}

void Editor::render()
{
	m_window.clear(sf::Color(231, 231, 231));

	m_frame.draw();

	if (m_button_curr_plat.isActive() && m_page_lvl->contains(getMousePosition())) {
		sf::Vertex quad[5] = {
			{m_plat_quad[0] - m_page_lvl->getActiveRegionPosition() + m_page_lvl->getPosition(), sf::Color::Red},
			{m_plat_quad[1] - m_page_lvl->getActiveRegionPosition() + m_page_lvl->getPosition(), sf::Color::Red},
			{m_plat_quad[2] - m_page_lvl->getActiveRegionPosition() + m_page_lvl->getPosition(), sf::Color::Red},
			{m_plat_quad[3] - m_page_lvl->getActiveRegionPosition() + m_page_lvl->getPosition(), sf::Color::Red},
			{m_plat_quad[0] - m_page_lvl->getActiveRegionPosition() + m_page_lvl->getPosition(), sf::Color::Red}
		};
		m_window.draw(quad, 5, sf::LinesStrip);

		sf::Color transWhite(255, 255, 255, 48);
		quad[0].color = transWhite;
		quad[1].color = transWhite;
		quad[2].color = transWhite;
		quad[3].color = transWhite;

		m_window.draw(quad, 4, sf::Quads);
	}

	for (auto it = m_selected_platforms_quad.begin(); it != m_selected_platforms_quad.end(); it++) {
		m_window.draw(*it);
	}

	m_window.display();
}

sf::Vector2f Editor::getMousePosition() const
{
	return m_window.mapPixelToCoords(sf::Mouse::getPosition(m_window));
}

sf::Vector2f Editor::getWinSize() const
{
	return sf::Vector2f(m_window.getSize());
}