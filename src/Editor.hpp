#pragma once
#include <SFML/Graphics.hpp>
#include <ZUI.hpp>
#include <vector>
#include <array>
#include <memory>
#include <queue>
#include "Platform.hpp"

class Editor {
private:
	void initWindow();
	void initzui();
	void initzuiFunctionality();
public:
	Editor();

	bool isRunning() const;

	void update();
	void pollEvents();
	void render();

	sf::Vector2f getMousePosition() const;
	sf::Vector2f getWinSize() const;

	sf::Vector2f m_lvlSize;

	sf::Font m_font;
	sf::Texture m_texture_update, m_texture_pencil, m_texture_eraser, m_texture_select, m_texture_lvlTile;
	std::array<std::array<sf::Texture, 12>, 1> m_texture_tiles;

	zui::Frame m_frame;
	zui::Page_ptr m_page_left, m_page_top;
	zui::TextButton_ptr m_textbutton_merge, m_textbutton_delete, m_textbutton_clear;
	zui::Button_ptr m_button_eraser, m_button_pencil, m_button_select, m_button_updatelvlWH;
	zui::Inputbox_ptr m_input_lvlW, m_input_lvlH;
	zui::Dropdown_ptr m_drop_tiles, m_drop_objects;
	zui::Page_ptr m_page_lvl;

	std::vector<std::shared_ptr<Platform>> m_button_platforms;
	std::queue<std::shared_ptr<Platform>> m_button_inactive_platforms;
	Platform m_button_curr_plat;
	std::vector<std::shared_ptr<Platform>> m_button_selected_platforms;
	std::vector<sf::VertexArray> m_selected_platforms_quad;

	sf::Vector2f m_plat_quad[4];

	std::array<zui::Button_ptr, 1> m_button_tileChoices;
	
	enum PenMode { SELECT, DRAW, ERASE } m_penMode;
	int m_penSize;

	enum TileType { GRASS};
	TileType m_tileType;

	
	zui::TextButton_ptr m_textbutton_saveLvl, m_textbutton_loadLvl;
	zui::Inputbox_ptr m_input_fileName;

	void addButtonPlatform();
	

private:
	sf::RenderWindow m_window;
	bool m_running;
};
