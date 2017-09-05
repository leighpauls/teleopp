#pragma once

#include "CommonTypes.h"

namespace teleopp {

struct RobotControlAction {
  /**
   * You should update all of your outputs according to this document.
   */
  DocumentPtr document;

  /**
   * If this is non-null, you should send it to the controller.
   */
  NetPayloadPtr nullableOutboundPayload;
};

struct RobotTickAction {

  RobotControlAction robotControlAction;

  /**
   * If this is non-empty, you should consider that the controller has lost control of this robot.
   * You should call overrideControlDocument with your new control document.
   */
  std::set<RobotOverrideReason> overrideReasons;
};

class TeleoppRobot {
 public:
  TeleoppRobot(Millis nominalTickDuration,
               Millis maxRoundTripLatency,
               Millis maxRobotTickDuration,
               Millis maxControllerTickDuration,
               Time now);

  /**
   * Call this when the robot needs to override the control document. If the most recent
   * `tick(now).overrideReasons` was empty, the override reason will become ROBOT_GONE_ROGUE, and
   * the next tick will attempt to reset the control document according to the controller.
   */
  RobotControlAction overrideControlDocument(Time now, DocumentPtr document);

  /**
   * Call this at least once every nominalTickDuration, and follow the instructions of the
   * RobotTickAction.
   */
  RobotTickAction tick(Time now);

  /**
   * Call this when we receive network traffic from the controller.
   */
  void onControllerNetPayload(NetPayloadPtr payload);
};
} // namespace teleopp
