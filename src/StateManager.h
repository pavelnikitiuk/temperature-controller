// #ifndef STATE_MANAGER_H
// #define STATE_MANAGER_H

// #include <StateMachine.h>

// #include "Variables.h"
// #include "ConfigManager.h"
// #include "DisplayManager.h"

// class StateManager {
//   public:
//     StateManager(ConfigManager &configManager, DisplayManager &displayManager);
//     void handle();
//     void setState(AppState state);

//   private:
//     StateMachine stateMachine;
//     ConfigManager &configManager;
//     DisplayManager &displayManager;
//     State* mainScreen;
//     State* settingsScreen;
//     State* getState(AppState state);
// };

// #endif