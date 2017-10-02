#include "TeleoppController.h"

#include "ControllerToRobotPayload.h"

namespace teleopp {

TeleoppController::TeleoppController(Millis nominalTickDuration, Time now, Document initialDocument)
    : controllerDocumentHistory_({ControllerHistoryElement{
          std::make_shared<Document>(std::move(initialDocument)), now, 0, 0,
      }}) {}

ControllerState TeleoppController::checkState() {
  return ControllerState{
      controllerDocumentHistory_.back().document, {}, {RobotOverrideReason::ROBOT_GONE_ROGUE},
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

} // namespace teleopp
