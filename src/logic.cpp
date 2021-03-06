/* opendlv-logic-test-kiwi
 * logic.cpp
 * Zehua Hou
 */
 
#include "logic.hpp"

Logic::Logic() noexcept:
  m_leftWheelSpeedRequest{},
  m_rightWheelSpeedRequest{},
  m_leftWheelSpeedRequestMutex{},
  m_rightWheelSpeedRequestMutex{},
  m_currentTimeUsMutex{},
{
}

opendlv::proxy::LeftWheelSpeedRequest Logic::getLeftWheelSpeedRequest() noexcept
{
  std::lock_guard<std::mutex> lock(m_leftWheelSpeedRequestMutex);
  return m_leftWheelSpeedRequest;
}
opendlv::proxy::RightWheelSpeedRequest Logic::getRightWheelSpeedRequest() noexcept
{
  std::lock_guard<std::mutex> lock(m_rightWheelSpeedRequestMutex);
  return m_rightWheelSpeedRequest;
}

void Logic::step(cluon::data::TimeStamp t0) noexcept
{
  cluon::data::TimeStamp currentTime = cluon::time::now();
  int64_t currentTimeUs = cluon::time::toMicroseconds(currentTime);
  int64_t startTimeUs = cluon::time::toMicroseconds(t0);
  float t = (currentTimeUs-startTimeUs)/1000000.0f;
  float leftWheelSpeed = 0.0f;
  float rightWheelSpeed = 0.0f;
  float t1 = 3.0f;
  float t2 = 10.0f;
  float v0 = 0.5f;
  
  if (t <= t1) {
    rightWheelSpeed = v0*t/t1;
  } else {
    if (t <= t2) {
    leftWheelSpeed = v0*(t-t1)/t2;
    rightWheelSpeed = v0;
    }
  }
  
  std::lock_guard<std::mutex> lock1(m_leftWheelSpeedRequestMutex);
  std::lock_guard<std::mutex> lock2(m_rightWheelSpeedRequestMutex);
  
  opendlv::proxy::LeftWheelSpeedRequest LeftWheelSpeedRequest;
  leftWheelSpeedRequest.speed(leftWheelSpeed);
  m_leftWheelSpeedRequest = leftWheelSpeedRequest;
  
  opendlv::proxy::RightWheelSpeedRequest RightWheelSpeedRequest;
  rightWheelSpeedRequest.speed(rightWheelSpeed);
  m_rightWheelSpeedRequest = rightWheelSpeedRequest;
 }
}
