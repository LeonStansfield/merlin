#pragma once

class GlobalVariables {
public:
    static GlobalVariables& GetInstance() {
        static GlobalVariables instance;
        return instance;
    }

    // Add your global variables here
    int score = 0;
    bool isPaused = false;

private:
    GlobalVariables() {}  // Private constructor to prevent instantiation
    GlobalVariables(const GlobalVariables&) = delete;  // Delete copy constructor
    void operator=(const GlobalVariables&) = delete;   // Delete assignment operator
};
