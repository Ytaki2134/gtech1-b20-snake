#include "MainSDLWindow.hpp"


int main(int argc, char* argv[]){
    int windowWidth = 800;
    int windowHeight = 800;

    MainSDLWindow* mainWindow = new MainSDLWindow();
    bool failedInit = mainWindow->Init("Snake", windowWidth, windowHeight);
    if(failedInit){
        return EXIT_FAILURE;
    }

    mainWindow->Run();

    delete mainWindow;

    return EXIT_SUCCESS;
}


