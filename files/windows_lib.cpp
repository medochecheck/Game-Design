#include <iostream>
#include <memory>
#include "engine.cpp"


class WindowInterface {
public:
    virtual void handleEvents() = 0;
    virtual void update() = 0;
    virtual void render() = 0;
    virtual void run() = 0;
};

class PhotoViewerAppWrapper : public WindowInterface {
private:
    std::unique_ptr<PhotoViewerApp> app;

public:
    PhotoViewerAppWrapper() {
        app = std::make_unique<PhotoViewerApp>();
    }

    void handleEvents() override {
        app->handleEvents();
    }

    void update() override {
        app->update();
    }

    void render() override {
        app->render();
    }

    void run() override {
        app->run();
    }
};

int main() {
    try {
        DebugLogger::initialize();
        std::unique_ptr<WindowInterface> app = std::make_unique<PhotoViewerAppWrapper>();
        app->run();
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }
    return 0;
}
