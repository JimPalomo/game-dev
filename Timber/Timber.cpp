#include <SFML/Graphics.hpp>
#include <sstream>

using namespace sf;

int main() {
    /* High resolution */
    // VideoMode vm(1920, 1080);                // videoMode Object

    // RenderWindow window(vm, "Timber!!!");    // create and open window

    /* Low resolution */
    VideoMode vm(960, 540);
    RenderWindow window(vm, "Timber!!!");
    View view(FloatRect(0, 0, 1920, 1080));
    window.setView(view);
    /* End of low res code */

    // create background sprite 
    Texture textureBackground;                                  // declare new texture (photo)
    textureBackground.loadFromFile("graphics/background.png");  // load texture
    Sprite spriteBackground;                                    // create sprite
    spriteBackground.setTexture(textureBackground);             // attach texture to sprite
    spriteBackground.setPosition(0,0);                          // set spriteBackground for whole screen

    // create tree sprite
    Texture textureTree;
    textureTree.loadFromFile("graphics/tree.png");
    Sprite spriteTree;
    spriteTree.setTexture(textureTree);
    spriteTree.setPosition(810, 0);

    // create bee sprite
    Texture textureBee;
    textureBee.loadFromFile("graphics/bee.png");
    Sprite spriteBee;
    spriteBee.setTexture(textureBee);
    spriteBee.setPosition(0, 800);
    bool beeActive = false; // is bee moving?
    float beeSpeed = 0.0f;  // bee's speed

    // create 3 cloud sprite 
    Texture textureCloud;
    textureCloud.loadFromFile("graphics/cloud.png");
    Sprite spriteCloud1;                    // declare 3 cloud sprites
    Sprite spriteCloud2;
    Sprite spriteCloud3;
    spriteCloud1.setTexture(textureCloud);  // set textures to cloud sprites
    spriteCloud2.setTexture(textureCloud);
    spriteCloud3.setTexture(textureCloud);
    spriteCloud1.setPosition(0, 0);         // set cloud sprite positions
    spriteCloud2.setPosition(0, 150);
    spriteCloud3.setPosition(0, 300);
    bool cloud1Active = false;              // are clouds on screen?
    bool cloud2Active = false;
    bool cloud3Active = false;
    float cloud1Speed = 0.0f;               // cloud speed
    float cloud2Speed = 0.0f;
    float cloud3Speed = 0.0f;
    
    // control time
    Clock clock;    // create clock object

    // Coordinate (1919, 1079)
    while (window.isOpen()) {
		/*
		****************************************
		Handle the players input
		****************************************
		*/

        // exit
        if (Keyboard::isKeyPressed(Keyboard::Escape)) {
            window.close();
        }

        // start game
        if (Keyboard::isKeyPressed(Keyboard::Return)) {
            paused = false;
        }

		/*
		****************************************
		Update the scene
		****************************************
		*/
        
        if (!paused) {
            // measure time
            Time dt = clock.restart();              // dt = change in time

            // setup bee
            if (!beeActive) {                       // bee not moving

                // bee's speed
                srand((int)time(0) * 10);           // seed random number generator w/ time
                beeSpeed = (rand() % 200) + 200;    // set bee's speed

                // bee's height
                srand((int)time(0) * 10);
                float height = (rand() % 500) + 500;
                spriteBee.setPosition(2000, height);
                beeActive = true;
            }

            else {  // move bee
                spriteBee.setPosition(
                    spriteBee.getPosition().x - 
                    (beeSpeed * dt.asSeconds()), 
                    spriteBee.getPosition().y);

                // bee is right hand edge of screen
                if (spriteBee.getPosition().x < -100) {
                    beeActive = false;  // stop the bee
                }
            }

            // setup cloud 1
            if (!cloud1Active) { // cloud is not moving

                // speed of cloud
                srand((int)time(0) * 10);
                cloud1Speed = (rand() % 200);

                // height of cloud
                srand((int)time(0)*10);
                float height = (rand() % 150);
                spriteCloud1.setPosition(-200, height);
                cloud1Active = true;
            }

            else {
                spriteCloud1.setPosition(
                    spriteCloud1.getPosition().x + 
                    (cloud1Speed * dt.asSeconds()), 
                    spriteCloud1.getPosition().y);

                // reached right end of screen
                if (spriteCloud1.getPosition().x > 1920) {
                    cloud1Active = false;   // prep for new frame
                }                
            }

            // setup cloud 2
            if (!cloud2Active) { // cloud is not moving

                // speed of cloud
                srand((int)time(0) * 20);
                cloud2Speed = (rand() % 200);

                // height of cloud
                srand((int)time(0)*10);
                float height = (rand() % 300) - 150;
                spriteCloud2.setPosition(-200, height);
                cloud2Active = true;
            }

            else {
                spriteCloud2.setPosition(
                    spriteCloud2.getPosition().x + 
                    (cloud2Speed * dt.asSeconds()), 
                    spriteCloud2.getPosition().y);

                // reached right end of screen
                if (spriteCloud2.getPosition().x > 1920) {
                    cloud2Active = false;   // prep for new frame
                }                
            }   

            // setup cloud 3
            if (!cloud3Active) { // cloud is not moving

                // speed of cloud
                srand((int)time(0) * 30);
                cloud3Speed = (rand() % 200);

                // height of cloud
                srand((int)time(0)*10);
                float height = (rand() % 450) - 150;
                spriteCloud3.setPosition(-200, height);
                cloud3Active = true;
            }

            else {
                spriteCloud3.setPosition(
                    spriteCloud3.getPosition().x + 
                    (cloud3Speed * dt.asSeconds()), 
                    spriteCloud3.getPosition().y);

                // reached right end of screen
                if (spriteCloud3.getPosition().x > 1920) {
                    cloud3Active = false;   // prep for new frame
                }                
            }
        }              

		/*
		****************************************
		Draw the scene
		****************************************
		*/        

        // clear frame from last scene
        window.clear();

        // draw game scene
        window.draw(spriteBackground);

        // draw clouds
        window.draw(spriteCloud1);
        window.draw(spriteCloud2);
        window.draw(spriteCloud3);

        // draw tree
        window.draw(spriteTree);

        // draw bee
        window.draw(spriteBee);

        // display drawing
        window.display();
    }

    return 0;
}