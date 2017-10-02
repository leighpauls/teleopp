#pragma once

#include "CommonTypes.h"

namespace teleopp {

struct robot_to_controller_ack {
  static NetPayload build(uint64_t controllerVersion, uint64_t robotVersion);

  static uint64_t controllerVersion(NetPayloadPtr payload);
  static uint64_t robotVersion(NetPayloadPtr payload);
};
}
