#pragma once

#include <memory>
#include <string>

namespace teleopp {

// TODO: I could probably templatize away all these std::string's
using Document = std::string;
using DocumentPtr = std::shared_ptr<Document>;

using NetPayload = std::string;
using NetPayloadPtr = std::shared_ptr<std::string>;

enum class RobotOverrideReason {
  ROBOT_GONE_ROGUE,
  END_TO_END_LATENCY_TOO_HIGH,
  TOO_LONG_SINCE_CONTROLLER_SIGNAL,
  TOO_LONG_SINCE_ROBOT_TICK,
};

} // namespace teleopp
