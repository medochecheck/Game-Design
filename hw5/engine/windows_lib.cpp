#include "engine.cpp"
#include <windows.h>

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