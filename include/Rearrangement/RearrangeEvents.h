#ifndef REARRANGE_EVENTS_H
#define REARRANGE_EVENTS_H

class Rearrange;
#include <SFML/Graphics.hpp>

namespace RearrangeEvents {
    void handleRearrangeInput(Rearrange& rearrange, sf::RenderWindow& window, bool& running);
    void handleKeyPressed(Rearrange& rearrange, const sf::Event& event);
}


#endif // REARRANGE_EVENTS_H