#pragma once

class GlobalVariables {
public:
    static GlobalVariables& GetInstance() {
        static GlobalVariables instance;
        return instance;
    }

    // Add your global variables here
    Vector2 cameraOffset = { 0, 0 };
    Vector2 playerPosition = { 0, 0 };
    int playerReference;
    int scene = 1;

private:
    GlobalVariables() {}  // Private constructor to prevent instantiation
    GlobalVariables(const GlobalVariables&) = delete;  // Delete copy constructor
    void operator=(const GlobalVariables&) = delete;   // Delete assignment operator
};
