#include "GameManagers/GameState.h"

GameState::GameState() : state(GameStateType::MainMenu) {}

void GameState::setState(GameStateType newState) { 
    state = newState; 
}

GameStateType GameState::getState() const { 
    return state;
}

bool GameState::isPlaying() const {
    return state == GameStateType::Playing;
}

bool GameState::isPaused() const { 
    return state == GameStateType::Paused; 
}

bool GameState::isGameOver() const { 
    return state == GameStateType::GameOver;
}

bool GameState::isMainMenu() const { 
    return state == GameStateType::MainMenu; 
}