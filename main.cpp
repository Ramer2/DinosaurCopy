#include <bits/stdc++.h>
#include <SFML/Graphics.hpp>

auto main() -> int {
    auto window = sf::RenderWindow(sf::VideoMode(1000, 600), "Dinosaur",
                                   sf::Style::Default, sf::ContextSettings(0, 0, 8));
    window.setFramerateLimit(60);

    auto begin = std::chrono::high_resolution_clock::now();//this is required for score

    //loading up the font
    auto font = sf::Font();
    if (!font.loadFromFile("../Arial.ttf")) {
        return EXIT_FAILURE;
    }

    //random generator
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist3(1,3); // distribution in range [1, 3]

    //constants
    auto spawnPoint = sf::Vector2f(1100.f, 445.f);
    auto startVel = sf::Vector2f(-4.f, 0.f);
    auto lost = false;//flag for when you lose
    auto zeroSpd = sf::Vector2f(0.f, 0.f);

    //text fields
    auto text = sf::Text("Use arrow up", font, 36);
    text.setPosition({700.f, 50.f});
    text.setFillColor(sf::Color::White);
    text.setOutlineThickness(1.5);

    auto youLost = sf::Text("You Lost!", font, 70);
    youLost.setPosition({325.f, 240.f});
    youLost.setFillColor(sf::Color::Red);
    youLost.setOutlineThickness(3.6);

    auto score = sf::Text("", font, 36);
    score.setPosition(sf::Vector2f(470.f, 45.f));
    score.setFillColor(sf::Color::White);
    score.setOutlineThickness(1.5);

    //button + button text
    auto resetBtn = sf::RectangleShape(sf::Vector2f(150.f, 75.f));
    resetBtn.setPosition({390.f, 330.f});
    resetBtn.setFillColor(sf::Color::Red);
    resetBtn.setOutlineThickness(3.6);
    resetBtn.setOutlineColor(sf::Color::Black);

    auto resetTxt = sf::Text("Reset", font, 36);
    resetTxt.setPosition({415.f, 340.f});
    resetTxt.setOutlineThickness(3.6);

    //creating shapes
    //ground, dinosaur and sun
    auto ground = sf::RectangleShape(sf::Vector2f(1000.f, 30.f));
    ground.setFillColor(sf::Color(244,194,140));
    auto dinosaur = sf::RectangleShape(sf::Vector2f(40.f, 70.f));
    dinosaur.setFillColor(sf::Color(112,178,106));
    auto sun = sf::CircleShape(125.f);
    sun.setFillColor(sf::Color::Yellow);
    sun.setPosition(sf::Vector2f(-100.f, -100.f));

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

    auto cactus1 = sizes[dist3(rng) % 3];
    auto cactus2 = sizes[dist3(rng) % 3];
    auto cactus3 = sizes[dist3(rng) % 3];

    //cacti starting positions
    auto cactus1pos = sf::Vector2f(1000.f, 445.f);
    cactus1.setPosition(cactus1pos);
    auto cactus2pos = sf::Vector2f(1400.f, 445.f);
    cactus2.setPosition(cactus2pos);
    auto cactus3pos = sf::Vector2f(1800.f, 445.f);
    cactus3.setPosition(cactus3pos);

    //cacti speed & acceleration
    auto cactiAcc = sf::Vector2f(-0.001, 0.f);
    auto cactus1Vel = startVel;
    auto cactus2Vel = startVel;
    auto cactus3Vel = startVel;

    while (window.isOpen()) {
        dinosaurPosition += dinosaurVel;
        dinosaur.setPosition(dinosaurPosition);
        if (dinosaurPosition.y >= 430.f) {
            dinosaurPosition.y = 430.f;
            dinosaurVel = sf::Vector2f(0.f, 0.f);
        } else {
            dinosaurVel += sf::Vector2f(0.f, 0.3f);
        }

//        checking collision:
        if ((dinosaur.getGlobalBounds().intersects(cactus1.getGlobalBounds()) ||
             dinosaur.getGlobalBounds().intersects(cactus2.getGlobalBounds()) ||
             dinosaur.getGlobalBounds().intersects(cactus3.getGlobalBounds())) && !lost) {
            cactus1Vel = zeroSpd;
            cactus2Vel = zeroSpd;
            cactus3Vel = zeroSpd;
            dinosaurVel = zeroSpd;
            cactiAcc = sf::Vector2f(0.f, 0.f);
            lost = true;
        }

        auto event = sf::Event();
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            if (event.key.code == sf::Keyboard::Up) {
                if (dinosaurPosition.y >= 430.f && !lost) {
                    dinosaurVel = sf::Vector2f(0.f, -8.f);
                }
            }
            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    auto mousePos = sf::Mouse::getPosition(window);
                    if (resetBtn.getGlobalBounds().contains({static_cast<float>(mousePos.x), static_cast<float> (mousePos.y)}) && lost) {
                        cactus1Vel = startVel;
                        cactus2Vel = startVel;
                        cactus3Vel = startVel;
                        cactus1pos = sf::Vector2f(1000.f, 445.f);
                        cactus2pos = sf::Vector2f(1400.f, 445.f);
                        cactus3pos = sf::Vector2f(1800.f, 445.f);
                        cactiAcc = sf::Vector2f(-0.001, 0.f);
                        score.setString("");
                        begin = std::chrono::high_resolution_clock::now();
                        lost = false;
                    }
                }
            }
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
        cactus1pos += cactus1Vel;
        cactus1.setPosition(cactus1pos);
        if (cactus1pos.x < -100) {
            cactus1pos = spawnPoint;
            cactus1 = sizes[dist3(rng) % 3];
        }

        cactus2pos += cactus2Vel;
        cactus2.setPosition(cactus2pos);
        if (cactus2pos.x < -100) {
            cactus2pos = spawnPoint;
            cactus2 = sizes[dist3(rng) % 3];
        }

        cactus3pos += cactus3Vel;
        cactus3.setPosition(cactus3pos);
        if (cactus3pos.x < -100) {
            cactus3pos = spawnPoint;
            cactus3 = sizes[dist3(rng) % 3];
        }

        // Drawing the shapes on the screen
        window.clear(sf::Color(116,149,179));//background
        window.draw(ground);
        window.draw(cactus1);
        window.draw(cactus2);
        window.draw(cactus3);
        window.draw(text);
        window.draw(dinosaur);
        if (lost) {
            window.draw(youLost);
            window.draw(resetBtn);
            window.draw(resetTxt);
        }
        window.draw(sun);

        //count the score
        if (!lost){
            auto end = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
            int val = duration.count() / 1000;
            score.setString(std::to_string(val));
        }

        window.draw(score);
        window.display();
    }
}

//speed: begin with -4.f, and move with -0.001.f steps up to -7.f (?)
//TODO: polish some stuff (text, colors)
//TODO: add random distances between cacti (kinda works with acceleration, maybe add some random distance between cacti)
//TODO: better shapes (optional). You can make this game about a slime and not a dinosaur
//TODO: improve graphics
