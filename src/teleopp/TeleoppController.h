#pragma once

#include "CommonTypes.h"

namespace teleopp {

struct ControllerState {
  /**
   * The most recent control document this controller is trying to send.
   */
  DocumentPtr latestDocument;

  /**
   * The most recent control document we know the robot has been using.
   */
  DocumentPtr latestRobotDocument;

  /**
   * latestRobotDocument is no more stale than this time.
   */
  Time minimumTimeOfLatestRobotDocument;

  /**
   * If this is non-empty, the robot thinks your controller has lost control, and
   * latestRobotDocument is a robot override.
   */
  std::set<RobotOverrideReason> overrideReasons;
};

struct ControllerTickAction {
  ControllerState controllerState;

  NetPayloadPtr outboundPayload;
};

class TeleoppController {
 public:
  TeleoppController(Millis nominalTickDuration, Time now);

  /**
   * Call this if you want to look at the pending and known-delivered document states before your
   * tick.
   */
  ControllerState checkState();

  /**
   * Call this at least once every nominalTickDuration milliseconds, but you may call it more often
   * if needed.
   */
  ControllerTickAction tick(Time now, Document document);

  /**
   * Call this when we receive network traffic from the robot.
   */
  void onRobotNetPayload(NetPayloadPtr payload);
};
} // namespace teleopp
