#include <bits/stdc++.h>
#include <SFML/Graphics.hpp>

//while playing: SPRITES ARE SQUARES (I tried to trim them, really, but I couldn't make it work)
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
    std::uniform_int_distribution<int> dist3(1,3); // distribution in range [1, 3]
    std::uniform_int_distribution<int> dist50(-50, 50); // distribution in range [-100, 100]

    //constants
    auto spawnPoint = sf::Vector2f(1100.f, 415.f);
    auto startVel = sf::Vector2f(-4.5f, 0.f);
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

    //importing textures
    auto dinoTexture = sf::Texture();
    if (!dinoTexture.loadFromFile("../dino.png")) std::cout << "Error, while loading a dino file\n";

    auto smallCactusTexture = sf::Texture();
    if(!smallCactusTexture.loadFromFile("../small_cactus.png")) std::cout << "Error, while loading a small cactus file\n";

    auto mediumCactusTexture = sf::Texture();
    if(!mediumCactusTexture.loadFromFile("../medium_cactus.png")) std::cout << "Error, while loading a medium cactus file\n";

    auto largeCactusTexture = sf::Texture();
    if(!largeCactusTexture.loadFromFile("../large_cactus.png")) std::cout << "Error, while loading a medium cactus file\n";

    //creating shapes
    //ground, dinosaur and sun
    auto ground = sf::RectangleShape(sf::Vector2f(1000.f, 150.f));
    ground.setFillColor(sf::Color(247, 247, 247, 255));
    ground.setOutlineColor(sf::Color(83, 83, 83,255));
    ground.setOutlineThickness(5.f);
    auto dinosaur = sf::Sprite(dinoTexture);
    dinosaur.setScale(0.7, 0.9);
    auto sun = sf::CircleShape(125.f);
    sun.setFillColor(sf::Color(247, 247, 247, 255));
    sun.setPosition(sf::Vector2f(-100.f, -100.f));
    sun.setOutlineThickness(5.f);
    sun.setOutlineColor(sf::Color(83, 83, 83,255));

    // Setting the initial position of these shapes
    auto groundPosition = sf::Vector2f(0.f, 490.f);
    ground.setPosition(groundPosition);
    auto dinosaurPosition = sf::Vector2f(50.f, 400.f);
    dinosaur.setPosition(dinosaurPosition);
    auto dinosaurVel = sf::Vector2f(0.f, 0.f); //dinosaur speed (initial)

    //3 cacti
    auto cacti = std::vector<sf::Sprite>{
        sf::Sprite(smallCactusTexture),
        sf::Sprite(mediumCactusTexture),
        sf::Sprite(largeCactusTexture)
    };

    auto cactus1 = cacti[dist3(rng) % 3];
    auto cactus2 = cacti[dist3(rng) % 3];
    auto cactus3 = cacti[dist3(rng) % 3];

    //scaling sprites
    cactus1.setScale(0.7f, 0.85f);
    cactus2.setScale(0.7f, 0.85f);
    cactus3.setScale(0.7f, 0.85f);

    //cacti starting positions
    auto cactus1pos = sf::Vector2f(1000.f, 415.f);
    cactus1.setPosition(cactus1pos);
    auto cactus2pos = sf::Vector2f(1400.f, 415.f);
    cactus2.setPosition(cactus2pos);
    auto cactus3pos = sf::Vector2f(1800.f, 415.f);
    cactus3.setPosition(cactus3pos);

    //cacti speed & acceleration
    auto cactiAcc = sf::Vector2f(-0.001, 0.f);
    auto cactus1Vel = startVel;
    auto cactus2Vel = startVel;
    auto cactus3Vel = startVel;

    while (window.isOpen()) {
        dinosaurPosition += dinosaurVel;
        dinosaur.setPosition(dinosaurPosition);
        if (dinosaurPosition.y >= 410.f) {
            dinosaurPosition.y = 410.f;
            dinosaurVel = sf::Vector2f(0.f, 0.f);
        } else if (!lost) {
            dinosaurVel += sf::Vector2f(0.f, 0.25f);
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
                if (dinosaurPosition.y >= 400.f && !lost) {
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
                        cactus1pos = sf::Vector2f(1000.f, 415.f);
                        cactus2pos = sf::Vector2f(1400.f, 415.f);
                        cactus3pos = sf::Vector2f(1800.f, 415.f);
                        cactiAcc = sf::Vector2f(-0.001, 0.f);
                        dinosaurPosition = sf::Vector2f(50.f, 410.f);
                        score.setString("");
                        begin = std::chrono::high_resolution_clock::now();
                        lost = false;
                    }
                }
            }
        }

//        Easter Egg: uncomment lines [156, 160] to play special mode))
//        if (cactus1Vel.x >= -5.5f) {
//            cactus1pos.x += dist100(rng);
//            cactus2pos.x += dist100(rng);
//            cactus3pos.x += dist100(rng);
//        }

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
            cactus1pos.x += dist50(rng);
            cactus1 = cacti[dist3(rng) % 3];
        }

        cactus2pos += cactus2Vel;
        cactus2.setPosition(cactus2pos);
        if (cactus2pos.x < -100) {
            cactus2pos = spawnPoint;
            cactus2pos.x += dist50(rng);
            cactus2 = cacti[dist3(rng) % 3];
        }

        cactus3pos += cactus3Vel;
        cactus3.setPosition(cactus3pos);
        if (cactus3pos.x < -100) {
            cactus3pos = spawnPoint;
            cactus3pos.x += dist50(rng);
            cactus3 = cacti[dist3(rng) % 3];
        }

        // Drawing the shapes on the screen
        window.clear(sf::Color(247,247,247,255));//background
        window.draw(ground);
        window.draw(dinosaur);
        window.draw(cactus1);
        window.draw(cactus2);
        window.draw(cactus3);
        window.draw(text);
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
            auto val = duration.count() / 1000;
            score.setString(std::to_string(val));
        }

        window.draw(score);
        window.display();
    }
}