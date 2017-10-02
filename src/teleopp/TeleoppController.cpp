#include "TeleoppController.h"

#include "ControllerToRobotPayload.h"
#include "RobotToControllerAck.h"

namespace teleopp {

TeleoppController::TeleoppController(Millis nominalTickDuration, Time now, Document initialDocument)
    : controllerDocumentHistory_({ControllerHistoryElement{
          std::make_shared<Document>(std::move(initialDocument)), now, 0, 0,
      }}),
      knownRobotState_(std::experimental::nullopt),
      overrideReasons_({RobotOverrideReason::ROBOT_GONE_ROGUE}) {}

ControllerState TeleoppController::checkState() {
  return ControllerState{
    controllerDocumentHistory_.back().document, knownRobotState_, overrideReasons_,
  };
}

ControllerTickAction TeleoppController::tick(Time now, Document document) {
  controllerDocumentHistory_.push_back(
      ControllerHistoryElement{std::make_shared<Document>(std::move(document)),
                               now,
                               controllerDocumentHistory_.back().controllerVersion + 1,
                               controllerDocumentHistory_.back().basisRobotVersion});
  return ControllerTickAction{checkState(),
                              std::make_shared<NetPayload>(controller_to_robot::build(
                                  controllerDocumentHistory_.back().document,
                                  controllerDocumentHistory_.back().controllerVersion,
                                  now,
                                  controllerDocumentHistory_.back().basisRobotVersion))};
}

void TeleoppController::onRobotNetPayload(NetPayloadPtr payload) {
  uint64_t ackedControllerVersion = robot_to_controller_ack::controllerVersion(payload);
  uint64_t ackedRobotVersion = robot_to_controller_ack::robotVersion(payload);

  // purge history before this ack
  while (controllerDocumentHistory_.front().controllerVersion < ackedControllerVersion) {
    controllerDocumentHistory_.pop_front();
  }
  if (controllerDocumentHistory_.front().controllerVersion == ackedControllerVersion &&
      controllerDocumentHistory_.front().basisRobotVersion == ackedRobotVersion) {
    // This is the most recent ack
    overrideReasons_ = {};
    knownRobotState_ = RobotPastState{
        controllerDocumentHistory_.front().document,
        controllerDocumentHistory_.front().controllerTime,
        ackedControllerVersion,
        ackedRobotVersion,
    };
  }
  // TODO: handle all the other cases
}

} // namespace teleopp
