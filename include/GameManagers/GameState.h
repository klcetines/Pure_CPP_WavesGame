#ifndef GAMESTATE_H
#define GAMESTATE_H

enum class GameStateType {
    MainMenu,
    Playing,
    Paused,
    GameOver
};

class GameState {
public:
    GameState();

    void setState(GameStateType newState);
    GameStateType getState() const;

    bool isPlaying() const;
    bool isPaused() const;
    bool isGameOver() const;
    bool isMainMenu() const;

private:
    GameStateType state;
};

#endif