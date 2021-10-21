#include <iostream>
#include <sstream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

using namespace sf;

void updateBranches(int seed);
const int NUM_BRANCHES = 6;
Sprite branches[NUM_BRANCHES];
//where is the player/branch?
//left or right
enum class side {LEFT, RIGHT, NONE};
side branchPositions[NUM_BRANCHES];

int main()
{
    //Create a video mode object
    VideoMode vm(1920, 1080);

    //Create and open a window for the game
    RenderWindow window(vm, "Timber!!!", Style::Fullscreen);

    /***************************************************
                     Make all the sprites
    ****************************************************/

    //Make a background 
    //Create a texture to hold a graphic on the GPU
    Texture textureBackground;
    //Load a graphic into the texture
    textureBackground.loadFromFile("graphics/background.png");
    //Create a sprite
    Sprite spriteBackground;
    //Attatch the texture to the sprite
    spriteBackground.setTexture(textureBackground);
    //Set the spriteBackground to cover the screen
    spriteBackground.setPosition(0, 0);


    //Make a tree sprite
    Texture textureTree;
    textureTree.loadFromFile("graphics/tree.png");
    Sprite spriteTree;
    spriteTree.setTexture(textureTree);
    spriteTree.setPosition(810, 0);

    //add bg trees
    Texture textureTree2;
    textureTree2.loadFromFile("graphics/tree2.png");
    Sprite spriteTree2;
    spriteTree2.setTexture(textureTree2);
    Sprite spriteTree3;
    spriteTree3.setTexture(textureTree2);
    Sprite spriteTree4;
    spriteTree4.setTexture(textureTree2);
    Sprite spriteTree5;
    spriteTree5.setTexture(textureTree2);
    Sprite spriteTree6;
    spriteTree6.setTexture(textureTree2);

    spriteTree2.setPosition(20, 0);
    spriteTree3.setPosition(300, -400);
    spriteTree4.setPosition(1300, -400);
    spriteTree5.setPosition(1500, -500);
    spriteTree6.setPosition(1900, 0);


    //Make a bee sprite
    Texture textureBee;
    textureBee.loadFromFile("graphics/bee.png");
    Sprite spriteBee;
    spriteBee.setTexture(textureBee);
    spriteBee.setPosition(0, 800);
    //is the bee moving?
    bool beeActive = false;
    //how fast can the bee fly
    float beeSpeed = 0.0f;

    //make the clouds with arrays instead
    Texture textureCloud;
    textureCloud.loadFromFile("graphics/cloud.png");

    const int NUM_CLOUDS = 6;
    Sprite clouds[NUM_CLOUDS];
    int cloudSpeeds[NUM_CLOUDS];
    bool cloudsActive[NUM_CLOUDS];

    for (int i = 0; i < NUM_CLOUDS; i++)
    {
        clouds[i].setTexture(textureCloud);
        clouds[i].setPosition(-300, i * 150);
        cloudsActive[i] = false;
        cloudSpeeds[i] = 0;

    }
    //make a more efficient loop for this but save for future reference
    /*
    //Make 3 clouds
    Texture textureCloud;
    textureCloud.loadFromFile("graphics/cloud.png");
    Sprite spriteCloud1;
    Sprite spriteCloud2;
    Sprite spriteCloud3;
    spriteCloud1.setTexture(textureCloud);
    spriteCloud2.setTexture(textureCloud);
    spriteCloud3.setTexture(textureCloud);
    spriteCloud1.setPosition(0, 0);
    spriteCloud2.setPosition(0, 150);
    spriteCloud3.setPosition(0, 300);
    //are the clouds on the screen?
    bool cloud1Active = false;
    bool cloud2Active = false;
    bool cloud3Active = false;
    //how fast is each cloud
    float cloud1Speed = 0.0f;
    float cloud2Speed = 0.0f;
    float cloud3Speed = 0.0f;
    */

    //variables to control time
    Clock clock;

    //Time bar

    RectangleShape timeBar;
    float timeBarStartWidth = 400;
    float timeBarHeight = 80;
    timeBar.setSize(Vector2f(timeBarStartWidth, timeBarHeight));
    timeBar.setFillColor(Color(255, 95, 95));
    timeBar.setPosition((1920 / 2) - timeBarStartWidth / 2, 980);

    Time gameTimeTotal;
    float timeRemaining = 6.0f;
    float timeBarWidthPerSecond = timeBarStartWidth / timeRemaining;

    //Track whether the game is running
    bool paused = true;

    //Draw the text
    int score = 0;
    sf::Text messageText;
    sf::Text scoreText;
    sf::Text instructionText;

    //Chose a font
    Font font;
    font.loadFromFile("fonts/KOMIKAP_.ttf");

    //Set the font
    messageText.setFont(font);
    scoreText.setFont(font);
    instructionText.setFont(font);

    //Assign the message
    messageText.setString("Press Enter to start");
    scoreText.setString("Score = 0");
    instructionText.setString(
        "Press the left and right arrow keys to move\n        Press m to mute or Esc to quit");

    //Set the size of the text
    messageText.setCharacterSize(75);
    scoreText.setCharacterSize(100);
    instructionText.setCharacterSize(30);

    //Set the color
    messageText.setFillColor(Color::White);
    scoreText.setFillColor(Color::White);
    instructionText.setFillColor(Color::White);

    //position the message text
    FloatRect textRect = messageText.getLocalBounds();
    messageText.setOrigin(textRect.left + textRect.width / 2.0f,
        textRect.top + textRect.height / 2.0f);
    messageText.setPosition(1920 / 2.0f, 1080 / 2.0f);
    scoreText.setPosition(20, 20);

    //position the instruction text
    FloatRect textRect2 = instructionText.getLocalBounds();
    instructionText.setOrigin(textRect2.left + textRect2.width / 2.0f,
        textRect2.top + textRect2.height / 2.0f);
    instructionText.setPosition(1920 / 2.0f, 40);

    //background for the score text
    // Backgrounds for the text
    RectangleShape rect1;
    rect1.setFillColor(sf::Color(0, 0, 0, 100));
    rect1.setSize(Vector2f(600, 105));
    rect1.setPosition(0, 30);

    

    //prepare the branches
    Texture textureBranch;
    textureBranch.loadFromFile("graphics/branch.png");
    //set the texture for each branch sprite
    for (int i = 0; i < NUM_BRANCHES; i++) {
        branches[i].setTexture(textureBranch);
        branches[i].setPosition(-2000, -2000);
        //set the sprite origin to the center
        branches[i].setOrigin(220, 20);
    }

    for (int i = 1; i <= 5; i++) {
        updateBranches(i);
    }

    //add the player sprite
    Texture texturePlayer;
    texturePlayer.loadFromFile("graphics/player.png");
    Sprite spritePlayer;
    spritePlayer.setTexture(texturePlayer);
    //flip the sprite for left side
    spritePlayer.scale(-1.f, 1.f);
    spritePlayer.setPosition(730, 700);

    //set player start side
    side playerSide = side::LEFT;

    //prepare the gravestone
    bool dead = false;
    Texture textureRIP;
    textureRIP.loadFromFile("graphics/rip.png");
    Sprite spriteRIP;
    spriteRIP.setTexture(textureRIP);
    spriteRIP.setPosition(600, 860);

    //prepare the axe
    Texture textureAxe;
    textureAxe.loadFromFile("graphics/axe.png");
    Sprite spriteAxe;
    spriteAxe.setTexture(textureAxe);
    spriteAxe.setPosition(700, 770);

    //line the axe up with the tree
    const float AXE_POSITION_LEFT = 700;
    const float AXE_POSITION_RIGHT = 1075;

    //flying log
    Texture textureLog;
    textureLog.loadFromFile("graphics/log.png");
    Sprite spriteLog;
    spriteLog.setTexture(textureLog);
    spriteLog.setPosition(810, 713);

    //log info
    bool logActive = false;
    float logSpeedX = 1000;
    float logSpeedY = -500;

    //control the player input
    bool acceptInput = false;

    //prepare the sounds
    bool mute = false;
    SoundBuffer chopBuffer;
    chopBuffer.loadFromFile("sound/chop.wav");
    Sound chop;
    chop.setBuffer(chopBuffer);
    chop.setVolume(3.0f);

    SoundBuffer deathBuffer;
    deathBuffer.loadFromFile("sound/death.wav");
    Sound death;
    death.setBuffer(deathBuffer);
    death.setVolume(3.0f);

    SoundBuffer ootBuffer;
    ootBuffer.loadFromFile("sound/out_of_time.wav");
    Sound outOfTime;
    outOfTime.setBuffer(ootBuffer);
    outOfTime.setVolume(3.0f);

    while (window.isOpen()) {
        /***************************************************
                     Handle the players input
        ****************************************************/

        Event event;

        while (window.pollEvent(event)) {
            if (event.type == Event::KeyReleased && !paused) {
                //listen for key presses
                acceptInput = true;

                //hide axe
                spriteAxe.setPosition(2000, spriteAxe.getPosition().y);
            }
        }

        if (Keyboard::isKeyPressed(Keyboard::Escape)) {
            window.close();
        }

        //Start the game
        if (Keyboard::isKeyPressed(Keyboard::Return)) {
            paused = false;

            //reset the time and score
            score = 0;
            timeRemaining = 5;
            dead = false;

            //make all the branches disappear
            for (int i = 1; i < NUM_BRANCHES; i++) {
                branchPositions[i] = side::NONE;
            }

            //make sure the gravestone is hidden
            spriteRIP.setPosition(675, 3000);

            //move the player into position
            //flip the player if on right
            if (playerSide == side::RIGHT) {
                spritePlayer.scale(-1.f, 1.f);
            }
            spritePlayer.setPosition(730, 700);
            playerSide = side::LEFT;
            acceptInput = true;
        }

        //player controls
        //accept input
        if (acceptInput) {
            if (Keyboard::isKeyPressed(Keyboard::M)) {
                mute = !(mute);
            }
            //right cursor
            if (Keyboard::isKeyPressed(Keyboard::Right)) {
                //get old position
                int adjust = 0;
                if (playerSide == side::RIGHT) {
                    adjust = 0;
                }
                else {
                    spritePlayer.scale(-1.f, 1.f);
                }
                //have player on right
                playerSide = side::RIGHT;
                score++;

                //add time
                timeRemaining += (2 / score) + .15;

                spriteAxe.setPosition(AXE_POSITION_RIGHT, spriteAxe.getPosition().y);

                //if player was on the left switch to right
                spritePlayer.setPosition(1200, 700);

                //update branches
                updateBranches(score);

                //make the log fly
                spriteLog.setPosition(810, 713);
                logSpeedX = -1000;
                logActive = true;

                acceptInput = false;

                //play chop sound
                if (!mute) {
                    chop.play();
                }                
            }

            //left cursor
            if (Keyboard::isKeyPressed(Keyboard::Left)) {
                int adjust = 0;
                if (playerSide == side::LEFT) {
                    spritePlayer.setPosition(730, 700);
                }
                else {
                    spritePlayer.scale(-1.f, 1.f);
                    //spritePlayer.setPosition(580, 720);
                }
                //have player on left
                playerSide = side::LEFT;
                score++;

                //add time
                timeRemaining += (2 / score) + .15;

                spriteAxe.setPosition(AXE_POSITION_LEFT, spriteAxe.getPosition().y);

                spritePlayer.setPosition(730, 700);

                //update branches
                updateBranches(score);

                //make the log fly
                spriteLog.setPosition(810, 713);
                logSpeedX = 1000;
                logActive = true;

                acceptInput = false;

                //play chop sound
                if (!mute) {
                    chop.play();
                }
            }
        }

        /***************************************************
                     Update the scene
        ****************************************************/
        if (!paused) {

            //measure time
            Time dt = clock.restart();

            //Subtract the amount of time remaining
            timeRemaining -= dt.asSeconds();

            //size the time bar
            timeBar.setSize(Vector2f(timeBarWidthPerSecond * timeRemaining, timeBarHeight));

            //check if time has run out
            if (timeRemaining <= 0.0f) {
                //pause the game
                paused = true;

                //change the message text
                messageText.setString("Out of time!!");

                //reposition the text based on its new size
                FloatRect textRect = messageText.getLocalBounds();
                messageText.setOrigin(textRect.left + textRect.width / 2.0f,
                    textRect.top + textRect.height / 2.0f);
                messageText.setPosition(1920 / 2.0f, 1080 / 2.0f);

                //play out of time sound
                if (!mute) {
                    outOfTime.play();
                }
            }

            //setup the bee
            if (!beeActive) {
                //How fast is the bee?
                srand((int)time(0) * 10);
                beeSpeed = (rand() % 200) + 200;

                //how high is the bee
                srand((int)time(0) * 10);
                float height = (rand() % 500) + 500;
                spriteBee.setPosition(2000, height);
                beeActive = true;
            }
            else {
                //move the bee
                spriteBee.setPosition(
                    spriteBee.getPosition().x - (beeSpeed * dt.asSeconds()),
                    spriteBee.getPosition().y);

                //has the bee reached the right hand edge of the screen?
                if (spriteBee.getPosition().x < -100) {
                    //set it up ready to be a whole new bee next frame
                    beeActive = false;
                }
            }

            //manage the clouds
            // Manage the clouds with arrays
            for (int i = 0; i < NUM_CLOUDS; i++)
            {
                if (!cloudsActive[i])
                {
                    // How fast is the cloud
                    srand((int)time(0) * i);
                    cloudSpeeds[i] = (rand() % 200);

                    // How high is the cloud
                    srand((int)time(0) * i);
                    float height = (rand() % 150);
                    clouds[i].setPosition(-200, height);
                    cloudsActive[i] = true;

                }
                else
                {
                    // Set the new position
                    clouds[i].setPosition(
                        clouds[i].getPosition().x +
                        (cloudSpeeds[i] * dt.asSeconds()),
                        clouds[i].getPosition().y);

                    // Has the cloud reached the right hand edge of the screen?
                    if (clouds[i].getPosition().x > 1920)
                    {
                        // Set it up ready to be a whole new cloud next frame
                        cloudsActive[i] = false;
                    }

                }

                std::stringstream ss;
                ss << "Score = " << score;
                scoreText.setString(ss.str());

                //update the branches
                for (int i = 0; i < NUM_BRANCHES; i++) {
                    float height = i * 150;
                   if (branchPositions[i] == side::LEFT) {
                        //move sprite to left
                        branches[i].setPosition(640, height);
                        //flip sprite
                        branches[i].setRotation(180);
                    }
                    else if (branchPositions[i] == side::RIGHT) {
                        //move sprite to right
                        branches[i].setPosition(1300, height);
                        //set rotation to normal
                        branches[i].setRotation(0);
                    }
                    else {
                        //hide the branch
                        branches[i].setPosition(3000, height);
                    }
                }

                //make log fly
                if (logActive) {
                    spriteLog.setPosition(spriteLog.getPosition().x + (logSpeedX * dt.asSeconds()),
                        spriteLog.getPosition().y + (logSpeedY * dt.asSeconds()));

                    //has the log reached the edge?
                    if (spriteLog.getPosition().x < -100 || spriteLog.getPosition().x > 2000) {
                        //set up a new log next frame
                        logActive = false;
                        spriteLog.setPosition(810, 713);
                    }
                }

                //kill the player
                if (branchPositions[5] == playerSide) {
                    //death
                    paused = true;
                    acceptInput = false;

                    //draw the gravestone
                    spriteRIP.setPosition(890, 760);

                    //hide the player and axe
                    spritePlayer.setPosition(3000, 660);
                    spriteAxe.setPosition(3000, 770);



                    //change the text of the message
                    messageText.setString("You were squished!!");

                    //center the text on the screen
                    FloatRect textRect = messageText.getLocalBounds();

                    messageText.setOrigin(textRect.left + textRect.width / 2.0f,
                        textRect.top + textRect.height / 2.0f);

                    messageText.setPosition(1920 / 2.0f, 1080 / 2.0f);
                    dead = true;

                    //play death sound
                    if (!mute) {
                        death.play();
                    }
                }
            }

            }//end if !paused


            /***************************************************
                         Draw the scene
            ****************************************************/

            //Clear everything from the last frame
            window.clear();

            if (!paused) {
                //Draw our game scene here
                window.draw(spriteBackground);

                for (int i = 0; i < NUM_CLOUDS; i++)
                {
                    window.draw(clouds[i]);
                }

                // bg trees
                window.draw(spriteTree2);
                window.draw(spriteTree3);
                window.draw(spriteTree4);
                window.draw(spriteTree5);
                window.draw(spriteTree6);



                //draw the branches behind the tree
                for (int i = 0; i < NUM_BRANCHES; i++) {
                    window.draw(branches[i]);
                }

                window.draw(spriteTree);

                window.draw(spritePlayer);
                window.draw(spriteAxe);
                window.draw(spriteLog);

                window.draw(spriteBee);

                window.draw(timeBar);

                window.draw(rect1);
                window.draw(scoreText);

            }//end if !paused
            else {
                window.draw(messageText);
                window.draw(instructionText);
                if (dead == true) {
                    window.draw(spriteRIP);
                }
            }


            //Show everything we drew
            window.display();
        
    }
}

void updateBranches(int seed) {
    //move the branches down
    for (int j = NUM_BRANCHES - 1; j > 0; j--) {
        branchPositions[j] = branchPositions[j - 1];
    }

    //spawn new branch at position 0
    //determine left right or none randomly
    srand((int)time(0) + seed);
    int r = (rand() % 5);
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