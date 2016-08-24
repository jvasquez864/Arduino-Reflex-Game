
//player1 win = blue
//player2 win = green

//port numbers for player 1
int player1pt1 = 11;
int player1pt2 = 10;
int player1WinPt = 7;
int player1Button = 8;

//port numbers for player 2
int player2pt1 = 3;
int player2pt2 = 5;
int player2WinPt = 6;
int player2Button = 4;

// port numbers for flashing LED and beeping sound
int flashLED = 12;
int sound = 2;

// arrays of the port numbers for each player's points
int player1points[] = { player1pt1, player1pt2, player1WinPt };
int player2points[] = { player2pt1, player2pt2, player2WinPt };

int p1ptCounter = 0;
int p2ptCounter = 0;

char player1[] = "Player1";
char player2[] = "Player2";

int beepAndFlashDelay = 25;
int possibleIntervalForBeepOrFlash = 5000;
int oneSecondPause = 1000;
int soundFrequency = 262;

int soundPlayStyle = 0;
int lightPlayStyle = 1;

void setup() 
{
  pinMode(sound,OUTPUT);
  pinMode(player1pt1, OUTPUT);
  pinMode(player1pt2, OUTPUT);
  pinMode(player2pt1, OUTPUT);
  pinMode(player2pt2, OUTPUT);
  pinMode(player1WinPt, OUTPUT);
  pinMode(player2WinPt, OUTPUT);
  pinMode(flashLED, OUTPUT);
  
  pinMode(player1Button,INPUT);
  pinMode(player2Button,INPUT);
}

//Checks to see if either of the players are cheating and holding down a button
bool validGame()
{
  if( digitalRead( player1Button ) == HIGH || digitalRead( player2Button ) == HIGH )
  {
    return false;
  }
  return true;
}

// Takes in a char array and int. The 'player' param denotes which player to give a point to, 
// while the 'point' param represents how many points the player has
void givePoint(char player[], int& point)
{
  if( player == player1 ) 
  {
    digitalWrite( player1points[ point ],HIGH );
    ++point;
  }
  else
  {
    digitalWrite( player2points[ point ],HIGH );
    ++point;
  }
}

void playPhase()
{
  //Wait for one of the players to react to the beep/flash 
   while( digitalRead( player1Button ) == LOW && digitalRead( player2Button ) == LOW)
  {
    delay(1);
  }

  digitalRead( player1Button ) == HIGH 
    ? givePoint( player1, p1ptCounter ) 
    : givePoint( player2, p2ptCounter );

}

void playGame( int playStyle )
{
  delay( random( possibleIntervalForBeepOrFlash ) );
  if( !validGame() ) return;

 if( playStyle == soundPlayStyle ){
     tone( sound, soundFrequency, 0 );
     delay( beepAndFlashDelay );
     tone( sound, soundFrequency, 1 );
 }
 else{
    digitalWrite(flashLED, HIGH);
    delay( beepAndFlashDelay );
    digitalWrite(flashLED,LOW);
 }

  playPhase();
}

void resetGame()
{
  p1ptCounter = 0;
  p2ptCounter = 0;
  digitalWrite( player1pt1,LOW );
  digitalWrite( player1pt2,LOW );
  digitalWrite( player2pt1,LOW );
  digitalWrite( player2pt2,LOW );
  digitalWrite( player1WinPt,LOW );
  digitalWrite( player2WinPt,LOW );
  
}

void alertGameBegin()
{
  digitalWrite( player1pt1,HIGH );
  digitalWrite( player1pt2,HIGH );
  digitalWrite( player2pt1,HIGH );
  digitalWrite( player2pt2,HIGH );
  digitalWrite( player1WinPt,HIGH );
  digitalWrite( flashLED,HIGH );
  
  delay( oneSecondPause );
  
  digitalWrite( player1pt1,LOW );
  digitalWrite( player1pt2,LOW );
  digitalWrite( player2pt1,LOW );
  digitalWrite( player2pt2,LOW );
  digitalWrite( player1WinPt,LOW );
  digitalWrite( flashLED,LOW );
}

void alertPlayerWon( int playerPoints[] )
{

// Flash the lights of the player that won 3 times
    for( int i = 0; i < 3; ++i )
    {
        digitalWrite(playerPoints[0],LOW);
        digitalWrite(playerPoints[1],LOW);
        digitalWrite(playerPoints[2],LOW);
        delay( oneSecondPause );
        digitalWrite(playerPoints[0],HIGH);
        digitalWrite(playerPoints[1],HIGH);
        digitalWrite(playerPoints[2],HIGH);
        delay( oneSecondPause );
     }
}


int getGamePlayStyle(){
  if ( digitalRead( player1Button ) == HIGH && digitalRead( player2Button ) == LOW )
    return soundPlayStyle;
  else if ( digitalRead( player1Button ) == LOW && digitalRead( player2Button ) == HIGH )
    return lightPlayStyle;
  else return -1;
}
void gameLoop(){
  int playStyle = getGamePlayStyle();
    if( playStyle != -1 )
  {
    resetGame();
    alertGameBegin();
    while( p1ptCounter < 3 && p2ptCounter < 3) \
    {
      playGame( playStyle );
    }
    
    p1ptCounter == 3 
      ? alertPlayerWon( player1points ) 
      : alertPlayerWon( player2points );

  }
  delay( oneSecondPause );
}
void loop() 
{
  gameLoop();
}


