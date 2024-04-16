#include <bits/stdc++.h>
#include <SFML/Graphics.hpp>

auto main() -> int {
    std::srand(time(nullptr));
    auto window = sf::RenderWindow(sf::VideoMode(1000, 600), "Dinosaur",
                                   sf::Style::Default, sf::ContextSettings(0, 0, 8));
    window.setFramerateLimit(60);

    //loading up fonts
    auto font = sf::Font();
    if (!font.loadFromFile("../Arial.ttf"))
        return EXIT_FAILURE;

    //text fields
    auto text = sf::Text("Use arrow up", font, 36);
    text.setPosition({700.f, 50.f});
    text.setFillColor(sf::Color::White);
    text.setOutlineThickness(1.5);

    auto youLost = sf::Text("You Lost!", font, 70);
    youLost.setPosition({360.f, 240.f});
    youLost.setFillColor(sf::Color::Red);
    youLost.setOutlineThickness(3.6);

    //creating shapes
    //ground, dinosaur and sun (in the future) TODO: sun
    auto ground = sf::RectangleShape(sf::Vector2f(1000.f, 30.f));
    ground.setFillColor(sf::Color(244,194,140));
    auto dinosaur = sf::RectangleShape(sf::Vector2f(40.f, 70.f));
    dinosaur.setFillColor(sf::Color(112,178,106));

    // Setting the initial position of these shapes
    auto groundPosition = sf::Vector2f(0.f, 500.f);
    ground.setPosition(groundPosition);
    auto dinosaurPosition = sf::Vector2f(50.f, 430.f);
    dinosaur.setPosition(dinosaurPosition);
    auto dinosaurVel = sf::Vector2f(0.f, 0.f); //dinosaur speed (initial)

    //3 cacti
    //they have three random shapes/sizes: small (40.f, 60.f), medium (60.f, 60.f) and large (80.f, 60.f)
    auto sizes = std::vector<sf::RectangleShape>{
        sf::RectangleShape(sf::Vector2f(40.f, 60.f)),
        sf::RectangleShape(sf::Vector2f(60.f, 60.f)),
        sf::RectangleShape(sf::Vector2f(80.f, 60.f)),
    };
    //coloring shapes
    sizes[0].setFillColor(sf::Color(64,90,61));
    sizes[1].setFillColor(sf::Color(64,90,61));
    sizes[2].setFillColor(sf::Color(64,90,61));

    auto cactus1 = sizes[std::rand() % 3];
    auto cactus2 = sizes[std::rand() % 3];
    auto cactus3 = sizes[std::rand() % 3];

    //cacti starting positions
    auto cactus1pos = sf::Vector2f(1000.f, 445.f);
    cactus1.setPosition(cactus1pos);
    auto cactus2pos = sf::Vector2f(1400.f, 445.f);
    cactus2.setPosition(cactus2pos);
    auto cactus3pos = sf::Vector2f(1800.f, 445.f);
    cactus3.setPosition(cactus3pos);

    //cacti speed
    auto cactiAcc = sf::Vector2f(-0.001, 0.f);
    auto cactus1Vel = sf::Vector2f(-4.f, 0.f);
    auto cactus2Vel = sf::Vector2f(-4.f, 0.f);
    auto cactus3Vel = sf::Vector2f(-4.f, 0.f);

    //constants
    auto spawnPoint = sf::Vector2f(1100.f, 445.f);
    auto lost = false;//flag for when you lost
    auto zeroSpd = sf::Vector2f(0.f, 0.f);

    while (window.isOpen()) {
        auto event = sf::Event();
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            if (event.key.code == sf::Keyboard::Up) {
                if (dinosaurVel == sf::Vector2f(0.f, 0.f) && !lost) {
                    dinosaurVel = sf::Vector2f(0.f, -8.f);
                }
            }
        }

        dinosaurPosition += dinosaurVel;
        dinosaur.setPosition(dinosaurPosition);
        if (dinosaurPosition.y >= 430.f) {
            dinosaurPosition.y = 430.f;
            dinosaurVel = sf::Vector2f(0.f, 0.f);
        } else {
            dinosaurVel += sf::Vector2f(0.f, 0.3f);
        }

        //checking collision:
        if (dinosaur.getGlobalBounds().intersects(cactus1.getGlobalBounds()) ||
            dinosaur.getGlobalBounds().intersects(cactus2.getGlobalBounds()) ||
            dinosaur.getGlobalBounds().intersects(cactus3.getGlobalBounds())) {
            cactus1Vel = zeroSpd;
            cactus2Vel = zeroSpd;
            cactus3Vel = zeroSpd;
            dinosaurVel = zeroSpd;
            lost = true;
        }

        //check for max speed: -8.f
        if (cactus1Vel.x == -7.f){
            cactiAcc = sf::Vector2f(0.f, 0.f);
        } else {
            cactus1Vel += cactiAcc;
            cactus2Vel += cactiAcc;
            cactus3Vel += cactiAcc;
        }

        //changing position and shape
        cactus1pos+=cactus1Vel;
        cactus1.setPosition(cactus1pos);
        if (cactus1pos.x < -100) {
            cactus1pos = spawnPoint;
            cactus1 = sizes[std::rand() % 3];
        }

        cactus2pos+=cactus2Vel;
        cactus2.setPosition(cactus2pos);
        if (cactus2pos.x < -100) {
            cactus2pos = spawnPoint;
            cactus2 = sizes[std::rand() % 3];
        }

        cactus3pos+=cactus3Vel;
        cactus3.setPosition(cactus3pos);
        if (cactus3pos.x < -100) {
            cactus3pos = spawnPoint;
            cactus3 = sizes[std::rand() % 3];
        }

        // Drawing the shapes on the screen
        window.clear(sf::Color(116,149,179));//background
        window.draw(ground);
        window.draw(cactus1);
        window.draw(cactus2);
        window.draw(cactus3);
        window.draw(text);
        window.draw(dinosaur);
        if (lost) window.draw(youLost);
        window.display();
    }
}

//TODO: add reset
//TODO: add variable speed and random distances
//speed: begin with -4.f, and move with -0.02.f steps up to -8.f (?)
//TODO: score counter
//TODO: polish some stuff (text, colors)
//TODO: better shapes (optional). You can make this game about a slime and not a dinosaur
//TODO: improve graphics
