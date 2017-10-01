#include "TeleoppController.h"

namespace teleopp {

TeleoppController::TeleoppController(Millis nominalTickDuration, Time now, Document initialDocument)
    : documentToSend_(std::make_shared<Document>(std::move(initialDocument))) {}

ControllerState TeleoppController::checkState() {
  return ControllerState{documentToSend_, {}, {RobotOverrideReason::ROBOT_GONE_ROGUE}};
}

} // namespace teleopp
