#define _CRT_SECURE_NO_DEPRECATE
#include "stdafx.h"
#include "Source.h"
#include "Leap.h"
#include <iostream>
#include "Socket.h"

void SampleListener::onInit(const Controller& controller) {
  
  std::cout << "Initialized" << std::endl;
  countFlag = 0;
  p = &setPotitionOfPalm;
  pCountFlag = &countFlag;
  gapDirection = 25.0;
  isEngineRunning = false;
  right = 1;
  left = 2;
  up = 3;
  down = 4;
  forward = 5;
  backward = 6;
}

void SampleListener::onConnect(const Controller& controller) {
  //std::cout << "Connected" << std::endl;
  controller.enableGesture(Gesture::TYPE_CIRCLE);
  controller.enableGesture(Gesture::TYPE_KEY_TAP);
  controller.enableGesture(Gesture::TYPE_SCREEN_TAP);
  controller.enableGesture(Gesture::TYPE_SWIPE);
}

void SampleListener::onDisconnect(const Controller& controller) {
  //Note: not dispatched when running in a debugger.
  //std::cout << "Disconnected" << std::endl;
}

void SampleListener::onExit(const Controller& controller) {
  //std::cout << "Exited" << std::endl;
}

void SampleListener::onFrame(const Controller& controller) {
  // Get the most recent frame and report some basic information
  const Frame frame = controller.frame();

  if (!frame.hands().isEmpty()) {
    // Get the first hand
    const Hand hand = frame.hands()[0];
	//if(isEngineRunning == false){
	//	std::cout<<"engine is ready to start on"<<std::endl;
	//	isEngineRunning = true;
	//}

    // Check if the hand has any fingers
    const FingerList fingers = hand.fingers();
    if (!fingers.isEmpty()) {
		DirectionTo(fingers, hand);

		//InvalidateRect(hWnd, &rect, FALSE);
      // Calculate the hand's average finger tip position
      Vector avgPos;
      for (int i = 0; i < fingers.count(); ++i) {
        avgPos += fingers[i].tipPosition();
      }
      avgPos /= (float)fingers.count();
    }


    // Get the hand's normal vector and direction
    const Vector normal = hand.palmNormal();
    const Vector direction = hand.direction();

  }
  else if(frame.hands().isEmpty()) {
	  *pCountFlag = 0;
	  //isEngineRunning = false;
  }

  // Get gestures
  const GestureList gestures = frame.gestures();
  for (int g = 0; g < gestures.count(); ++g) {
    Gesture gesture = gestures[g];

    switch (gesture.type()) {
      case Gesture::TYPE_CIRCLE:
      {
        CircleGesture circle = gesture;
        std::string clockwiseness;

        if (circle.pointable().direction().angleTo(circle.normal()) <= PI/4) {
          clockwiseness = "clockwise";
        } else {
          clockwiseness = "counterclockwise";
        }

        // Calculate angle swept since last frame
        float sweptAngle = 0;
        if (circle.state() != Gesture::STATE_START) {
          CircleGesture previousUpdate = CircleGesture(controller.frame(1).gesture(circle.id()));
          sweptAngle = (circle.progress() - previousUpdate.progress()) * 2 * PI;
        }
        break;
      }
	  case Gesture::TYPE_SWIPE:
      {
        SwipeGesture swipe = gesture;
        break;
      }
      case Gesture::TYPE_KEY_TAP:
      {
        KeyTapGesture tap = gesture;
        break;
      }
      case Gesture::TYPE_SCREEN_TAP:
      {
        ScreenTapGesture screentap = gesture;
        break;
      }
      default:

        break;
    }
  }

  if (!frame.hands().isEmpty() || !gestures.isEmpty()) {
    std::cout << std::endl;
  }
}

void SampleListener::DirectionTo(FingerList _fingers, Hand _hand)
{
	if(_fingers.count() >= 3){
		if(*pCountFlag == 0){
			*pCountFlag = 1;
			*p = _hand.palmPosition();
		}
		if(_hand.palmPosition().x > p->x+gapDirection){
			direction = "right";
		}
		else if(_hand.palmPosition().x < p->x-gapDirection){
			direction = "left";
		}
		else if(_hand.palmPosition().y > p->y+gapDirection){
			direction = "up";
		}
		else if(_hand.palmPosition().y < p->y-gapDirection){
			direction = "down";
		}
		else if(_hand.palmPosition().z > p->z+gapDirection){
			direction = "backward";
		}
		else if(_hand.palmPosition().z < p->z-gapDirection){
			direction = "forward";
		}
		else
			direction = "stay";
	}
}

void SampleListener::onFocusGained(const Controller& controller) {
  std::cout << "Focus Gained" << std::endl;
}

void SampleListener::onFocusLost(const Controller& controller) {
  std::cout << "Focus Lost" << std::endl;
}
