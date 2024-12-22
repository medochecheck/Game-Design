#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <fstream>
#include <iostream>
#include <vector>
#include <memory>
#include <windows.h>

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


class Window {
protected:
    sf::RenderWindow window;
    sf::Vector2u size;
    std::string title;

public:
    Window(const std::string& title, const sf::Vector2u& size)
        : title(title), size(size), window(sf::VideoMode::getDesktopMode(), title, sf::Style::Fullscreen) {}

    virtual void handleEvents() = 0;
    virtual void update() = 0;
    virtual void render() = 0;

    void run() {
        while (window.isOpen()) {
            handleEvents();
            update();
            window.clear();
            render();
            window.display();
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



class PhotoViewerApp : public Window {
private:
    BaseContainer container;
    sf::RectangleShape closeButton, minimizeButton, maximizeButton, openWindowButton;
    std::shared_ptr<Slider> slider;
    bool isFullscreen = true;
    float imageScale = 1.f;
    std::shared_ptr<DraggableWindow> draggableWindow;
    bool isWindowOpen;

public:
    PhotoViewerApp()
        : Window("Photo Viewer", { 800, 600 }), isWindowOpen(false) {
        slider = std::make_shared<Slider>("image.bmp");
        container.addChild(slider);

        closeButton.setSize({ 40.f, 40.f });
        closeButton.setFillColor(sf::Color::Red);
        closeButton.setPosition(window.getSize().x - 50.f, 10.f);

        maximizeButton.setSize({ 40.f, 40.f });
        maximizeButton.setFillColor(sf::Color::Yellow);
        maximizeButton.setPosition(window.getSize().x - 100.f, 10.f);

        minimizeButton.setSize({ 40.f, 40.f });
        minimizeButton.setFillColor(sf::Color::Green);
        minimizeButton.setPosition(window.getSize().x - 150.f, 10.f);

        openWindowButton.setSize({ 40.f, 40.f });
        openWindowButton.setFillColor(sf::Color::Blue);
        openWindowButton.setPosition(10.f, 10.f);

        slider->centerImage(window);
    }

    void minimizeWindow(sf::RenderWindow& window) {
        HWND hwnd = window.getSystemHandle();
        ShowWindow(hwnd, SW_MINIMIZE);
    }

    void positionButtons() {
        closeButton.setPosition(window.getSize().x - 50.f, 10.f);
        maximizeButton.setPosition(window.getSize().x - 100.f, 10.f);
        minimizeButton.setPosition(window.getSize().x - 150.f, 10.f);
    }

    void handleEvents() override {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::MouseButtonPressed) {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);

                if (openWindowButton.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                    if (!isWindowOpen) {
                        draggableWindow = std::make_shared<DraggableWindow>("image_2.bmp");
                        draggableWindow->setPosition({ 100.f, 100.f });
                        isWindowOpen = true;
                        slider->setWindowOpen(true);
                        DebugLogger::log("The second window was open");
                    }
                }

                if (closeButton.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                    window.close();
                    DebugLogger::log("The window is closed");
                }
                else if (minimizeButton.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                    minimizeWindow(window);
                    DebugLogger::log("The window is hidden");
                }
                else if (maximizeButton.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                    if (isFullscreen) {
                        imageScale = slider->getScale();
                        window.create(sf::VideoMode(1280, 768), title, sf::Style::Titlebar | sf::Style::Close);
                        window.setPosition(sf::Vector2i(
                            (sf::VideoMode::getDesktopMode().width - 1280) / 2,
                            (sf::VideoMode::getDesktopMode().height - 768) / 2
                        ));

                        slider->centerImage(window);
                        positionButtons();
                        isFullscreen = false;
                        DebugLogger::log("Switched to Windowed Mode");
                    }
                    else {
                        window.create(sf::VideoMode::getDesktopMode(), title, sf::Style::Fullscreen);
                        slider->setScale(imageScale);
                        slider->centerImage(window);
                        positionButtons();
                        isFullscreen = true;
                        DebugLogger::log("Switched to Fullscreen Mode");
                    }
                }
            }

            if (isWindowOpen) {
                draggableWindow->handleEvents(window, event);
                if (draggableWindow->isCloseButtonClicked(event, window)) {
                    isWindowOpen = false; 
                    slider->setWindowOpen(false); 
                }
            }

            container.handleEvents(window, event);
        }
    }

    void update() override {
        container.update();
    }

    void render() override {
        container.render(window);
        window.draw(closeButton);
        window.draw(minimizeButton);
        window.draw(maximizeButton);
        window.draw(openWindowButton);

        if (isWindowOpen) {
            draggableWindow->render(window);
        }
    }
};


int main() {
    DebugLogger::initialize();
    PhotoViewerApp app;
    app.run();
    return 0;
}
