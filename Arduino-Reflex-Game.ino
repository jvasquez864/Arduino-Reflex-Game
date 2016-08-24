
//player1 win = blue
//player2 win = green

//port numbers for player 1
int player1pt1Port = 11;
int player1pt2Port = 10;
int player1WinPtPort = 7;
int player1ButtonPort = 8;

//port numbers for player 2
int player2pt1Port = 3;
int player2pt2Port = 5;
int player2WinPtPort = 6;
int player2ButtonPort = 4;

// port numbers for flashing LED and beeping sound
int flashingLEDPort = 12;
int beepingSoundPort = 2;

// arrays of the port numbers for each player's points
int player1points[] = { player1pt1Port, player1pt2Port, player1WinPtPort };
int player2points[] = { player2pt1Port, player2pt2Port, player2WinPtPort };

int p1ptCounter = 0;
int p2ptCounter = 0;

char player1[] = "Player1";
char player2[] = "Player2";

//Delay values are in units of ms
int beepAndFlashDelay = 25;
int possibleIntervalForBeepOrFlash = 5000;
int oneSecondDelay = 1000;
int soundFrequency = 262;

int soundPlayStyle = 0;
int lightPlayStyle = 1;

void setup() 
{
  pinMode(beepingSoundPort,OUTPUT);
  pinMode(player1pt1Port, OUTPUT);
  pinMode(player1pt2Port, OUTPUT);
  pinMode(player2pt1Port, OUTPUT);
  pinMode(player2pt2Port, OUTPUT);
  pinMode(player1WinPtPort, OUTPUT);
  pinMode(player2WinPtPort, OUTPUT);
  pinMode(flashingLEDPort, OUTPUT);
  
  pinMode(player1ButtonPort,INPUT);
  pinMode(player2Button,INPUT);
}

//Checks to see if either of the players are cheating and holding down a button
bool validGame()
{
  if( digitalRead( player1ButtonPort ) == HIGH || digitalRead( player2ButtonPort ) == HIGH )
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
   while( digitalRead( player1ButtonPort ) == LOW && digitalRead( player2Button ) == LOW)
  {
    delay(1);
  }

  digitalRead( player1ButtonPort ) == HIGH 
    ? givePoint( player1, p1ptCounter ) 
    : givePoint( player2, p2ptCounter );

}

void playGame( int playStyle )
{
  delay( random( possibleIntervalForBeepOrFlash ) );
  if( !validGame() ) return;

 if( playStyle == soundPlayStyle ){
     tone( beepingSoundPort, soundFrequency, 0 );
     delay( beepAndFlashDelay );
     tone( beepingSoundPort, soundFrequency, 1 );
 }
 else{
    digitalWrite(flashingLEDPort, HIGH);
    delay( beepAndFlashDelay );
    digitalWrite(flashingLEDPort,LOW);
 }

  playPhase();
}

void resetGame()
{
  p1ptCounter = 0;
  p2ptCounter = 0;
  digitalWrite( player1pt1Port,LOW );
  digitalWrite( player1pt2Port,LOW );
  digitalWrite( player2pt1Port,LOW );
  digitalWrite( player2pt2Port,LOW );
  digitalWrite( player1WinPtPort,LOW );
  digitalWrite( player2WinPtPort,LOW );
  
}

void alertGameBegin()
{
  digitalWrite( player1pt1Port,HIGH );
  digitalWrite( player1pt2Port,HIGH );
  digitalWrite( player2pt1Port,HIGH );
  digitalWrite( player2pt2Port,HIGH );
  digitalWrite( player1WinPtPort,HIGH );
  digitalWrite( flashingLEDPort,HIGH );
  
  delay( oneSecondDelay );
  
  digitalWrite( player1pt1Port,LOW );
  digitalWrite( player1pt2Port,LOW );
  digitalWrite( player2pt1Port,LOW );
  digitalWrite( player2pt2Port,LOW );
  digitalWrite( player1WinPtPort,LOW );
  digitalWrite( flashingLEDPort,LOW );
}

void alertPlayerWon( int playerPoints[] )
{

// Flash the lights of the player that won 3 times
    for( int i = 0; i < 3; ++i )
    {
        digitalWrite(playerPoints[0],LOW);
        digitalWrite(playerPoints[1],LOW);
        digitalWrite(playerPoints[2],LOW);
        delay( oneSecondDelay );
        digitalWrite(playerPoints[0],HIGH);
        digitalWrite(playerPoints[1],HIGH);
        digitalWrite(playerPoints[2],HIGH);
        delay( oneSecondDelay );
     }
}


int getGamePlayStyle(){
  if ( digitalRead( player1ButtonPort ) == HIGH && digitalRead( player2Button ) == LOW )
    return soundPlayStyle;
  else if ( digitalRead( player1ButtonPort ) == LOW && digitalRead( player2Button ) == HIGH )
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
  delay( oneSecondDelay );
}
void loop() 
{
  gameLoop();
}


