#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <sstream>

using namespace sf;

// Function Declarations
void updateBranches(int seed);
const int NUM_BRANCHES = 6;
Sprite branches[NUM_BRANCHES];
enum class side{ LEFT, RIGHT, NONE};    // player from tree
side branchPositions[NUM_BRANCHES];   

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

    // time bar
    RectangleShape timeBar;
    float timeBarStartWidth = 400;
    float timeBarHeight = 80;
    timeBar.setSize(Vector2f(timeBarStartWidth, timeBarHeight));
    timeBar.setFillColor(Color::Red);
    timeBar.setPosition((1920 / 2) - timeBarStartWidth / 2, 980);

    Time gameTimeTotal;
    float timeRemaining = 6.0f;
    float timeBarWidthPerSecond = timeBarStartWidth / timeRemaining;

    // check game status
    bool paused = true;

    // keep track of score
    int score = 0;

    // create in game text
    Text messageText;
    Text scoreText;

    // load font
    Font font;
	font.loadFromFile("fonts/KOMIKAP_.ttf");

    // set font to text message
    messageText.setFont(font);
    scoreText.setFont(font);

    // assign text message w/ font to in game text
    messageText.setString("Press Enter to start!");
    scoreText.setString("Score = 0");

    // set text size
    messageText.setCharacterSize(75);
    scoreText.setCharacterSize(100);

    // set text color
    messageText.setFillColor(Color::White);
    scoreText.setFillColor(Color::White);

    // set text position
    FloatRect textRect = messageText.getLocalBounds();

    messageText.setOrigin(textRect.left + 
        textRect.width / 2.0f,
        textRect.top + 
        textRect.height / 2.0f);

    messageText.setPosition(1920 / 2.0f, 1080 / 2.0f);

    scoreText.setPosition(20, 20);

    // branches
    Texture textureBranch;
    textureBranch.loadFromFile("graphics/branch.png");
    for (int i = 0; i < NUM_BRANCHES; i++) {
        branches[i].setTexture(textureBranch);
        branches[i].setPosition(-2000, -2000);
        branches[i].setOrigin(220, 20);
    }

    // player
    Texture texturePlayer;
    texturePlayer.loadFromFile("graphics/player.png");
    Sprite spritePlayer;
    spritePlayer.setTexture(texturePlayer);
    spritePlayer.setPosition(580, 720);
    side playerSide = side::LEFT;   // start left side

    // gravestone
    Texture textureRIP;
    textureRIP.loadFromFile("graphics/rip.png");
    Sprite spriteRIP;
    spriteRIP.setTexture(textureRIP);
    spriteRIP.setPosition(600, 860);

    // axe
    Texture textureAxe;
    textureAxe.loadFromFile("graphics/axe.png");
    Sprite spriteAxe;
    spriteAxe.setTexture(textureAxe);
    spriteAxe.setPosition(700, 830);

    // axe w/ tree
    const float AXE_POSITION_LEFT = 700;
    const float AXE_POSITION_RIGHT = 1075;

    // flying log
    Texture textureLog;
    textureLog.loadFromFile("graphics/log.png");
    Sprite spriteLog;
    spriteLog.setTexture(textureLog);
    spriteLog.setPosition(810, 720);
    bool logActive = false; // log variables
    float logSpeedX = 1000;
    float logSpeedY = -1500;

    // player input
    bool acceptInput = false;

    // chop sound
	SoundBuffer chopBuffer;
	chopBuffer.loadFromFile("sound/chop.wav");
	Sound chop;
	chop.setBuffer(chopBuffer);

    // death sound
	SoundBuffer deathBuffer;
	deathBuffer.loadFromFile("sound/death.wav");
	Sound death;
	death.setBuffer(deathBuffer);

    // out of time sound
	SoundBuffer ootBuffer;
	ootBuffer.loadFromFile("sound/out_of_time.wav");
	Sound outOfTime;
	outOfTime.setBuffer(ootBuffer);       

    // Coordinate (1919, 1079)
    while (window.isOpen()) {

        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::KeyReleased && !paused) {
                acceptInput = true;             // listen for key press

                spriteAxe.setPosition(2000,     // hide axe
                    spriteAxe.getPosition().y);
            }
        }
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

            // reset 
            score = 0;
            timeRemaining = 5;

            // clear branches
            for (int i = 1; i < NUM_BRANCHES; i++) {
                branchPositions[i] = side::NONE;
            }

            spriteRIP.setPosition(675, 2000);   // hidden RIP stone

            spritePlayer.setPosition(580, 720); // player

            acceptInput = true;
        }

        // when accepting player input
        if (acceptInput) {
            if (Keyboard::isKeyPressed(Keyboard::D)) {
                playerSide = side::RIGHT;

                score++;

                // add time
                timeRemaining += (2 / score) + .15;

                spriteAxe.setPosition(AXE_POSITION_RIGHT,
                    spriteAxe.getPosition().y);

                spritePlayer.setPosition(1200, 720);

                updateBranches(score);              // update branches

                spriteLog.setPosition(810, 720);    // flying log
                logSpeedX = -5000;
                logActive = true;

                acceptInput = false;

                chop.play();                        // chop sound
            }

            if (Keyboard::isKeyPressed(Keyboard::A)) {
                playerSide = side::LEFT;

                score++;

                // add time
                timeRemaining += (2 / score) + .15;

                spriteAxe.setPosition(AXE_POSITION_LEFT,
                    spriteAxe.getPosition().y);

                spritePlayer.setPosition(580, 720);

                updateBranches(score);              // update branches

                spriteLog.setPosition(810, 720);    // flying log
                logSpeedX = 5000;
                logActive = true;

                acceptInput = false;

                chop.play();                        // chop sound
            }
        }

		/*
		****************************************
		Update the scene
		****************************************
		*/
        
        if (!paused) {
            // measure time
            Time dt = clock.restart();              // dt = change in time

            // subtract time
            timeRemaining -= dt.asSeconds();

            // set timeBar size
            timeBar.setSize(Vector2f(timeBarWidthPerSecond * 
                timeRemaining, timeBarHeight));

            if (timeRemaining <= 0.0f) {
                paused = true;  // pause game

                messageText.setString("Out of time!!");

				// retouch text
				FloatRect textRect = messageText.getLocalBounds();
				messageText.setOrigin(textRect.left +
					textRect.width / 2.0f,
					textRect.top +
					textRect.height / 2.0f);

				messageText.setPosition(1920 / 2.0f, 1080 / 2.0f);

                // out of time sound
				outOfTime.play();                
            }

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

            // update score
            std::stringstream ss;
            ss << "Score = " << score;
            scoreText.setString(ss.str());       

            // update branch
            for (int i = 0; i < NUM_BRANCHES; i++) {
                float height = i * 150;

                if (branchPositions[i] == side::LEFT) {
                    branches[i].setPosition(610, height);   // move left
                    branches[i].setRotation(180);           // flip 
                }

                else if (branchPositions[i] == side::RIGHT) {
                    branches[i].setPosition(1330, height);  // move right
                    branches[i].setRotation(0);             // default
                }

                else {
                    branches[i].setPosition(3000, height);
                }
            }

            if (logActive) {
                spriteLog.setPosition(
                    spriteLog.getPosition().x + (logSpeedX * dt.asSeconds()),
                    spriteLog.getPosition().y + (logSpeedY * dt.asSeconds()));

                // bee at right hand side of screen
                if (spriteLog.getPosition().x < -100 || 
                    spriteLog.getPosition().x > 2000) {

                        // set up for new frame
                        logActive = false;
                        spriteLog.setPosition(810, 720);
                    }
            }

            // player death by branch
			if (branchPositions[5] == playerSide)
			{
				paused = true;
				acceptInput = false;

				spriteRIP.setPosition(525, 760);    // replace player w/ RIP
				spritePlayer.setPosition(2000, 660);    

				messageText.setString("SQUISHED!!");    // death text

				// Center it on the screen
				FloatRect textRect = messageText.getLocalBounds();

				messageText.setOrigin(textRect.left +
					textRect.width / 2.0f,
					textRect.top + textRect.height / 2.0f);

				messageText.setPosition(1920 / 2.0f,
					1080 / 2.0f);

				// Play the death sound
				death.play();


			}            

        } // End if (!paused)

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

        // draw branches
        for (int i = 0; i < NUM_BRANCHES; i++) {
            window.draw(branches[i]);
        }

        // draw tree
        window.draw(spriteTree);

        // draw player
        window.draw(spritePlayer);

        // draw axe
        window.draw(spriteAxe);

        // draw flying log
        window.draw(spriteLog);

        // draw gravestone
        window.draw(spriteRIP);

        // draw bee
        window.draw(spriteBee);

        // draw score
        window.draw(scoreText);
        if (paused) {
            window.draw(messageText);
        }

        // draw timebar
        window.draw(timeBar);

        // display drawings
        window.display();
    }

    return 0;
}

// Function Definitions
void updateBranches(int seed) {
    for (int i = NUM_BRANCHES - 1; i > 0; i--) {
        branchPositions[i] = branchPositions[i - 1];
    }

    srand((int)time(0) + seed);
    int r = (rand() % 5);   // rand num 0-4

    switch (r) {
        case 0:
            branchPositions[0] = side::LEFT;
            break;
  
        case 1:
            branchPositions[0] = side::RIGHT;
            break;
  
        default:
            branchPositions[0] = side::NONE;
            break;
  
    }
}