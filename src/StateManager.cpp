// #include "StateManager.h"

// StateManager::StateManager(ConfigManager &config, DisplayManager &display)
//     : configManager(config), displayManager(display) {}

// void StateManager::handle()
// {
//   stateMachine.run();
// }

// State *StateManager::getState(AppState state)
// {
//   switch (state)
//   {
//   case MAIN_SCREEN:
//     return mainScreen;
//   case SETTINGS_SCREEN:
//     return settingsScreen;
//   default:
//     return mainScreen;
//   }
// }

// void StateManager::setState(AppState state)
// {
//   State* machineState = getState(state);
//   stateMachine.transitionTo(machineState);
// }