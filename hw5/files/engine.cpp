#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <fstream>
#include <iostream>
#include <vector>
#include <memory>

class DebugLogger {
public:
    static void initialize() {
        std::ofstream logFile("debug.txt", std::ios::trunc);
        if (!logFile.is_open()) {
            std::cerr << "Failed to clear debug file" << std::endl;
        }
    }

    static void log(const std::string& message) {
        std::ofstream logFile("debug.txt", std::ios::app);
        if (logFile.is_open()) {
            logFile << message << std::endl;
        }
    }
};


class DraggableWindow {
private:
    sf::RectangleShape windowBackground;
    sf::Sprite imageSprite;
    sf::Texture imageTexture;
    sf::Vector2f dragOffset;
    bool isDragging;
    sf::RectangleShape closeButton;

public:
    DraggableWindow(const std::string& imagePath) : isDragging(false) {
        if (!imageTexture.loadFromFile(imagePath)) {
            throw std::runtime_error("Failed to load image for draggable window");
        }

        windowBackground.setSize({ 400.f, 300.f });
        windowBackground.setFillColor(sf::Color(100, 100, 100));

        imageSprite.setTexture(imageTexture);
        scaleImageToFitWindow();

        closeButton.setSize({ 30.f, 30.f });
        closeButton.setFillColor(sf::Color::Red);
        closeButton.setPosition(windowBackground.getSize().x - 40.f, 10.f);
    }

    void scaleImageToFitWindow() {
        sf::Vector2u imageSize = imageTexture.getSize();
        float scaleX = static_cast<float>(windowBackground.getSize().x) / static_cast<float>(imageSize.x);
        float scaleY = static_cast<float>(windowBackground.getSize().y) / static_cast<float>(imageSize.y);
        float scale = std::min(scaleX, scaleY);
        imageSprite.setScale(scale, scale);
        windowBackground.setSize({
            static_cast<float>(imageSize.x) * scale + 20.f,
            static_cast<float>(imageSize.y) * scale + 70.f
            });
    }

    void setPosition(const sf::Vector2f& position) {
        windowBackground.setPosition(position);
        imageSprite.setPosition(position.x + 10.f, position.y + 50.f);
        closeButton.setPosition(position.x + windowBackground.getSize().x - 40.f, position.y + 10.f);
    }

    void handleEvents(sf::RenderWindow& window, sf::Event& event) {
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        sf::Vector2f worldPos = window.mapPixelToCoords(mousePos);

        if (event.type == sf::Event::MouseButtonPressed) {
            if (windowBackground.getGlobalBounds().contains(worldPos)) {
                isDragging = true;
                dragOffset = worldPos - windowBackground.getPosition();
            }
            if (closeButton.getGlobalBounds().contains(worldPos)) {
                isDragging = false;
            }
        }

        if (event.type == sf::Event::MouseButtonReleased) {
            isDragging = false;
        }

        if (event.type == sf::Event::MouseMoved && isDragging) {
            windowBackground.setPosition(worldPos - dragOffset);
            imageSprite.setPosition(windowBackground.getPosition().x + 10.f, windowBackground.getPosition().y + 50.f);
            closeButton.setPosition(windowBackground.getPosition().x + windowBackground.getSize().x - 40.f, windowBackground.getPosition().y + 10.f);
        }
    }

    void render(sf::RenderWindow& window) {
        window.draw(windowBackground);
        window.draw(imageSprite);
        window.draw(closeButton);
    }

    bool isCloseButtonClicked(sf::Event& event, sf::RenderWindow& window) {
        if (event.type == sf::Event::MouseButtonPressed) {
            sf::Vector2i mousePos = sf::Mouse::getPosition(window);
            if (closeButton.getGlobalBounds().contains(window.mapPixelToCoords(mousePos))) {
                DebugLogger::log("The second window was closed");
                return true;
            }
        }
        return false;
    }
};