#include <SFML/Graphics.hpp>
#include <vector>
#include <deque>
#include <cstdlib>
#include <ctime>
#include <string>
#include <optional>

using namespace std;

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
const int CELL_SIZE = 20; 
const int COLS = WINDOW_WIDTH / CELL_SIZE;  
const int ROWS = WINDOW_HEIGHT / CELL_SIZE; 

struct Point {
    int x, y;
};

int main() {
    sf::RenderWindow window(sf::VideoMode(sf::Vector2u(WINDOW_WIDTH, WINDOW_HEIGHT)), "Zmeya");
    window.setFramerateLimit(60);

    srand((unsigned)time(0));

    deque<Point> snake;
    snake.push_front({COLS / 2, ROWS / 2}); 
    
    int dirX = 1, dirY = 0; 
    int nextDirX = 1, nextDirY = 0;
    
    Point fruit;
    bool fruitSpawned = false;
    
    int score = 0;
    bool gameOver = false;
    
    sf::Clock clock;
    float moveDelay = 0.15f; 

    sf::Font font;
    if (!font.openFromFile("C:/Windows/Fonts/arial.ttf")) {
        return -1; 
    }
    sf::Text text(font);
    text.setCharacterSize(24);
    text.setFillColor(sf::Color::White);
    text.setPosition(sf::Vector2f(10.f, 10.f));

    while (window.isOpen()) {

        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }

            if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                if (!gameOver) {
                    if (keyPressed->code == sf::Keyboard::Key::W || keyPressed->code == sf::Keyboard::Key::Up) {
                        if (dirY != 1) { nextDirX = 0; nextDirY = -1; }
                    }
                    else if (keyPressed->code == sf::Keyboard::Key::S || keyPressed->code == sf::Keyboard::Key::Down) {
                        if (dirY != -1) { nextDirX = 0; nextDirY = 1; }
                    }
                    else if (keyPressed->code == sf::Keyboard::Key::A || keyPressed->code == sf::Keyboard::Key::Left) {
                        if (dirX != 1) { nextDirX = -1; nextDirY = 0; }
                    }
                    else if (keyPressed->code == sf::Keyboard::Key::D || keyPressed->code == sf::Keyboard::Key::Right) {
                        if (dirX != -1) { nextDirX = 1; nextDirY = 0; }
                    }
                } 
                else if (keyPressed->code == sf::Keyboard::Key::R) {
                    gameOver = false;
                    snake.clear();
                    snake.push_front({COLS / 2, ROWS / 2});
                    dirX = 1; dirY = 0; nextDirX = 1; nextDirY = 0;
                    score = 0;
                    fruitSpawned = false;
                    moveDelay = 0.15f;
                }
            }
        }

        if (!gameOver && clock.getElapsedTime().asSeconds() >= moveDelay) {
            clock.restart();
            
            dirX = nextDirX;
            dirY = nextDirY;

            Point newHead = {snake.front().x + dirX, snake.front().y + dirY};


            if (newHead.x < 0 || newHead.x >= COLS || newHead.y < 0 || newHead.y >= ROWS) {
                gameOver = true;
            }


            for (size_t i = 0; i < snake.size(); i++) {
                if (snake[i].x == newHead.x && snake[i].y == newHead.y) {
                    gameOver = true;
                }
            }

            if (!gameOver) {
                snake.push_front(newHead); 

                if (newHead.x == fruit.x && newHead.y == fruit.y) {
                    score += 1;
                    fruitSpawned = false; 
                    if (moveDelay > 0.05f) moveDelay -= 0.01f; 
                } else {
                    snake.pop_back(); 
                }
            }
        }

        if (!fruitSpawned && !gameOver) {
            bool validPosition = false;
            while (!validPosition) {
                fruit.x = rand() % COLS;
                fruit.y = rand() % ROWS;
                validPosition = true;
                for (auto& segment : snake) {
                    if (segment.x == fruit.x && segment.y == fruit.y) {
                        validPosition = false;
                        break;
                    }
                }
            }
            fruitSpawned = true;
        }

        window.clear(sf::Color(30, 30, 30));

        if (!gameOver) {
            sf::CircleShape fruitShape(CELL_SIZE / 2.0f);
            fruitShape.setFillColor(sf::Color::Red);
            fruitShape.setPosition(sf::Vector2f(static_cast<float>(fruit.x * CELL_SIZE), static_cast<float>(fruit.y * CELL_SIZE)));
            window.draw(fruitShape);

            for (size_t i = 0; i < snake.size(); i++) {
                sf::RectangleShape block(sf::Vector2f(CELL_SIZE - 1.f, CELL_SIZE - 1.f));
                
                if (i == 0) block.setFillColor(sf::Color::Green);
                else block.setFillColor(sf::Color(0, 150, 0)); 

                block.setPosition(sf::Vector2f(static_cast<float>(snake[i].x * CELL_SIZE), static_cast<float>(snake[i].y * CELL_SIZE)));
                window.draw(block);
            }

            text.setString("Score: " + to_string(score));
            window.draw(text);
        } else {
            text.setString("GAME OVER!\nScore: " + to_string(score) + "\nPress 'R' to Restart");
            text.setCharacterSize(40);
            sf::FloatRect textRect = text.getLocalBounds();
            text.setOrigin(sf::Vector2f(textRect.position.x + textRect.size.x / 2.0f, textRect.position.y + textRect.size.y / 2.0f));
            text.setPosition(sf::Vector2f(WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f));
            
            window.draw(text);
            
            text.setCharacterSize(24);
            text.setOrigin(sf::Vector2f(0.f, 0.f));
            text.setPosition(sf::Vector2f(10.f, 10.f));
        }

        window.display();
    }

    return 0;
}