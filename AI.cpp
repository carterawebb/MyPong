#include "AI.h"

AI::AI(int d, Paddle *cPaddle, Ball *wBall) {
  difficulty = d;
  controlledPaddle = cPaddle;
  watchedBall = wBall;
}

/* -----====== PRIVATE FUNCTIONS =====----- */
int AI::easyAI() {
  // Easy AI logic: Move paddle randomly

  int randomMove = rand() % 3;  // Generate a random number between 0 and 2

  if (randomMove == 0) {
    return -1;  // Move left
  } else if (randomMove == 1) {
    return 1;  // Move right
  }

  return 0;  // No movement
}

int AI::mediumAI() {
  // Medium AI logic: Move paddle towards the predicted position where the
  // ball will hit

  int ballX = watchedBall->getPositionX();
  int ballY = watchedBall->getPositionY();
  int ballSpeedX = watchedBall->getSpeedX();
  int ballSpeedY = watchedBall->getSpeedY();

  // Calculate the estimated y-position where the ball will hit the paddle
  // int estimatedBallY = ballY + (controlledPaddle->getPos().xPos - ballX) *
  //                                 (ballSpeedY / ballSpeedX);
  int estimatedBallY = ballY + (ballX - controlledPaddle->getPos().xPos) *
                                   (ballSpeedY / ballSpeedX);

  if (estimatedBallY < controlledPaddle->getPos().yPos) {
    return -1;  // Move left
  } else if (estimatedBallY >
             controlledPaddle->getPos().yPos + controlledPaddle->getHeight()) {
    return 1;  // Move right
  }

  return 0;  // No movement
}

int AI::hardAI() {
  // Hard AI logic: Move paddle to the exact position where the ball will hit

  int ballX = watchedBall->getPositionX();
  int ballY = watchedBall->getPositionY();
  int paddleCenterX =
      controlledPaddle->getPos().xPos + controlledPaddle->getWidth() / 2;
  int paddleWidth = controlledPaddle->getWidth();
  int ballSpeedX = watchedBall->getSpeedX();
  int ballSpeedY = watchedBall->getSpeedY();
  float estimatedBallY = ballY;

  // Check if the ball is moving towards the paddle
  if (ballSpeedY > 0) {
    // Calculate the estimated x-position where the ball will intersect with the
    // paddle's y-position
    float timeToPaddleY =
        static_cast<float>(controlledPaddle->getPos().yPos +
                           controlledPaddle->getHeight() - ballY) /
        ballSpeedY;
    float estimatedBallX = ballX + timeToPaddleY * ballSpeedX;

    // Check if the estimated x-position is within the paddle's bounds
    if (estimatedBallX >= paddleCenterX - paddleWidth / 2 &&
        estimatedBallX <= paddleCenterX + paddleWidth / 2) {
      estimatedBallY =
          controlledPaddle->getPos().yPos + controlledPaddle->getHeight();
    }
  }

  if (estimatedBallY < controlledPaddle->getPos().yPos) {
    return -1;  // Move left
  } else if (estimatedBallY >
             controlledPaddle->getPos().yPos + controlledPaddle->getHeight()) {
    return 1;  // Move right
  }

  return 0;  // No movement
}

/*
int AI::impossibleAI() {
  // Impossible AI logic: Move paddle to the exact position where the ball will
  // hit, and also move the paddle to the exact position where the ball will
  // hit after it bounces off the wall

  // Obtain the ball's position and velocity
  float ballX = watchedBall->getPositionX() + watchedBall->getWidth() / 2;
  float ballY = watchedBall->getPositionY() + watchedBall->getWidth() / 2;
  float ballVelocityX = watchedBall->getSpeedX();
  float ballVelocityY = watchedBall->getSpeedY();

  // Determine the direction of the ball
  bool ballMovingUp = ballVelocityY < 0;

  // Calculate the predicted position where the ball will cross the AI paddle's
  // vertical position
  float predictedBallY = ballY + (ballVelocityY / ballVelocityX) *
                                     (controlledPaddle->getPos().xPos - ballX);

  // Adjust the predicted position based on the expected ball reflection off the
  // top and bottom of the screen
  int topBoundary = watchedBall->getWidth() / 2;
  int bottomBoundary = SCREEN_HEIGHT - watchedBall->getWidth() / 2;
  int predictedBallYAdjusted = predictedBallY;
  if (predictedBallYAdjusted < topBoundary) {
    predictedBallYAdjusted =
        topBoundary + (topBoundary - predictedBallYAdjusted);
  } else if (predictedBallYAdjusted > bottomBoundary) {
    predictedBallYAdjusted =
        bottomBoundary - (predictedBallYAdjusted - bottomBoundary);
  }

  // Move the AI paddle towards the predicted position
  int paddleSpeed = ballVelocityY;
  if (watchedBall->getPositionY() + watchedBall->getWidth() / 2 <
      predictedBallYAdjusted) {
    controlledPaddle->setPosY(controlledPaddle->getPos().yPos + paddleSpeed);
    return 1;  // Move right
  } else if (controlledPaddle->getPos().yPos +
                 controlledPaddle->getHeight() / 2 >
             predictedBallYAdjusted) {
    controlledPaddle->setPosY(controlledPaddle->getPos().yPos - paddleSpeed);

  }

  return 0;  // No movement
}
*/
int AI::impossibleAI() {
  // Obtain the ball's position
  float ballY = watchedBall->getPositionY() + watchedBall->getWidth() / 2;

  // Move the AI paddle towards the ball's vertical position
  int paddleSpeed = watchedBall->getSpeedY();
  int paddleCenterY =
      controlledPaddle->getPos().yPos + controlledPaddle->getHeight() / 2;

  if (ballY < paddleCenterY) {
    controlledPaddle->setPosY(controlledPaddle->getPos().yPos - paddleSpeed);
    return -1;  // Move up
  } else if (ballY > paddleCenterY) {
    controlledPaddle->setPosY(controlledPaddle->getPos().yPos + paddleSpeed);
    return 1;  // Move down
  }

  return 0;  // No movement
}


/* -----====== PUBLIC FUNCTIONS =====----- */
void AI::aiUpdate() {
  int move = calculateMove();
  controlledPaddle->setDir(move);
}

int AI::calculateMove() {
  if (this->difficulty == 0) {
    return this->easyAI();
  } else if (this->difficulty == 1) {
    return this->mediumAI();
  } else if (this->difficulty == 2) {
    return this->hardAI();
  } else if (this->difficulty == 3) {
    return this->impossibleAI();
  }

  return 0;  // Default case, no movement
}