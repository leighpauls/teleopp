#pragma once

#include "CommonTypes.h"

#include <set>
#include <experimental/optional>

namespace teleopp {

/**
 * Represents an historical state of the client's control document.
 */
struct RobotPastState {
  /**
   * The most recent control document we know the robot has been using.
   */
  DocumentPtr latestRobotDocument;

  /**
   * The document was in effect no longer ago that than this
   */
  Time minimumTimeOfLatestRobotDocument;
};

struct ControllerState {
  /**
   * The most recent control document this controller is trying to send.
   */
  DocumentPtr latestDocument;

  /**
   * Most recent info on what the robot is doing. has_value = false if we've never head from the
   * robot.
   */
  std::experimental::optional<RobotPastState> robotPastState;

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
  template <class T>
  TeleoppController(Millis nominalTickDuration, T now, Document initialDocument)
      : TeleoppController(nominalTickDuration,
                          std::chrono::time_point_cast<Millis>(now),
                          std::move(initialDocument)) {}

  TeleoppController(Millis nominalTickDuration, Time now, Document initalDocument);
  virtual ~TeleoppController() = default;

  /**
   * Call this if you want to look at the pending and known-delivered document states before your
   * tick.
   */
  virtual ControllerState checkState();

  /**
   * Call this at least once every nominalTickDuration milliseconds, but you may call it more often
   * if needed.
   */
  virtual ControllerTickAction tick(Time now, Document document) {
    throw std::runtime_error("not implemented");
  }

  /**
   * Call this when we receive network traffic from the robot.
   */
  virtual void onRobotNetPayload(NetPayloadPtr payload) {
    throw std::runtime_error("not implemented");
  }

 private:
  DocumentPtr documentToSend_;
};
} // namespace teleopp
