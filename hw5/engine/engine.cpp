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

class BaseContainer {
protected:
    std::vector<std::shared_ptr<BaseContainer>> children;

public:
    virtual void handleEvents(sf::RenderWindow& window, sf::Event& event) {
        for (auto& child : children) {
            child->handleEvents(window, event);
        }
    }

    virtual void update() {
        for (auto& child : children) {
            child->update();
        }
    }

    virtual void render(sf::RenderWindow& window) {
        for (auto& child : children) {
            child->render(window);
        }
    }

    void addChild(const std::shared_ptr<BaseContainer>& child) {
        children.push_back(child);
    }
};


class ImageContainer : public BaseContainer {
protected:
    sf::Sprite sprite;
    sf::Texture texture;
    sf::Vector2f position;
    float scale;

public:
    ImageContainer(const std::string& imagePath)
        : position(0.f, 0.f), scale(1.f) {
        if (!texture.loadFromFile(imagePath)) {
            throw std::runtime_error("Failed to load image");
        }
        sprite.setTexture(texture);
    }

    sf::Sprite& getSprite() { return sprite; }
    const sf::Sprite& getSprite() const { return sprite; }

    void setScale(float newScale) {
        scale = newScale;
        sprite.setScale(scale, scale);
    }

    float getScale() const {
        return scale;
    }

    void centerImage(const sf::RenderWindow& window) {
        sf::Vector2u windowSize = window.getSize();
        sf::Vector2u imageSize = texture.getSize();
        sprite.setPosition(
            (windowSize.x - imageSize.x * scale) / 2.f,
            (windowSize.y - imageSize.y * scale) / 2.f
        );
    }

    void render(sf::RenderWindow& window) override {
        window.draw(sprite);
    }
};

class Slider : public ImageContainer {
private:
    bool isDragging;
    bool isWindowOpen;
    sf::Vector2f mousePosition;

public:
    Slider(const std::string& imagePath)
        : ImageContainer(imagePath), isDragging(false), isWindowOpen(false) {
        setScale(1.f);
    }

    void setWindowOpen(bool open) {
        isWindowOpen = open;
    }

    void handleEvents(sf::RenderWindow& window, sf::Event& event) override {
        if (isWindowOpen) {
            return;
        }

        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        sf::Vector2f worldPos = window.mapPixelToCoords(mousePos);
        DebugLogger::log("Mouse Pos = (" + std::to_string(worldPos.x) + ", " + std::to_string(worldPos.y) + ")");

        if (event.type == sf::Event::MouseButtonPressed) {
            if (getSprite().getGlobalBounds().contains(worldPos)) {
                isDragging = true;
                mousePosition = worldPos - getSprite().getPosition();
                DebugLogger::log("Started dragging");
            }
        }

        if (event.type == sf::Event::MouseButtonReleased) {
            if (isDragging) {
                isDragging = false;
                DebugLogger::log("Stopped dragging");
            }
        }

        if (event.type == sf::Event::MouseMoved && isDragging) {
            getSprite().setPosition(worldPos - mousePosition);
        }

        if (event.type == sf::Event::MouseWheelScrolled) {
            if (isWindowOpen) {
                return;
            }

            float oldScale = getScale();
            float newScale = getScale() + event.mouseWheelScroll.delta * 0.1f;
            setScale(std::max(0.1f, newScale));

            sf::Vector2f offset = (worldPos - getSprite().getPosition()) * (newScale / oldScale - 1.f);
            getSprite().move(-offset);
            DebugLogger::log("Mouse Wheel Scrolled: Scale = " + std::to_string(newScale));
        }

        ImageContainer::handleEvents(window, event);
    }

    void updateWithKeyboard() {
        if (isWindowOpen) {
            return;
        }

        sf::Vector2f movement(0.f, 0.f);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
            movement.y -= 0.5f;
            DebugLogger::log("Keyboard Pressed: Up Arrow");
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
            movement.y += 0.5f;
            DebugLogger::log("Keyboard Pressed: Down Arrow");
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
            movement.x -= 0.5f;
            DebugLogger::log("Keyboard Pressed: Left Arrow");
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
            movement.x += 0.5f;
            DebugLogger::log("Keyboard Pressed: Right Arrow");
        }

        getSprite().move(movement);
    }

    void update() override {
        updateWithKeyboard();
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